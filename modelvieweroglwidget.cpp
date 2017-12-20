#define _USE_MATH_DEFINES
extern "C" {
#include "mymath.h"
#include "modelvbo.h"
#include "image.h"
#include "ray.h"
}
#include "LoadShaders.h"
#include "modelvieweroglwidget.hpp"
#include <QMenu>
#include <QPoint>
#include <math.h>

enum RENDER_MODE { RENDER_POLYGON, RENDER_WIREFRAME, RENDER_PICKING, RENDER_LINES };
enum OP_MODE { VIEW_MODE, TRANSLATE_MODE, ROTATE_MODE, SCALE_MODE };

extern float g_originWidth;
extern float g_originHeight;


ModelViewerOGLWidget::ModelViewerOGLWidget(QWidget * parent) : QOpenGLWidget(parent)
{
	int i;

	opMode = VIEW_MODE;

	iconSize = 32;

	eye[0] = 1; eye[1] = 0; eye[2] = 0;
	lok[0] = 0; lok[1] = 0; lok[2] = 0;
	vup[0] = 0, vup[1] = 1, vup[2] = 0;
	viewTrans[0] = 0; viewTrans[1] = 0; viewTrans[2] = 0;
	viewRot[0] = 0; viewRot[1] = 0; viewRot[2] = 0;
	viewPhi = radians(90);
	viewTheta = radians(60);
	viewRadius = 400.0;
	fovy = 45.0;
	nearClip = 0.1;
	farClip = 1000.0;
	fixView();

	// selected
	selectedItem = NULL;
	selectedObject = NULL;

	// light
	for (i = 0; i < 10; ++i)
	{
		lights[i] = NULL;
	}
	numLights = 0;
	lights[numLights++] = newLight("light0");
	globalLight = newGlobalLight();
	setLightAmbient(globalLight, 0.3, 0.3, 0.3, 1.0);
	setLightPosition(lights[0], 1, 1, 1, 0);
	setLightDirection(lights[0], 1, 1, 1);
	setLightDiffuse(lights[0], 0.45, 0.45, 0.45, 1.0);
	setLightSpecular(lights[0], 0.6, 0.6, 0.6, 1.0);

	lightWidget = new LightWidget(numLights, lights, globalLight, this);
	lightWidget->close();
	//managerWidget = new ModelSceneManagerWidget(this);
	//managerWidget->hide();
	//managerWidget->show();
	textureWidget = new ImageProjectionWidget(this);
	textureWidget->hide();
	//textureWidget->show();
	voxelizeWidget = new VoxelizeWidget(this);
	voxelizeWidget->hide();
	//voxelizeWidget->show();

	// initialze vbo objects
	projCylinder = NULL;

	axisX = NULL;
	axisY = NULL;
	axisZ = NULL;
	axesTsfm = newTransformation();
	selectedAxis = EMPTY_ID;

	rotCircleX = NULL;	
	rotCircleY = NULL;
	rotCircleZ = NULL;
	rotCircleTsfm = newTransformation();
	selectedRotCircle = EMPTY_ID;

	scaleAxisX = NULL;
	scaleAxisY = NULL;
	scaleAxisZ = NULL;
	scaleAxesTsfm = newTransformation();
	selectedScaleAxis = EMPTY_ID;

	modifyingTransformation = newTransformation();


	// flags settings
	activeTextureMapping = false;

	// clip
	clip[0] = 0;
	clip[1] = 0;
	clip[2] = 0;
	clip[3] = 0;

	// ssao
	kernelSize = 64;
	noiseSize = 4.0;
	kernelRadius = 1.0;
	quadVAO = 0;
	ssaoKernel = (GLfloat*)calloc(kernelSize * 3, sizeof(GLfloat));
	ssaoNoise = (GLfloat*)calloc(noiseSize * noiseSize * 3, sizeof(GLfloat));


	// shadow mapping
	shadowWidth = 4096;
	shadowHeight = 4096;

	// data
	scene = newModelScene();
	vboa = newVertexBufferObjectArray();


	// effect
	enableShadowMapping = false;
	enableSSAO = false;

	//  set opengl version and profile
	setupOpenGL();

	// set context menu
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(lightWidget, SIGNAL(dataUpdate(void)), this, SLOT(update(void)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(toggleContextMenu(const QPoint&)));
	connect(textureWidget, SIGNAL(sendTexture(QImage*)), this, SLOT(updateProjectionTexture(QImage*)));
	//connect(this, SIGNAL(selectModel(MeshModelItem*)), managerWidget, SLOT(modelSelected(MeshModelItem*)));
	//connect(voxelizeWidget, SIGNAL(voxelizeInfo(float, float, float, bool)), this, SLOT(confirmVoxelize(float, float, float, bool)));
}

ModelViewerOGLWidget::~ModelViewerOGLWidget() 
{
	
}

void ModelViewerOGLWidget::genSSAO(void)
{
	// Set samplers
	glUseProgram(program[7]);
	glUniform1i(glGetUniformLocation(program[7], "gPositionDepth"), 0);
	glUniform1i(glGetUniformLocation(program[7], "gNormal"), 1);
	glUniform1i(glGetUniformLocation(program[7], "gAlbedo"), 2);
	glUniform1i(glGetUniformLocation(program[7], "ssao"), 3);
	glUseProgram(program[5]);
	glUniform1i(glGetUniformLocation(program[5], "gPositionDepth"), 0);
	glUniform1i(glGetUniformLocation(program[5], "gNormal"), 1);
	glUniform1i(glGetUniformLocation(program[5], "texNoise"), 2);

	// Set up G-Buffer
	// 3 textures:
	// 1. Positions + depth (RGBA)
	// 2. Color (RGB) 
	// 3. Normals (RGB) 
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// - Position + linear depth color buffer
	glGenTextures(1, &gPositionDepth);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionDepth, 0);
	// - Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// - Albedo color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// - Create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "GBuffer Framebuffer not complete!\\n");
	}

	// Also create framebuffer to hold SSAO processing stage 
	glGenFramebuffers(1, &ssaoFBO);
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// - SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "SSAO Framebuffer not complete!");
	}
	// - and blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "SSAO Blur Framebuffer not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());

	int i;
	float factor, scale, sample[3];
	// Sample kernel

	for (i = 0; i < kernelSize; ++i)
	{
		sample[0] = randomF(2.0) - 1;
		sample[1] = randomF(2.0) - 1;
		sample[2] = randomF(1.0);
		normalize3fv(sample);
		factor = randomF(1.0);
		mult3x1fv(sample, factor, sample);
		scale = i / (float)kernelSize;

		// Scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		mult3x1fv(sample, scale, sample);
		ssaoKernel[i * 3] = sample[0];
		ssaoKernel[i * 3 + 1] = sample[1];
		ssaoKernel[i * 3 + 2] = sample[2];
	}

	// Noise texture
	for (i = 0; i < 16; i++)
	{
		ssaoNoise[i * 3] = randomF(2.0) - 1.0;
		ssaoNoise[i * 3 + 1] = randomF(2.0) - 1.0;
		ssaoNoise[i * 3 + 2] = 0.0;
	}

	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, noiseSize, noiseSize, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void ModelViewerOGLWidget::genShadowMapping(void)
{
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("FBO error\\n");
	}
}

