#pragma once
extern "C" {
#include "vbo.h"
#include "light.h"
#include "material.h"
#include "meshmodel.h"
#include "distancefield.h"
#include "marchingcube.h"
}
#include "lightwidget.hpp"
#include "modelscenemanagerwidget.hpp"
#include "imageprojectionwidget.hpp"
#include "voxelizewidget.hpp"
#include <GL\glew.h>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QMouseEvent>


#define MAX_VBO 100

class ModelViewerOGLWidget : public QOpenGLWidget {
	Q_OBJECT

public:
	ModelViewerOGLWidget(QWidget  *parent = Q_NULLPTR);
	~ModelViewerOGLWidget();

protected:
	void initializeGL(void);
	void paintGL(void);
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);


private:
	void setupOpenGL(void);
	void setViewingMatrix(void);
	void setLighting(void);
	void setClipping(void);
	void setMaterial(VBO obj);
	void fixView(void);
	void updateViewing(void);

	// render
	void bindData(VBO *vbo);
	void drawStaticScene();
	void drawAxes(int mode);
	void drawRotCircle(int mode);
	void drawScaleAxes(int mode);
	void drawData(VBO *const vbo, int mode);
	void drawBbox(VBO *const vbo, int mode);
	void renderScene(void);
	void renderBbox(void);

	// shadow mapping
	void genShadowMapping(void);
	void configureShadowMapping(void);
	void renderQuad(void);

	// multi-sampling
	void genMultiSampling(void);

	// ssao
	void genSSAO(void);
	void resizeSSAO(void);
	void configureGBufferShader(void);
	void gBufferRendering(void);
	void ssaoTextureRendering(void);
	void ssaoBlurRendering(void);
	void ssaoLightPassRendering(void);
	void shadowMappingRendering(void);

	// texture projection mapping
	void cylinderProjection();

signals:
	void changeToVoxelViewer(void);
	void sendModelScene(MeshModelScene *sc);
	void voxelizeModelScene(MeshModelScene *sc, float xSize, float ySize, float zSize, bool isSolid);
	void selectModel(MeshModelItem *item);
	void sendModelObject(MeshModelObject *obj);

public slots:
	// context menu
	void toggleContextMenu(const QPoint& pos);
	// menu actions
	void openLightWidget(void);
	// get texture from imageprojectionwidget
	void updateProjectionTexture(QImage *texture);
	// get seleted model from moamanager
	void actionOnModelObject(MeshModelObject *obj, int action);

	void openSTL(const char *fileName);
	void showVoxelizeWidget(void);
	void confirmVoxelize(float xSize, float ySize, float zSize, bool isSolid);
	void genIsoSurface(float value, DistanceField *df);

	// change operation mode
	void changeToViewMode(void);
	void changeToTranslateMode(void);
	void changeToRotateMode(void);
	void changeToScaleMode(void);

// widget
private:
	QPushButton *voxelizeButton;
	LightWidget *lightWidget;
	//ModelSceneManagerWidget *managerWidget;
	ImageProjectionWidget *textureWidget;
public:
	VoxelizeWidget *voxelizeWidget;

private:
	int iconSize;
	// operating mode
	int opMode;

	// camera
	float eye[3];
	float lok[3];
	float vup[3];
	float u[3];
	float v[3];
	float w[3];
	float viewTrans[3];
	float viewRot[3];
	float viewPhi;
	float viewTheta;
	float viewRadius;
	float fovy;
	float nearClip;
	float farClip;
	float curRot[16];
	float lastRot[16];

	// lights
	int numLights;
	Light *lights[10];
	GlobalLight *globalLight;

	// clip
	float clip[4];

	// matrices
	float modelMat[16];
	float viewMat[16];
	float projectionMat[16];
	float mvMat[16];
	float mvpMat[16];
	float normalMat[16];


	// shadow
	float lightSpaceMat[16];
	GLuint depthMapFBO;
	GLuint shadowWidth;
	GLuint shadowHeight;
	GLuint depthMap;

	// multi-sampling
	GLuint multiSampleTex;
	GLuint multiSampleFBO;
	GLuint multiSampleColorBuffer;
	GLuint multiSampleDepthBuffer;

	// ssao
	GLuint kernelSize;
	GLuint noiseSize;
	GLfloat kernelRadius;
	GLfloat *ssaoKernel;
	GLfloat *ssaoNoise;
	GLuint quadVAO;
	GLuint quadVBO;

	GLuint gBuffer;
	GLuint gPositionDepth;
	GLuint gNormal;
	GLuint gAlbedo;
	GLuint rboDepth;
	GLuint noiseTexture;
	GLuint ssaoFBO;
	GLuint ssaoBlurFBO;
	GLuint ssaoColorBuffer;
	GLuint ssaoColorBufferBlur;

	// object uniform location
	GLuint colorLoc;

	// light uniform location
	GLint lightLoc0;

	// flag uniform location
	GLint wireframe;

	// shader program
	GLuint program[10];

	// mouse controll
	int oldPosX;
	int oldPosY;
	bool isMousePressed;

	// widget size
	int oldWidth;
	int oldHeight;

	// selected
	MeshModelItem *selectedItem;
	MeshModelObject *selectedObject;

	// mesh model scene
	MeshModelScene *scene;

	// ground
	VBO *ground;

	// project cylinder
	VBO *projCylinder;

	// translate axes
	VBO *axisX;
	VBO *axisY;
	VBO *axisZ;
	Transformation *axesTsfm;
	int selectedAxis;
	float selectedModelPos[3];
	
	// rotate circles
	VBO *rotCircleX;
	VBO *rotCircleY;
	VBO *rotCircleZ;
	Transformation *rotCircleTsfm;
	int selectedRotCircle;
	float selectedCirclePos[3];

	// scale axes
	VBO *scaleAxisX;
	VBO *scaleAxisY;
	VBO *scaleAxisZ;
	Transformation *scaleAxesTsfm;
	int selectedScaleAxis;
	float selectedScaleAxisPos[3];

	// the transformation for current selected model
	Transformation *modifyingTransformation;
	float modifyingMatrix[16];

	// vbo array
	VBOArray *vboa;

	// test texture
	GLuint testTexID;
	bool activeTextureMapping;
	
	// effects
	bool enableShadowMapping;
	bool enableSSAO;
};