void ModelViewerOGLWidget::genMultiSampling(void)
{
	glGenTextures(1, &multiSampleTex);
	glGenFramebuffers(1, &multiSampleFBO);
	glGenRenderbuffers(1, &multiSampleColorBuffer);
	glGenRenderbuffers(1, &multiSampleDepthBuffer);

	glEnable(GL_MULTISAMPLE);
}

void ModelViewerOGLWidget::setupOpenGL(void)
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();

	format.setDepthBufferSize(64);
	format.setStencilBufferSize(8);
	format.setVersion(4, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	setFocusPolicy(Qt::StrongFocus);
}

void ModelViewerOGLWidget::initializeGL(void)
{
	float mat[16];

	//  initialize glew
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	printf("%s\n", glGetString(GL_VERSION));
	printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

	//  load shaders

	// phong shading model (object) render
	ShaderInfo modelShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\model.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\model.frag" },
		{ GL_NONE, NULL }
	};
	program[0] = LoadShaders(modelShaders);

	// shadow mapping for model render
	ShaderInfo shadowShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\shadow.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\shadow.frag" },
		{ GL_NONE, NULL }
	};
	program[1] = LoadShaders(shadowShaders);

	ShaderInfo pickingShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\picking.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\picking.frag" },
		{ GL_NONE, NULL }
	};
	program[2] = LoadShaders(pickingShaders);

	ShaderInfo simpleShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\simple.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\simple.frag" },
		{ GL_NONE, NULL }
	};
	program[3] = LoadShaders(simpleShaders);

	ShaderInfo deferredShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\ssao_geometry.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\ssao_geometry.frag" },
		{ GL_NONE, NULL }
	};
	program[4] = LoadShaders(deferredShaders);

	ShaderInfo ssaoShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\ssao.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\ssao.frag" },
		{ GL_NONE, NULL }
	};
	program[5] = LoadShaders(ssaoShaders);

	ShaderInfo ssaoBlurShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\ssao_blur.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\ssao_blur.frag" },
		{ GL_NONE, NULL }
	};
	program[6] = LoadShaders(ssaoBlurShaders);

	ShaderInfo ssaoLightPassShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\lightpass.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\lightpass.frag" },
		{ GL_NONE, NULL }
	};
	program[7] = LoadShaders(ssaoLightPassShaders);

	// use shader program
	glUseProgram(program[0]);

	// viewing varibles
	setViewingMatrix();

	// light varibles
	setLighting();

	// color varibles
	colorLoc = glGetUniformLocation(program[0], "color");

	// flag varible
	wireframe = glGetUniformLocation(program[0], "wireframe");
	glUniform1i(wireframe, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	// check assignment error
	//CheckGLErrors();

	// create gound plane
	ground = newPlane(200, 200, 1);
	addVertexBufferObject(ground, vboa);
	setMaterialAmbient(ground->material, .19225, .19225, .19225, 1.0);
	setMaterialDiffuse(ground->material, .50754, .50754, .50754, 1.0);
	setMaterialSpecular(ground->material, .508273, .508273, .508273, 1.0);
	setMaterialShininess(ground->material, .4 * 128.0);
	rotateX(90.0, mat);
	translate(-100, -100, 0, ground->modelMat);
	multMat4fv(mat, ground->modelMat, ground->modelMat);
	bindData(ground);

	
	// create project cylinder
	//projCylinder = newCylinder(10.0, 0.0, 30.0, 360, 100);

	// create axes
	// x-axis
	axisX = newAxis(10, 2, 4, 4);
	setColorVBO(1.0, 0.0, 0.0, 1.0, axisX);
	rotateZ(-90, mat);
	multMat4fv(mat, axisX->modelMat, axisX->modelMat);
	translateVBO(axisX, true);
	registerID(axisX, 0);
	bindData(axisX);

	// y-axis
	axisY = newAxis(10, 2, 4, 4);
	setColorVBO(0.0, 1.0, 0.0, 1.0, axisY);
	registerID(axisY, 1);
	bindData(axisY);

	// z-axis
	axisZ = newAxis(10, 2, 4, 4);
	setColorVBO(0.0, 0.0, 1.0, 1.0, axisZ);
	rotateX(-90, mat);
	multMat4fv(mat, axisZ->modelMat, axisZ->modelMat);
	translateVBO(axisZ, true);
	registerID(axisZ, 2);
	bindData(axisZ);

	// create rotate circles
	rotCircleX = newCircle(1, 360, 1);
	setColor(1.0, 0.0, 0.0, 1.0, rotCircleX);
	rotateY(90, mat);
	multMat4fv(mat, rotCircleX->modelMat, rotCircleX->modelMat);
	translateVBO(rotCircleX, true);
	registerID(rotCircleX, 0);
	bindData(rotCircleX);

	// create rotate circles
	rotCircleY = newCircle(1, 360, 1);
	setColor(0.0, 1.0, 0.0, 1.0, rotCircleY);
	rotateX(90, mat);
	multMat4fv(mat, rotCircleY->modelMat, rotCircleY->modelMat);
	translateVBO(rotCircleY, true);
	registerID(rotCircleY, 1);
	bindData(rotCircleY);

	// create rotate circles
	rotCircleZ = newCircle(1, 360, 1);
	setColor(0.0, 0.0, 1.0, 1.0, rotCircleZ);
	registerID(rotCircleZ, 2);
	bindData(rotCircleZ);

	// create scale axes
	scaleAxisX = newHammer(10, 2, 6, 6, 6);
	setColorVBO(1.0, 0.0, 0.0, 1.0, scaleAxisX);
	rotateZ(-90, mat);
	multMat4fv(mat, scaleAxisX->modelMat, scaleAxisX->modelMat);
	translateVBO(scaleAxisX, true);
	registerID(scaleAxisX, 0);
	bindData(scaleAxisX);

	// create scale axes
	scaleAxisY = newHammer(10, 2, 6, 6, 6);
	setColorVBO(0.0, 1.0, 0.0, 1.0, scaleAxisY);
	registerID(scaleAxisY, 1);
	bindData(scaleAxisY);

	// create scale axes
	scaleAxisZ = newHammer(10, 2, 6, 6, 6);
	setColorVBO(0.0, 0.0, 1.0, 1.0, scaleAxisZ);
	rotateX(-90, mat);
	multMat4fv(mat, scaleAxisZ->modelMat, scaleAxisZ->modelMat);
	translateVBO(scaleAxisZ, true);
	registerID(scaleAxisZ, 2);
	bindData(scaleAxisZ);
	



	// ssao
	genSSAO();
	glUseProgram(program[0]);
	glUniform1i(12, 1);

	// FBO depth map
	genShadowMapping();
	glUseProgram(program[0]);
	glUniform1i(11, 1);


	// multiple sample FBO
	genMultiSampling();

	// enable clip plane
	glEnable(GL_CLIP_DISTANCE0);

	// test texture
	int width, height;
	unsigned char* data = NULL;

	//loadBMP("C:\\Users\\LuKang\\Desktop\\lab503slicer\\Textures\\test_low.bmp", &width, &height, &data);

	glGenTextures(1, &testTexID);

	glBindTexture(GL_TEXTURE_2D, testTexID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	activeTextureMapping = true;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// init parameters
	//managerWidget->move(this->width() - managerWidget->width(), 0);
	oldWidth = this->width();
	oldHeight = this->height();
}

void ModelViewerOGLWidget::configureShadowMapping(void)
{
	float nearPlane = 1.0, farPlane = 500.0, mat[16], view[16], proj[16];
	float lit[] = { lights[0]->position[0], lights[0]->position[1], lights[0]->position[2] };
	normalize3fv(lit);
	float lightPos[] = { lit[0] * 200, lit[1] * 200, lit[2] * 200 };
	float lightLok[] = { 0, 0, 0 };
	float lightVup[] = { 0, 1, 0 };

	ortho(-300.0, 300.0, -300.0, 300.0, nearPlane, farPlane, proj);
	lookAt(lightPos, lightLok, lightVup, view);
	multMat4fv(proj, view, lightSpaceMat);
	transposeMat4fv(lightSpaceMat, mat);
	glUniformMatrix4fv(6, 1, GL_FALSE, mat);
}

void ModelViewerOGLWidget::configureGBufferShader(void)
{

}

void ModelViewerOGLWidget::renderScene(void)
{
	MeshModelItem* item;

	// draw static scene
	drawStaticScene();

	// reserve 1% of the front depth range for the 3D axis
	glDepthRange(0, 0.01);
	if (opMode == TRANSLATE_MODE) {
		drawAxes(RENDER_POLYGON);
	}
	if (opMode == ROTATE_MODE) {
		drawRotCircle(RENDER_WIREFRAME);
	}
	if (opMode == SCALE_MODE) {
		drawScaleAxes(RENDER_POLYGON);
	}

	// reserve 99% of the back depth range for the 3D axis
	glDepthRange(0.01, 1.0);
	// draw models
	item = firstModelItem(scene->items);
	while (!isAnchorModelItem(item))
	{
		drawData(item->model->vbo, RENDER_POLYGON);
		item = nextModelItem(item);
	}

	if (projCylinder != NULL) {
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniform1i(10, activeTextureMapping);	// active texture
		//glUniform1i(glGetUniformLocation(program[0], "textureMap"), 2);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, testTexID);
		drawData(projCylinder, RENDER_POLYGON);
		//glUniform1i(10, 0); // disable texute
		//glDisable(GL_BLEND);
	}

	// restore depth range
	glDepthRange(0, 1.0);
}

void ModelViewerOGLWidget::renderBbox(void)
{
	MeshModelItem* item;

	item = nextModelItem(scene->items);
	while (!isAnchorModelItem(item))
	{
		if (item->model->bbox->vbo->bboxDetect) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			drawBbox(item->model->bbox->vbo, RENDER_WIREFRAME);
			glDisable(GL_BLEND);
		}
		item = nextModelItem(item);
	}
}

void ModelViewerOGLWidget::setViewingMatrix(void)
{
	float mat[16], tmp[16], aspect;

	// initialize variables and assign them to shader program
	// viewing matrices
	// z-axis up

	identifyMat4fv(modelMat);						// model matrix
	transposeMat4fv(modelMat, mat);
	glUniformMatrix4fv(0, 1, false, mat);
	lookAt(eye, lok, vup, viewMat);					// view matrix
	transposeMat4fv(viewMat, mat);
	glUniformMatrix4fv(1, 1, false, mat);
	multMat4fv(viewMat, modelMat, mvMat);			// modelview matrix
	transposeMat4fv(mvMat, mat);
	glUniformMatrix4fv(3, 1, false, mat);
	normalM(mvMat, normalMat);						// normal matrix
	transposeMat4fv(normalMat, mat);
	glUniformMatrix4fv(5, 1, false, mat);
	aspect = this->width() / (float)this->height();	// projection matrix
	perspective(fovy, aspect, nearClip, farClip, projectionMat);
	transposeMat4fv(projectionMat, mat);
	glUniformMatrix4fv(2, 1, false, mat);
	multMat4fv(projectionMat, viewMat, mvpMat);
	multMat4fv(mvpMat, modelMat, mvpMat);
	transposeMat4fv(mvpMat, mat);
	glUniformMatrix4fv(4, 1, false, mat);
	glViewport(0, 0, this->width(), this->height());
}

void ModelViewerOGLWidget::setLighting(void)
{
	GLuint loc;

	glUniform4fv(7, 1, lights[0]->position);
	loc = glGetUniformLocation(program[0], "lightAmbient0");
	glUniform4fv(loc, 1, globalLight->ambient);
	loc = glGetUniformLocation(program[0], "lightDiffuse0");
	glUniform4fv(loc, 1, lights[0]->diffuse);
	loc = glGetUniformLocation(program[0], "lightSpecular0");
	glUniform4fv(loc, 1, lights[0]->specular);
}

void ModelViewerOGLWidget::setClipping(void)
{
	glUniform4fv(8, 1, clip);
}

void ModelViewerOGLWidget::setMaterial(VBO obj)
{
	GLuint loc;

	// material varibles
	loc = glGetUniformLocation(program[0], "materialAmbient");
	glUniform4fv(loc, 1, obj.material->ambient);
	loc = glGetUniformLocation(program[0], "materialDiffuse");
	glUniform4fv(loc, 1, obj.material->diffuse);
	loc = glGetUniformLocation(program[0], "materialSpecular");
	glUniform4fv(loc, 1, obj.material->specular);
	loc = glGetUniformLocation(program[0], "shininess");
	glUniform1f(loc, obj.material->shininess);
	loc = glGetUniformLocation(program[0], "enableMaterial");
	glUniform1i(loc, obj.material->isEnable);
}

void ModelViewerOGLWidget::fixView(void)
{
	float temp;

	if (viewTheta < 0.01) {
		viewTheta = 0.0 + 0.001;
	}
	else if (viewTheta > M_PI) {
		viewTheta = M_PI - 0.001;
	}
	else {
		vup[0] = 0;
		vup[1] = 1;
		vup[2] = 0;
	}

	eye[0] = lok[0] + viewRadius * cos(viewPhi) * sin(viewTheta);
	eye[1] = lok[1] + viewRadius * cos(viewTheta);
	eye[2] = lok[2] + viewRadius * sin(viewPhi) * sin(viewTheta);

	lookAt(eye, lok, vup, viewMat);
}


void ModelViewerOGLWidget::updateViewing(void)
{
	float mat[16];

	multMat4fv(viewMat, modelMat, mvMat);
	normalM(mvMat, normalMat);
	multMat4fv(projectionMat, mvMat, mvpMat);

	transposeMat4fv(modelMat, mat);
	glUniformMatrix4fv(0, 1, false, mat);
	transposeMat4fv(viewMat, mat);
	glUniformMatrix4fv(1, 1, false, mat);
	transposeMat4fv(normalMat, mat);
	glUniformMatrix4fv(5, 1, false, mat);
	transposeMat4fv(mvMat, mat);
	glUniformMatrix4fv(3, 1, false, mat);
	transposeMat4fv(mvpMat, mat);
	glUniformMatrix4fv(4, 1, false, mat);
}

void ModelViewerOGLWidget::bindData(VBO *vbo)
{
	int i;

	if (!vbo->isGenBuffer) {
		glGenBuffers(NUM_ATTRIBS, vbo->buffers);
		//glGenBuffers(NUM_ATTRIBS, vbo->bbox->buffers);
		vbo->isGenBuffer = true;
		//printf("gen: %d\\n", CheckGLErrors());
	}
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if (vbo->enableBuffers[i]) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[i]);
			//printf("bindBuffer: %d\\n", CheckGLErrors());
			glBufferData(GL_ARRAY_BUFFER, vbo->numVertices * vbo->dataSize[i] * sizeof(float), vbo->attribs[i], GL_STATIC_DRAW);
			//printf("bufferData: %d\\n", CheckGLErrors());
		}
		/*
		if (vbo->bbox->enableBuffers[i]) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo->bbox->buffers[i]);
			if (i == WIREFRAME) {
				glBufferData(GL_ARRAY_BUFFER, vbo->bbox->numWFVertices * vbo->bbox->dataSize[i] * sizeof(float), vbo->bbox->attribs[i], GL_STATIC_DRAW);
			}
			else {
				glBufferData(GL_ARRAY_BUFFER, vbo->bbox->numVertices * vbo->bbox->dataSize[i] * sizeof(float), vbo->bbox->attribs[i], GL_STATIC_DRAW);
			}
		}
		*/
	}
}

void ModelViewerOGLWidget::drawStaticScene()
{
	// draw ground
	drawData(ground, RENDER_POLYGON);
}

void ModelViewerOGLWidget::drawAxes(int mode)
{
	float center[3], translate[3], mat[16];
	AABB* bbox;

	if (selectedItem == NULL) return;

	bbox = selectedItem->model->bbox;
	aabbCenter(*bbox, center);

	tsfmTranslate(modifyingTransformation, translate);
	
	tsfmSetTranslate(center, axesTsfm);
	tsfmAddTranslate(translate, axesTsfm);

	transformMatrix(axesTsfm, axisX->modelMat, NULL);
	transformMatrix(axesTsfm, axisY->modelMat, NULL);
	transformMatrix(axesTsfm, axisZ->modelMat, NULL);

	drawData(axisX, mode);
	drawData(axisY, mode);
	drawData(axisZ, mode);
}

void ModelViewerOGLWidget::drawRotCircle(int mode)
{
	float center[3], translate[3], size[3], mat[16];
	AABB* bbox;

	if (selectedItem == NULL) return;

	bbox = selectedItem->model->bbox;
	aabbCenter(*bbox, center);
	size[0] = size[1] = size[2] = aabbMaxSize(*bbox) / 2.0;

	tsfmTranslate(modifyingTransformation, translate);

	tsfmSetTranslate(center, rotCircleTsfm);
	tsfmSetScale(size, rotCircleTsfm);
	tsfmAddTranslate(translate, rotCircleTsfm);

	transformMatrix(rotCircleTsfm, rotCircleX->modelMat, NULL);
	transformMatrix(rotCircleTsfm, rotCircleY->modelMat, NULL);
	transformMatrix(rotCircleTsfm, rotCircleZ->modelMat, NULL);

	drawData(rotCircleX, mode);
	drawData(rotCircleY, mode);
	drawData(rotCircleZ, mode);
}

void ModelViewerOGLWidget::drawScaleAxes(int mode)
{
	float center[3], translate[3], mat[16];
	AABB* bbox;

	if (selectedItem == NULL) return;

	bbox = selectedItem->model->bbox;
	aabbCenter(*bbox, center);

	tsfmTranslate(modifyingTransformation, translate);

	tsfmSetTranslate(center, scaleAxesTsfm);
	tsfmAddTranslate(translate, scaleAxesTsfm);

	transformMatrix(scaleAxesTsfm, scaleAxisX->modelMat, NULL);
	transformMatrix(scaleAxesTsfm, scaleAxisY->modelMat, NULL);
	transformMatrix(scaleAxesTsfm, scaleAxisZ->modelMat, NULL);

	drawData(scaleAxisX, mode);
	drawData(scaleAxisY, mode);
	drawData(scaleAxisZ, mode);
}

void ModelViewerOGLWidget::drawData(VBO *const vbo, int mode)
{
	int i;
	float center[3], rotMat[16];

	copyMat4fv(vbo->modelMat, modelMat);
	if (isMousePressed && selectedItem != NULL) {
		MeshModel* model = selectedItem->model;
		if (vbo == model->vbo) {
			aabbCenter(*model->bbox, center);
			transformMatrix(modifyingTransformation, modelMat, center);
			multMat4fv(modelMat, vbo->modelMat, modelMat);
		}
	}

	if (mode == RENDER_PICKING) {
		if (vbo->enableBuffers[FACE]) {
			glUseProgram(program[2]);
			updateViewing();
			setLighting();
			setClipping();
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[VERTEX]);
			glVertexAttribPointer(VERTEX, vbo->dataSize[VERTEX], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERTEX);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[FACE]);
			glVertexAttribPointer(FACE, vbo->dataSize[FACE], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(FACE);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		}
	}
	else if (mode == RENDER_WIREFRAME) {
		glUseProgram(program[3]);
		updateViewing();
		setLighting();
		setClipping();
		glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[WIREFRAME]);
		glVertexAttribPointer(VERTEX, vbo->dataSize[WIREFRAME], GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(VERTEX);
		glLineWidth(3);
		glUniform4f(11, vbo->color[0], vbo->color[1], vbo->color[2], vbo->color[3]);
		glDrawArrays(GL_LINES, 0, vbo->numWFVertices);
		/*
		glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[VERTEX]);
		glVertexAttribPointer(VERTEX, vbo->dataSize[VERTEX], GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(VERTEX);
		glUniform4f(0, 0.0, 0.0, 0.0, 1.0);
		glLineWidth(2.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		*/
	}
	else {
		glUseProgram(program[0]);
		updateViewing();
		setLighting();
		setClipping();
		setMaterial(*vbo);
		float screenSize[2];
		screenSize[0] = this->width();
		screenSize[1] = this->height();
		glUniform2fv(glGetUniformLocation(program[0], "screenSize"), 1, screenSize);
		for (i = 0; i < NUM_ATTRIBS; ++i)
		{
			if (vbo->enableBuffers[i]) {
				glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[i]);
				glVertexAttribPointer(i, vbo->dataSize[i], GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(i);
				//printf("bind: %d\\n", CheckGLErrors());
			}
		}
		if (mode == RENDER_POLYGON) {
			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_FRONT);
			//glPolygonMode(GL_BACK, GL_LINE);
			//glLineWidth(2.0);
			//glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
			//glLineWidth(1.0);
			//glPolygonMode(GL_BACK, GL_FILL);
			//glDisable(GL_CULL_FACE);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		}
	}
	//printf("draw: %d\\n", CheckGLErrors());
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if (vbo->enableBuffers[i]) {
			glDisableVertexAttribArray(i);
		}
	}
}

void ModelViewerOGLWidget::drawBbox(VBO *const vbo, int mode)
{
	int i;
	float rotMat[16], color[4];

	copyMat4fv(vbo->modelMat, modelMat);
	
	// render bounding box
	if (mode == RENDER_POLYGON) {
		if (vbo->enableBuffers[VERTEX]) {
			glUseProgram(program[3]);
			updateViewing();
			setLighting();
			setClipping();
			setMaterial(*vbo);
			for (i = 0; i < 4; ++i)
			{
				if (vbo->enableBuffers[i]) {
					glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[i]);
					glVertexAttribPointer(i, vbo->dataSize[i], GL_FLOAT, GL_FALSE, 0, NULL);
					glEnableVertexAttribArray(i);
				}
			}
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
			glDisable(GL_BLEND);
		}
	}
	// line loop each face
	if (mode == RENDER_WIREFRAME) {
		if (vbo->enableBuffers[WIREFRAME]) {
			glUseProgram(program[3]);
			updateViewing();
			setLighting();
			setClipping();
			color[0] = 0.0;
			color[1] = 1.0;
			color[2] = 0.0;
			color[3] = 1.0;
			glUniform4fv(11, 1, color);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[WIREFRAME]);
			glVertexAttribPointer(VERTEX, vbo->dataSize[WIREFRAME], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERTEX);
			//printf("bind: %d\\n", CheckGLErrors());
			for (i = 0; i < vbo->numWFVertices; i += vbo->numWFLoop)
			{
				glDrawArrays(GL_LINE_LOOP, i, vbo->numWFLoop);
			}
		}
	}
	if (mode == RENDER_PICKING) {
		if (vbo->enableBuffers[FACE]) {
			glUseProgram(program[2]);
			updateViewing();
			setLighting();
			setClipping();
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[VERTEX]);
			glVertexAttribPointer(VERTEX, vbo->dataSize[VERTEX], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERTEX);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[FACE]);
			glVertexAttribPointer(FACE, vbo->dataSize[FACE], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(FACE);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		}
	}
	// close Attirbs
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if (vbo->enableBuffers[i]) {
			glDisableVertexAttribArray(i);
		}
	}
	
}

void ModelViewerOGLWidget::gBufferRendering(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program[4]);
	renderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void ModelViewerOGLWidget::ssaoTextureRendering(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program[5]);
	float screenSize[2];
	screenSize[0] = this->width();
	screenSize[1] = this->height();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glUniform1i(glGetUniformLocation(program[5], "noiseSize"), noiseSize);
	glUniform1i(glGetUniformLocation(program[5], "kernelSize"), kernelSize);
	glUniform1f(glGetUniformLocation(program[5], "radius"), kernelRadius);
	glUniform2fv(glGetUniformLocation(program[5], "screenSize"), 1, screenSize);
	// Send kernel + rotation 
	char str[100] = { '\\0' }, idx[10];
	for (GLuint i = 0; i < kernelSize; ++i)
	{
		strcpy(str, "samples[");
		sprintf(idx, "%d", i);
		strcat(str, idx);
		strcat(str, "]");
		glUniform3fv(glGetUniformLocation(program[5], str), 1, ssaoKernel + i * 3);
	}
	setViewingMatrix();
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void ModelViewerOGLWidget::ssaoBlurRendering(void)
{
	// 3. Blur SSAO texture to remove noise
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program[6]);
	glUniform1i(glGetUniformLocation(program[6], "noiseSize"), noiseSize);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void ModelViewerOGLWidget::ssaoLightPassRendering(void)
{
	// 4. Lighting Pass: traditional deferred Blinn-Phong lighting now with added screen-space ambient occlusion
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program[7]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glActiveTexture(GL_TEXTURE3); // Add extra SSAO texture to lighting pass
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	// Also send light relevant uniforms
	// Lights
	float lightPos[3] = { 2.0, 4.0, -2.0 };
	float lightColor[3] = { 0.7, 0.7, 0.7 };
	glUniform3fv(glGetUniformLocation(program[7], "light.Position"), 1, lightPos);
	glUniform3fv(glGetUniformLocation(program[7], "light.Color"), 1, lightColor);
	// Update attenuation parameters
	const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
	const GLfloat linear = 0.09;
	const GLfloat quadratic = 0.032;
	glUniform1f(glGetUniformLocation(program[7], "light.Linear"), linear);
	glUniform1f(glGetUniformLocation(program[7], "light.Quadratic"), quadratic);
	renderQuad();
}

void ModelViewerOGLWidget::shadowMappingRendering(void)
{
	// 1. first render to depth map
	glUseProgram(program[1]);
	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	configureShadowMapping();
	renderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());

	// 2. then render scene as normal with shadow mapping (using depth map)
	glUseProgram(program[0]);
	
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, this->width(), this->height(), GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, multiSampleFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, multiSampleColorBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, multiSampleColorBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, multiSampleDepthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, multiSampleDepthBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multiSampleDepthBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex, 0);
	
}

void ModelViewerOGLWidget::cylinderProjection()
{
	/*
	if (selectedModelObjID < 0 || selectedModelID < 0) return;
	
	static int objID = -1, modelID = -1;
	MeshModelObject* obj = moa->objects[selectedModelObjID];
	MeshModel* model = obj->models[selectedModelID];
	float diagonal[3], radius;

	if (objID != selectedModelObjID || modelID != selectedModelID) {
		subtract3fv(model->bbox->max, model->bbox->min, diagonal);
		radius = length3fv(diagonal) / 2.0;
		deleteVBO(&projCylinder);
		projCylinder = newCylinder(radius, radius, diagonal[1], 360, 100);
		setColorVBO(1.0, 1.0, 1.0, 0.1, projCylinder);
		bindData(projCylinder);
		objID = selectedModelObjID;
		modelID = selectedModelID;
	}
	copyTransformation(model->transformation, projCylinder->transformation);
	transformMatrix(model->transformation, projCylinder->modelMat, NULL);
	*/
}

void ModelViewerOGLWidget::paintGL(void)
{
	float mat[16], color[4];

	
	glUniform1i(glGetUniformLocation(program[0], "enableShadowMapping"), enableShadowMapping);
	glUniform1i(glGetUniformLocation(program[0], "enableSSAO"), enableSSAO);

	// ssao
	if (enableSSAO) {
		gBufferRendering();
		ssaoTextureRendering();
		ssaoBlurRendering();
	}
	//ssaoLightPassRendering();
	
	// render shadow map
	
	if (enableShadowMapping) {
		shadowMappingRendering();
	}
	
	glUseProgram(program[0]);
	glViewport(0, 0, this->width(), this->height());
	glClearColor(0.66, 0.66, 0.66, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (enableShadowMapping) {
		glUniform1i(glGetUniformLocation(program[0], "shadowMap"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		transposeMat4fv(lightSpaceMat, mat);
		glUniformMatrix4fv(6, 1, GL_FALSE, mat);
	}
	if (enableSSAO) {
		glUniform1i(glGetUniformLocation(program[0], "ssaoMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	}
	renderScene();
	renderBbox();
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, multiSampleFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->defaultFramebufferObject());
	glDrawBuffer(GL_BACK);
	glBlitFramebuffer(0, 0, this->width(), this->height(), 0, 0, this->width(), this->height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
}

void ModelViewerOGLWidget::resizeSSAO(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// - Position + linear depth color buffer
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// - Normal color buffer
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// - Albedo color buffer
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// - Depth buffer (renderbuffer)
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// - SSAO color buffer
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// - and blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void ModelViewerOGLWidget::resizeGL(int width, int height)
{
	float mat[16];

	glViewport(0, 0, width, height);
	float aspect = width / (float)height;
	perspective(fovy, aspect, nearClip, farClip, projectionMat);
	transposeMat4fv(projectionMat, mat);
	glUniformMatrix4fv(2, 1, false, mat);
		
	/*
	// fix widget position
	QSize size = voxelizeButton->size();
	float ratio = fmin((width + 211) / (float)g_originWidth, (height + 78) / (float)g_originHeight);
	int scaledSize = iconSize * ratio;
	QPixmap pixmap(":/mainwindow/icons/voxelize.png");
	QPixmap scaled = pixmap.scaled(scaledSize, scaledSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
	QIcon ButtonIcon(scaled);
	voxelizeButton->setIcon(ButtonIcon);
	voxelizeButton->setIconSize(scaled.rect().size());
	voxelizeButton->setFixedSize(scaled.rect().size());

	voxelizeButton->move(QPoint(width - scaledSize - 10, height - scaledSize - 10));
	*/

	// reset widget position
	//managerWidget->move(width * managerWidget->pos().x() / (float)oldWidth,
	//					height * managerWidget->pos().y() / (float)oldHeight);
	

	resizeSSAO();

	oldWidth = width;
	oldHeight = height;

	update();
}

void ModelViewerOGLWidget::mousePressEvent(QMouseEvent *e)
{
	int i, j, ret;
	unsigned int objID;
	float rayDir[3], boxCenter[3], param[4], dir[3], w[3];
	unsigned char pickedColor[4];
	MeshModelItem* item;
	MeshModel* model;

	isMousePressed = true;
	switch (e->buttons())
	{
	case Qt::LeftButton:
		this->grabFramebuffer();

		// if a model be selected, then...
		if (selectedItem != NULL) {
			
			if (opMode == TRANSLATE_MODE) {
				// select axes by off-screen picking
				glClearColor(1.0, 1.0, 1.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawAxes(RENDER_PICKING);
				glReadPixels(e->x(), this->height() - e->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickedColor);
				toObjectID(pickedColor, &objID);
				selectedAxis = objID;
				if (selectedAxis != EMPTY_ID) {	// if selected, then cancel the actions bellow
					break;
				}
			}
			else if (opMode == SCALE_MODE) {
				// select scale axes by off-screen picking
				glClearColor(1.0, 1.0, 1.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawScaleAxes(RENDER_PICKING);
				glReadPixels(e->x(), this->height() - e->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickedColor);
				toObjectID(pickedColor, &objID);
				selectedScaleAxis = objID;
				if (selectedScaleAxis != EMPTY_ID) {	// if selected, then cancel the actions bellow
					break;
				}
			}
			else if (opMode == ROTATE_MODE) {
				// select rotate circles by off-screen picking
				glClearColor(1.0, 1.0, 1.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawRotCircle(RENDER_PICKING);
				glReadPixels(e->x(), this->height() - e->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickedColor);
				toObjectID(pickedColor, &objID);
				selectedRotCircle = objID;
				if (selectedRotCircle != EMPTY_ID) {	// if selected, then cancel the actions bellow
					model = selectedItem->model;
					aabbCenter(*model->bbox, boxCenter);
					// mouse position
					if (selectedRotCircle == 0) {
						w[0] = 1;	w[1] = 0;	w[2] = 0;
					}
					else if (selectedRotCircle == 1) {
						w[0] = 0;	w[1] = 1;	w[2] = 0;
					}
					else if (selectedRotCircle == 2) {
						w[0] = 0;	w[1] = 0;	w[2] = -1;
					}
					compPlane(w, boxCenter, param);
					getRayDir(e->x(), e->y(), this->width(), this->height(), viewMat, projectionMat, dir);
					compPntOnPlane(param, eye, dir, selectedCirclePos);
					break;
				}
			}
		}

		// select models by off-screen picking
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		item = firstModelItem(scene->items);
		while (!isAnchorModelItem(item))
		{
			model = item->model;
			drawData(model->vbo, RENDER_PICKING);
			item = nextModelItem(item);
		}
		glReadPixels(e->x(), this->height() - e->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickedColor);
		toObjectID(pickedColor, &objID);
		if (objID != EMPTY_ID) {
			//managerWidget->show();
			item = searchModel(objID, scene);
			item->model->bbox->vbo->bboxDetect = true;
			selectedItem = item;
			aabbCenter(*item->model->bbox, selectedModelPos);
			emit selectModel(item);
			emit sendModelObject(item->object);
		}
		else {
			selectedItem = NULL;
		}
		/*
		this->grabFramebuffer();
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (i = 0; i < moa->numObjects; ++i)
		{
			for (j = 0; j < moa->objects[i]->numModels; ++j)
			{
				drawData(moa->objects[i]->models[j]->bbox->vbo, RENDER_PICKING);
			}
		}
		glReadPixels(e->x(), this->height() - e->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickedColor);
		toObjectID(pickedColor, &objID);
		if (objID >= moa->numObjects) {
			selectedModelID = -1;
		}
		else {
			selectedModelID = objID;
			selectedModelObjID = 0;
			managerWidget->show();
			//cylinderProjection();
			emit modelSelected(selectedModelObjID, selectedModelBvhID, selectedModelID);
		}
		*/
		break;
	case Qt::MidButton:
		break;
	case Qt::RightButton:
		break;
	default:
		break;
	}
	oldPosX = e->x();
	oldPosY = e->y();
	update();
}

void ModelViewerOGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	float u[3] = { viewMat[0], viewMat[1], viewMat[2] };
	float v[3] = { viewMat[4], viewMat[5], viewMat[6] };
	float w[3] = { viewMat[8], viewMat[9], viewMat[10] };
	float rotVec1[3], rotVec2[3], rotW[3], rotCross[3], transMat[16], boxCenter[3], boxSize[3], param[4], projPnt[3], dir[3], tmp;
	float nWidth, nHeight, mOffX, mOffY;
	float translation[3] = { 0.0, 0.0, 0.0 };
	float rotation[3] = { 0.0, 0.0, 0.0 };
	float scale[3] = { 1.0, 1.0, 1.0 };
	int offsetX = (e->x() - oldPosX);
	int offsetY = -(e->y() - oldPosY);
	bool isColckwise;
	MeshModelObject* obj;
	MeshModel* model;

	switch (e->buttons())
	{
	case Qt::LeftButton:
		if (selectedItem != NULL) {
			model = selectedItem->model;
			aabbCenter(*model->bbox, selectedModelPos);
			// if axis selected, then move the object along a spcific direction
			if (opMode == ROTATE_MODE && selectedRotCircle != EMPTY_ID) {
				if (selectedRotCircle == 0) {
					rotW[0] = 1; rotW[1] = 0; rotW[2] = 0;
				}
				else if (selectedRotCircle == 1) {
					rotW[0] = 0; rotW[1] = 1; rotW[2] = 0;
				}
				else if (selectedRotCircle == 2) {
					rotW[0] = 0; rotW[1] = 0; rotW[2] = -1;
				}
				compPlane(rotW, selectedModelPos, param);
				getRayDir(e->x(), e->y(), this->width(), this->height(), viewMat, projectionMat, dir);
				compPntOnPlane(param, eye, dir, projPnt);
				subtract3fv(selectedCirclePos, selectedModelPos, rotVec1);
				subtract3fv(projPnt, selectedModelPos, rotVec2);
				cross3fv(rotVec1, rotVec2, rotCross);
				normalize3fv(rotCross);
				if (equal3fv(rotCross, rotW, 0.0001)) {
					rotation[selectedRotCircle] = -degrees(acos(dot3fv(rotVec1, rotVec2) / (length3fv(rotVec1) * length3fv(rotVec2))));
				}
				else {
					rotation[selectedRotCircle] = degrees(acos(dot3fv(rotVec1, rotVec2) / (length3fv(rotVec1) * length3fv(rotVec2))));
				}
				if (rotation[selectedRotCircle] > 360 || rotation[selectedRotCircle] < -360) {
					rotation[selectedRotCircle] = 0;
				}
			}
			else if (opMode == TRANSLATE_MODE) {
				// calculate scales of translation
				//aabbCenter(*model->bbox, boxCenter);
				compPlane(w, selectedModelPos, param);
				getRayDir(e->x(), e->y(), this->width(), this->height(), viewMat, projectionMat, dir);
				compPntOnPlane(param, eye, dir, projPnt);
				subtract3fv(projPnt, selectedModelPos, translation);
				if (selectedAxis == 0) {
					translation[1] = translation[2] = 0.0;
				}
				else if (selectedAxis == 1) {
					translation[0] = translation[2] = 0.0;
				}
				else if (selectedAxis == 2) {
					translation[0] = translation[1] = 0.0;
				}
			}
			else if (opMode == SCALE_MODE && selectedScaleAxis != EMPTY_ID) {
				// calculate scales of translation
				//aabbCenter(*model->bbox, boxCenter);
				compPlane(w, selectedModelPos, param);
				getRayDir(e->x(), e->y(), this->width(), this->height(), viewMat, projectionMat, dir);
				compPntOnPlane(param, eye, dir, projPnt);
				subtract3fv(projPnt, selectedModelPos, scale);
				aabbSize(*model->bbox, boxSize);
				div3fv(scale, boxSize, scale);
				if (selectedScaleAxis == 0) {
					scale[1] = scale[2] = 1.0;
				}
				else if (selectedScaleAxis == 1) {
					scale[0] = scale[2] = 1.0;
				}
				else if (selectedScaleAxis == 2) {
					scale[0] = scale[1] = 1.0;
				}
			}
			// update modifying matrix
			tsfmSetTranslate(translation, modifyingTransformation);
			tsfmSetRotate(rotation, modifyingTransformation);
			tsfmSetScale(scale, modifyingTransformation);
			//transformModel(model);
			transformMatrix(modifyingTransformation, modifyingMatrix, NULL);
			//tsfmSetTranslate(translation, transform);
			//transformAABB(model->bbox, transform);
			/*
			model = moa->objects[0]->models[selectedModelID];
						//cylinderProjection();
			*/
		}
		else {
			viewTheta += offsetY * 0.01;
			viewPhi += offsetX * 0.01;
			if (viewPhi > 2 * M_PI) viewPhi -= 2 * M_PI;
			else if (viewPhi < 2 * M_PI) viewPhi += 2 * M_PI;
		}
		break;
	case Qt::MiddleButton:
		lok[0] -= sqrt(viewRadius) * (u[0] * offsetX * 0.01 + v[0] * offsetY * 0.01);
		lok[1] -= sqrt(viewRadius) * (u[1] * offsetX * 0.01 + v[1] * offsetY * 0.01);
		lok[2] -= sqrt(viewRadius) * (u[2] * offsetX * 0.01 + v[2] * offsetY * 0.01);
		break;
	case Qt::RightButton:
		break;
	}
	oldPosX = e->x();
	oldPosY = e->y();
	fixView();
	update();
}

void ModelViewerOGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	int i, j;
	unsigned int objID;
	unsigned char pickedColor[4];
	float center[3], translation[3], transMat[16], newModelMat[16];
	MeshModel* model;

	isMousePressed = false;
	switch (e->button())
	{
	case Qt::LeftButton:
		if (selectedItem != NULL) {
			model = selectedItem->model;
			aabbCenter(*model->bbox, center);
			//tsfmAddTransformation(modifyingTransformation, model->transformation);
			//transformMatrix(model->transformation, newModelMat, NULL);
			transformMatrix(modifyingTransformation, newModelMat, center);
			tsfmAddTransformation(modifyingTransformation, model->transformation);
			multMat4fv(newModelMat, model->vbo->modelMat, model->vbo->modelMat);
			transformAABB(model->bbox, modifyingTransformation);
			printf("Min: %f %f %f\n", model->bbox->min[0], model->bbox->min[1], model->bbox->min[2]);
			printf("Max: %f %f %f\n\n", model->bbox->max[0], model->bbox->max[1], model->bbox->max[2]);
			//tsfmTranslate(modifyingTransformation, translation);
			//translate(translation[0], translation[1], translation[2], transMat);
			//multMat4x3fv(transMat, model->bbox->max, model->bbox->max);
			//multMat4x3fv(transMat, model->bbox->min, model->bbox->min);
			initTransformation(modifyingTransformation);
			identifyMat4fv(modifyingMatrix);
			selectedAxis = selectedRotCircle = selectedScaleAxis = EMPTY_ID;
			calModelObjectBBox(scene->root);
		}
		break;
	case Qt::MidButton:
		break;
	case Qt::RightButton:
		break;
	default:
		break;
	}
	update();
}

void ModelViewerOGLWidget::keyPressEvent(QKeyEvent *e)
{

}

void ModelViewerOGLWidget::wheelEvent(QWheelEvent *e)
{
	viewRadius -= viewRadius * e->delta() / 250;
	fixView();
	update();
}


// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.

void ModelViewerOGLWidget::renderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

// Slots

void ModelViewerOGLWidget::openSTL(const char *fileName)
{
	int i;
	MeshModelObject* obj;
	MeshModelItem* item;
	MeshModel* model;
	VBO* vbo;

	// load model and add to the scene
	obj = loadSTL(fileName);
	addModelObjectToScene(scene, obj);

	// singal modelitem
	item = lastModelItem(scene->items);

	// gen model's vbo
	model = item->model;
	vbo = createModelVBO(model);
	setModelID(model, vbo);
	//addVertexBufferObject(vbo, vboa);
	setColorVBO(1.2, 1.0, 0.47, 1.0, vbo);
	bindData(vbo);
	bindData(model->bbox->vbo);

	//showModelItems(scene);

	emit sendModelScene(scene);
	//managerWidget->setModelScene(scene);
	update();
}

void ModelViewerOGLWidget::showVoxelizeWidget(void)
{
	//if (isEnqEmpty(&scene->items.link)) return;
	voxelizeWidget->setModelScene(scene);
	voxelizeWidget->show();
	//emit(sendModelScene(scene));
	//emit(changeToVoxelViewer());
}

void ModelViewerOGLWidget::confirmVoxelize(float xSize, float ySize, float zSize, bool isSolid)
{
	emit voxelizeModelScene(this->scene, xSize, ySize, zSize, isSolid);
	emit changeToVoxelViewer();
}

// 2017 / 08 / 01.  Need to modified, because of the struct changed.
void ModelViewerOGLWidget::genIsoSurface(float value, DistanceField *df)
{
	/*
	int i;
	MeshModelObject* obj;
	MeshModelObject* tmvbo;
	MeshModel* model;
	VBO* vbo;
	VBO* tmpVbo;

	obj = marchingCube(value, df);
	model = obj->models[0];
	vbo = createModelVBO(model);
	if (moa->numObjects <= 0) {
		addModelObjectToMOA(obj, moa);
		strcpy(vbo->name, model->name);
		addVertexBufferObject(vbo, vboa);
	}
	else {
		for (i = 0; i < moa->numObjects; ++i)
		{
			if (!strcmp(moa->objects[i]->name, "Iso-surface")) {
				tmvbo = moa->objects[i];
				moa->objects[i] = obj;
				deleteModelObject(&tmvbo);
			}
		}
		
		for (i = 0; i < vboa->numVBOs; ++i)
		{
			if (!strcmp(vboa->vbos[i]->name, "Iso-surface")) {
				tmpVbo = vboa->vbos[i];
				vboa->vbos[i] = vbo;
				deleteVBO(&tmpVbo);
			}
		}
	}

	setColorVBO(1.2, 1.0, 0.47, 1.0, vbo);
	bindData(vbo);

	update();
	*/
}

void ModelViewerOGLWidget::toggleContextMenu(const QPoint &pos)
{
	QMenu menu(tr("Actions"), this);
	QMenu panelMenu(tr("Panels"), this);

//  Panels
	QAction action1("Lighting panel", this);
	action1.setCheckable(true);
	action1.setChecked(this->lightWidget->isVisible());
	connect(&action1, SIGNAL(triggered()), this, SLOT(openLightWidget()));
	panelMenu.addAction(&action1);

	menu.addMenu(&panelMenu);

	menu.exec(mapToGlobal(pos));
}

void ModelViewerOGLWidget::openLightWidget(void)
{
	if (lightWidget->isVisible()) {
		lightWidget->close();
	}
	else {
		lightWidget->show();
	}
}

void ModelViewerOGLWidget::updateProjectionTexture(QImage *texture)
{
	activeTextureMapping = true;
	glBindTexture(GL_TEXTURE_2D, testTexID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width(), texture->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texture->bits());
	update();
}

void ModelViewerOGLWidget::actionOnModelObject(MeshModelObject *obj, int action)
{
	MODEL_SCENE_OBJ_ACTIONS ac;

	ac = (MODEL_SCENE_OBJ_ACTIONS)action;

	switch (ac)
	{
	case MOBJ_SELECT:
		emit sendModelObject(obj);
		break;
	case MOBJ_DELETE:
		if (obj->item == selectedItem) {
			selectedItem = NULL;
		}
		delModelObjectFromScene(scene, obj);
		break;
	default:
		break;
	}
	update();
}

void ModelViewerOGLWidget::changeToViewMode(void)
{
	opMode = VIEW_MODE;
	update();
}

void ModelViewerOGLWidget::changeToTranslateMode(void)
{
	opMode = TRANSLATE_MODE;
	update();
}

void ModelViewerOGLWidget::changeToRotateMode(void)
{
	opMode = ROTATE_MODE;
	update();
}

void ModelViewerOGLWidget::changeToScaleMode(void)
{
	opMode = SCALE_MODE;
	update();
}