#include "vbo.h"
#include "mymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define X 0
#define Y 1
#define Z 2


static VBO *asciiSTL(const char* filename);
static VBO *binarySTL(const char* filename);

// Marco
#define FILE_ERROR_MSG(){printf_s("File Error\n");}

//
//		Transformation
//

Transformation *newTransformation(void)
{
	Transformation *tsfm;

	tsfm = (Transformation*)calloc(1, sizeof(Transformation));
	tsfm->scale[0] = tsfm->scale[1] = tsfm->scale[2] = 1;

	return tsfm;
}

void deleteTransformation(Transformation **tsfm)
{
	free(*tsfm);
	*tsfm = NULL;
}

void initTransformation(Transformation *tsfm)
{
	tsfm->translate[0] = tsfm->translate[1] = tsfm->translate[2] = 0.0;
	tsfm->rotate[0] = tsfm->rotate[1] = tsfm->rotate[2] = 0.0;
	tsfm->scale[0] = tsfm->scale[1] = tsfm->scale[2] = 1.0;
}

void copyTransformation(Transformation *src, Transformation *dest)
{
	if (src != NULL && dest != NULL) {
		memcpy(dest, src, sizeof(Transformation));
	}
}

void tsfmTranslate(Transformation *tsfm, float translate[3])
{
	translate[0] = tsfm->translate[0];
	translate[1] = tsfm->translate[1];
	translate[2] = tsfm->translate[2];
}

void tsfmRotate(Transformation *tsfm, float rotate[3])
{
	rotate[0] = tsfm->rotate[0];
	rotate[1] = tsfm->rotate[1];
	rotate[2] = tsfm->rotate[2];
}

void tsfmScale(Transformation *tsfm, float scale[3])
{
	scale[0] = tsfm->scale[0];
	scale[1] = tsfm->scale[1];
	scale[2] = tsfm->scale[2];
}

void tsfmSetTranslate(float translate[3], Transformation *tsfm)
{
	tsfm->translate[0] = translate[0];
	tsfm->translate[1] = translate[1];
	tsfm->translate[2] = translate[2];
}

void tsfmSetRotate(float rotate[3], Transformation *tsfm)
{
	tsfm->rotate[0] = rotate[0];
	tsfm->rotate[1] = rotate[1];
	tsfm->rotate[2] = rotate[2];
}

void tsfmSetScale(float scale[3], Transformation *tsfm)
{
	tsfm->scale[0] = scale[0];
	tsfm->scale[1] = scale[1];
	tsfm->scale[2] = scale[2];
}

void tsfmAddTranslate(float translate[3], Transformation *tsfm)
{
	tsfm->translate[0] += translate[0];
	tsfm->translate[1] += translate[1];
	tsfm->translate[2] += translate[2];
}

void tsfmAddRotate(float rotate[3], Transformation *tsfm)
{
	tsfm->rotate[0] += rotate[0];
	tsfm->rotate[1] += rotate[1];
	tsfm->rotate[2] += rotate[2];
}

void tsfmAddScale(float scale[3], Transformation *tsfm)
{
	tsfm->scale[0] *= scale[0];
	tsfm->scale[1] *= scale[1];
	tsfm->scale[2] *= scale[2];
}

void tsfmAddTransformation(Transformation *src, Transformation *dest)
{
	tsfmAddTranslate(src->translate, dest);
	tsfmAddRotate(src->rotate, dest);
	tsfmAddScale(src->scale, dest);
}

void transformMatrix(Transformation *tsfm, float mat[16], float center[3])
{
	float transMat[16], rotateXMat[16], rotateYMat[16], rotateZMat[16], scaleMat[16], originMat[16], centerMat[16];

	translate(tsfm->translate[X], tsfm->translate[Y], tsfm->translate[Z], transMat);
	rotateX(tsfm->rotate[X], rotateXMat);
	rotateY(tsfm->rotate[Y], rotateYMat);
	rotateZ(tsfm->rotate[Z], rotateZMat);
	scale(tsfm->scale[0], tsfm->scale[1], tsfm->scale[2], scaleMat);
	if (center != NULL) {
		translate(-center[0], -center[1], -center[2], originMat);
		translate(center[0], center[1], center[2], centerMat);
	}

	identifyMat4fv(mat);
	if(center != NULL) {
		multMat4fv(originMat, mat, mat);
	}
	multMat4fv(scaleMat, mat, mat);
	multMat4fv(rotateXMat, mat, mat);
	multMat4fv(rotateYMat, mat, mat);
	multMat4fv(rotateZMat, mat, mat);
	if (center != NULL) {
		multMat4fv(centerMat, mat, mat);
	}
	multMat4fv(transMat, mat, mat);
}

//
//		Object
//

VBO *newVBO(void)
{
	int i;
	VBO *vbo;

	vbo = (VBO*)calloc(1, sizeof(VBO));
	vbo->material = newMaterial();
	identifyMat4fv(vbo->modelMat);
	vbo->dataSize[VERTEX] = 3;
	vbo->dataSize[NORMAL] = 3;
	vbo->dataSize[TEXCOORD] = 2;
	vbo->dataSize[COLOR] = 4;
	vbo->dataSize[FACE] = 4;
	vbo->dataSize[WIREFRAME] = 3;
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		vbo->attribs[i] = NULL;
	}
	vbo->transformation = newTransformation();

	return vbo;
}

void deleteVBO(VBO **vbo)
{
	int i = 0;

	if (*vbo != NULL) {
		for (i = 0; i < NUM_ATTRIBS; ++i)
		{
			if ((*vbo)->attribs[i] != NULL) {
				free((*vbo)->attribs[i]);
				(*vbo)->attribs[i] = NULL;
			}
		}
		free(*vbo);
	}
	*vbo = NULL;
}

void copyVBOAttribute(int vboAttrib, int offsetDst, int offsetSrc, VBO *dst, VBO *src)
{
	int i;

	if (src->dataSize[vboAttrib] == dst->dataSize[vboAttrib]) {
		memcpy( dst->attribs[vboAttrib] + offsetDst,
				src->attribs[vboAttrib] + offsetSrc,
				src->dataSize[vboAttrib] * sizeof(float) * src->numVertices
		);
	}
}
/*
void getTriangle(VBO vbo, int index, float v[9])
{
	int i;

	if (index * 9 <= vbo.numVertices * 3) {
		for (i = 0; i < 9; ++i)
		{
			v[i] = vbo.attribs[VERTEX][index * 9 + i];
		}
	}
	else { 
		for (i = 0; i < 9; ++i) 
		{
			v[i] = 0;
		}
	}
}
*/

void compNormals(VBO *vbo)
{
	int i, j;
	float v[9], n[3];

	// each triangles
	for (i = 0; i < vbo->numVertices; i += 3)
	{
		memcpy(v, vbo->attribs[VERTEX] + i * 3, 9 * sizeof(float));
		compNormal(v, n);
		memcpy(vbo->attribs[NORMAL] + i * 3, n, 3 * sizeof(float));
		memcpy(vbo->attribs[NORMAL] + i * 3 + 3, n, 3 * sizeof(float));
		memcpy(vbo->attribs[NORMAL] + i * 3 + 6, n, 3 * sizeof(float));
	}
}

void translateVBO(VBO *vbo, bool isRotated)
{
	int i;
	float normalMat[16];

	for (i = 0; i < vbo->numVertices; ++i)
	{
		multMat4x3fv(vbo->modelMat, vbo->attribs[VERTEX] + i * 3, vbo->attribs[VERTEX] + i * 3);
	}
	if (vbo->attribs[WIREFRAME] != NULL) {
		for (i = 0; i < vbo->numWFVertices; ++i)
		{
			multMat4x3fv(vbo->modelMat, vbo->attribs[WIREFRAME] + i * 3, vbo->attribs[WIREFRAME] + i * 3);
		}
	}
	if (isRotated) {
		normalM(vbo->modelMat, normalMat);
		for (i = 0; i < vbo->numVertices; ++i)
		{
			multMat4x3fv(normalMat, vbo->attribs[NORMAL] + i * 3, vbo->attribs[NORMAL] + i * 3);
		}
	}
	identifyMat4fv(vbo->modelMat);
}

void toColorID(unsigned int index, float color[4])
{
	int i;

	for (i = 0; i < 4; ++i)
	{
		color[i] = index % 256 / 255.0;
		index /= 256;
	}
}

void toObjectID(const unsigned char color[4], unsigned int* index)
{
	*index = color[0] + color[1] * 256 + color[2] * 256 * 256 + color[3] * 256 * 256 * 256;
}


// - register a face id to vbo
// - it will create a face vbo array, and turn the id value into 4 floats
// * note: need to re-bind data after registering id
void registerID(VBO *vbo, int id)
{
	int i, j, k;
	float face[4];

	// create unique color for off-scene rendering picking
	toColorID(id, face);
	if (vbo->attribs[FACE] != NULL) {
		free(vbo->attribs[FACE]);
	}
	vbo->attribs[FACE] = (float*)calloc(vbo->numVertices * vbo->dataSize[FACE], sizeof(float));

	for (i = 0; i < vbo->numVertices; ++i)
	{
		memcpy(vbo->attribs[FACE] + i * 4, face, vbo->dataSize[FACE] * sizeof(float));
	}
	vbo->enableBuffers[FACE] = true;

	/*
	vbo->id = *id / 6;
	vbo->bbox->attribs[COLOR] = (float*)calloc(BBOX_SIZE * vbo->bbox->dataSize[COLOR], sizeof(float));
	vbo->bbox->attribs[FACE] = (float*)calloc(BBOX_SIZE * vbo->bbox->dataSize[FACE], sizeof(float));
	for (i = 0; i < 6; ++i)
	{
		toColorID((*id)++, face);
		for (j = 0; j < 6; ++j)
		{
			for (k = 0; k < vbo->bbox->dataSize[FACE]; ++k)
			{
				vbo->bbox->attribs[FACE][i * 6 * vbo->bbox->dataSize[FACE] + j * vbo->bbox->dataSize[FACE] + k] = face[k];
			}
		}
	}
	vbo->bbox->enableBuffers[COLOR] = vbo->bbox->enableBuffers[FACE] = true;
	*/
}

void setColor(float r, float g, float b, float a, VBO * vbo)
{
	vbo->color[0] = r;
	vbo->color[1] = g;
	vbo->color[2] = b;
	vbo->color[3] = a;
}

void setColorVBO(float r, float g, float b, float a, VBO *vbo)
{
	int i;

	vbo->attribs[COLOR] = (float*)calloc(vbo->numVertices * vbo->dataSize[COLOR], sizeof(float));
	for (i = 0; i < vbo->numVertices; ++i)
	{
		vbo->attribs[COLOR][i * vbo->dataSize[COLOR]] = r;
		vbo->attribs[COLOR][i * vbo->dataSize[COLOR] + 1] = g;
		vbo->attribs[COLOR][i * vbo->dataSize[COLOR] + 2] = b;
		vbo->attribs[COLOR][i * vbo->dataSize[COLOR] + 3] = a;
	}
	setColor(r, g, b, a, vbo);
	vbo->enableBuffers[COLOR] = true;;
	vbo->material->isEnable = false;
}

VBO *mergeVBO(VBO *vbo1, VBO *vbo2)
{
	int i;
	VBO *vbo;

	vbo = newVBO();

	vbo->numVertices = vbo1->numVertices + vbo2->numVertices;
	// merge vbo1 and vbo2 into vbo
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{	
		if (vbo1->attribs[i] != NULL && vbo2->attribs[i] != NULL && vbo1->dataSize[i] == vbo2->dataSize[i]) {
			
			// allocate memory
			vbo->dataSize[i] = vbo1->dataSize[i];
			vbo->attribs[i] = (float*)calloc(vbo->numVertices * vbo->dataSize[i], sizeof(float));
			vbo->enableBuffers[i] = true;

			// copy vbo1 into vbo
			copyVBOAttribute(i, 0, 0, vbo, vbo1);

			// copy vbo2 into vbo
			copyVBOAttribute(i, vbo1->numVertices * vbo->dataSize[i], 0, vbo, vbo2);
		}
	}

	return vbo;
}


VBOArray *newVertexBufferObjectArray()
{
	VBOArray *vboa;

	vboa = (VBOArray*)calloc(1, sizeof(VBOArray));
	vboa->capVBOs = 1;
	vboa->vbos = (VBO**)calloc(1, sizeof(VBO*));

	return vboa;
}

void deleteVertexBufferObjectArray(VBOArray **vboa)
{
	int i;

	for (i = 0; i < (*vboa)->numVBOs; ++i)
	{
		deleteVBO(&(*vboa)->vbos[i]);
	}
	free((*vboa)->vbos);
	free(*vboa);
}

void addVertexBufferObject(VBO *vbo, VBOArray *vboa)
{
	// realloc
	if (vboa->numVBOs >= vboa->capVBOs) {
		vboa->capVBOs *= 2;
		vboa->vbos = (VBO**)realloc(vboa->vbos, vboa->capVBOs * sizeof(VBO*));
	}

	// add vbo
	vboa->vbos[vboa->numVBOs++] = vbo;
}


//
//		Drawing Stuff
//

//
//   5¢z¡X¢{2 4
//    ¢x¢¬¢x
// 0 3¢|¢w¢}1
//
static void createTile(float x, float y,  float gap, float v[18], float n[18])
{
	v[0] = x; v[1] = y; v[2] = 0;
	v[3] = x + gap; v[4] = y; v[5] = 0;
	v[6] = x; v[7] = y + gap; v[8] = 0;
	v[9] = x + gap; v[10] = y + gap; v[11] = 0;
	v[12] = x; v[13] = y + gap; v[14] = 0;
	v[15] = x + gap; v[16] = y; v[17] = 0;

	n[0]  = 0;  n[1]  = 0;   n[2]  = 1;
	n[3]  = 0;  n[4]  = 0;   n[5]  = 1;
	n[6]  = 0;  n[7]  = 0;   n[8]  = 1;
	n[9]  = 0;  n[10] = 0;   n[11] = 1;
	n[12] = 0;  n[13] = 0;   n[14] = 1;
	n[15] = 0;  n[16] = 0;   n[17] = 1;
}

//
//  v3¢z¡X¢{v2
//    ¢x¢¬¢x
//  v0¢|¢w¢}v1
//
//
//   5¢z¡X¢{2 4
//    ¢x¢¬¢x
// 0 3¢|¢w¢}1
//
void createPolygonVBO(float v0[3], float v1[3], float v2[3], float v3[3], float v[18], float vn[18])
{
	int i;
	float e0[3], e1[3], normal[3];

	memcpy(v, v0, 3 * sizeof(float));
	memcpy(v + 3, v1, 3 * sizeof(float));
	memcpy(v + 6, v2, 3 * sizeof(float));
	memcpy(v + 9, v0, 3 * sizeof(float));
	memcpy(v + 12, v2, 3 * sizeof(float));
	memcpy(v + 15, v3, 3 * sizeof(float));

	e0[0] = v1[0] - v0[0];
	e0[1] = v1[1] - v0[1];
	e0[2] = v1[2] - v0[2];
	e1[0] = v2[0] - v0[0];
	e1[1] = v2[1] - v0[1];
	e1[2] = v2[2] - v0[2];
	cross3fv(e1, e0, normal);
	normalize3fv(normal);
	for (i = 0; i < 18; i += 3)
	{
		memcpy(vn + i, normal, 3 * sizeof(float));
	}
}

void createPolygonTexCoordVBO(float u, float v, float du, float dv, float vt[12])
{
	float v0[2] = { u, v };
	float v1[2] = { u + du, v };
	float v2[2] = { u + du, v + dv };
	float v3[2] = { u, v + dv };

	memcpy(vt, v0, 2 * sizeof(float));
	memcpy(vt + 2, v1, 2 * sizeof(float));
	memcpy(vt + 4, v2, 2 * sizeof(float));
	memcpy(vt + 6, v0, 2 * sizeof(float));
	memcpy(vt + 8, v2, 2 * sizeof(float));
	memcpy(vt + 10, v3, 2 * sizeof(float));
}

VBO *newPlane(int xSize, int ySize, float gap)
{
	int i, j, index;
	VBO *vbo;
	float tileV[18], tileN[18], mat[16], tmp;

	vbo = newVBO();
	vbo->numVertices = xSize * ySize * 6;
	vbo->attribs[VERTEX] = (float*)calloc(vbo->numVertices * 3, sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(vbo->numVertices * 3, sizeof(float));
	vbo->enableBuffers[VERTEX] = vbo->enableBuffers[NORMAL] = true;
	vbo->enable = true;
	strcpy(vbo->name, "plane");

	index = 0;
	for (i = 0; i < xSize; ++i)
	{
		for (j = 0; j < ySize; ++j)
		{
			createTile(i * gap, j * gap, gap, tileV, tileN);
			memcpy(vbo->attribs[VERTEX] + index, tileV, 18 * sizeof(float));
			memcpy(vbo->attribs[NORMAL] + index, tileN, 18 * sizeof(float));
			index += 18;
		}
	}

	return vbo;
}

VBO *newPlainWF(int xSize, int ySize, float gap)
{
	int i, j, k, index;
	VBO *vbo;
	float tileV[18], tileN[18], mat[16], tmp;

	vbo = newVBO();
	vbo->numVertices = 4 * xSize * ySize + 2 * (xSize + ySize);
	vbo->attribs[VERTEX] = (float*)calloc(vbo->numVertices * 3, sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(vbo->numVertices * 3, sizeof(float));
	vbo->enableBuffers[VERTEX] = vbo->enableBuffers[NORMAL] = true;
	vbo->enable = true;
	strcpy(vbo->name, "plane");

	// x-direct lines
	for (i = 0; i <= ySize; ++i)
	{
		for (j = 0; j < xSize; ++j)
		{
			// v1
			vbo->attribs[VERTEX][(i * xSize * 2 + j) * 3] = j * gap;
			vbo->attribs[VERTEX][(i * xSize * 2 + j) * 3 + 1] = i * gap;
			vbo->attribs[VERTEX][(i * xSize * 2 + j) * 3 + 2] = 0;
			// v2
			vbo->attribs[VERTEX][(i * xSize * 2 + j + 1) * 3] = (j + 1) * gap;
			vbo->attribs[VERTEX][(i * xSize * 2 + j + 1) * 3 + 1] = i * gap;
			vbo->attribs[VERTEX][(i * xSize * 2 + j + 1) * 3 + 2] = 0;
		}
	}

	// y-direct lines
	for (i = 0; i <= xSize; ++i)
	{
		for (j = 0; j < ySize; ++j)
		{
			// v1
			vbo->attribs[VERTEX][(i * ySize * 2 + j) * 3] = i * gap;
			vbo->attribs[VERTEX][(i * ySize * 2 + j) * 3 + 1] = j * gap;
			vbo->attribs[VERTEX][(i * ySize * 2 + j) * 3 + 2] = 0;
			// v2
			vbo->attribs[VERTEX][(i * ySize * 2 + j + 1) * 3] = i * gap;
			vbo->attribs[VERTEX][(i * ySize * 2 + j + 1) * 3 + 1] = (j + 1) * gap;
			vbo->attribs[VERTEX][(i * ySize * 2 + j + 1) * 3 + 2] = 0;
		}
	}

	return vbo;
}

VBO *newBox(bool face[8])
{
	VBO *vbo;
	int i, j, k, numFaces, numVertices;
	//	x, y, z position of eight vertices
	float v[24] =
	{ -0.5, -0.5,  0.5,		//  0
	  -0.5,  0.5,  0.5,		//	1
	   0.5,  0.5,  0.5,		//	2
	   0.5, -0.5,  0.5,		//	3
	  -0.5, -0.5, -0.5,		//	4
	  -0.5,  0.5, -0.5,		//	5
	   0.5,  0.5, -0.5,		//	6
	   0.5, -0.5, -0.5 };	//	7

	//	six box face normals
	float n[24] = 
	{  1,  0,  0,		//	0
	  -1,  0,  0,		//	1
	   0,  1,  0,		//	2
	   0, -1,  0,		//	3
	   0,  0,  1,		//	4
	   0,  0, -1,		//	5
	   0,  0,  0,		// better normal
	   0,  0,  0  };	// better normal

	if (face == NULL){
		numFaces = 6;
	}
	else {
		numFaces = 0;
		for (i = 0; i < 6; ++i) {
			face[i] ? ++numFaces : 0;
		}
	}

	int vidx[36] = { 2, 3, 7,		//	frnot
					 7, 6, 2,
					 5, 4, 0,		//	back
					 0, 1, 5,
					 6, 5, 1,		//	top
					 1, 2, 6,
					 3, 0, 4,		//	bottom
					 4, 7, 3,
					 1, 0, 3,		//	right
					 3, 2, 1,
					 4, 5, 6,		//	left
					 6, 7, 4 };

	int nidx[36] = { 0, 0, 0,		//	fonrt
					 0, 0, 0,
					 1, 1, 1,		//	back
					 1, 1, 1,
					 2, 2, 2,		//	top
					 2, 2, 2,
					 3, 3, 3,		//	bottom
					 3, 3, 3,
					 4, 4, 4,		//	right
					 4, 4, 4,
					 5, 5, 5,		//	left
					 5, 5, 5 };

	vbo = newVBO();
	strcpy(vbo->name, "box");
	vbo->attribs[VERTEX] = (float*)calloc(numFaces * 18, sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(numFaces * 18, sizeof(float));
	numVertices = 0;
	for (i = 0; i < 6; ++i) {
		if (face == NULL || face[i]) {
			for (j = 0; j < 6; ++j) {
				for (k = 0; k < 3; ++k) 
				{
					vbo->attribs[VERTEX][numVertices * 3 + k] = v[vidx[i * 6 + j] * 3 + k];
					vbo->attribs[NORMAL][numVertices * 3 + k] = n[nidx[i * 6 + j] * 3 + k];
				}
				++numVertices;
			}
		}
	}

	vbo->numVertices = numVertices;
	vbo->enableBuffers[VERTEX] = vbo->enableBuffers[NORMAL] = true;
	return vbo;
}

VBO *newBoxWF(bool face[8])
{
	VBO *vbo;
	int i, j, k, numFaces, numVertices;
	//	x, y, z position of eight vertices
	float v[24] =
	{ -0.5, -0.5,  0.5,		//  0
	  -0.5,  0.5,  0.5,		//	1
	   0.5,  0.5,  0.5,		//	2
	   0.5, -0.5,  0.5,		//	3
	  -0.5, -0.5, -0.5,		//	4
	  -0.5,  0.5, -0.5,		//	5
	   0.5,  0.5, -0.5,		//	6
	   0.5, -0.5, -0.5  };	//	7

	if (face == NULL) {
		numFaces = 6;
	}
	else {
		numFaces = 0;
		for (i = 0; i < 6; ++i) {
			face[i] ? ++numFaces : 0;
		}
	}

	int vidx[24] = { 2, 3, 7, 6,		//	frnot
					 5, 4, 0, 1,		//	back
					 6, 5, 1, 2,		//	top
					 3, 0, 4, 7,		//	bottom
					 1, 0, 3, 2,		//	right
					 4, 5, 6, 7 };		//	left

	vbo = newVBO();
	vbo->attribs[VERTEX] = (float*)calloc(numFaces * 12, sizeof(float));
	numVertices = 0;
	for (i = 0; i < 6; ++i) {
		if (face == NULL || face[i]) {
			for (j = 0; j < 4; ++j) {
				for (k = 0; k < 3; ++k)
				{
					vbo->attribs[VERTEX][numVertices * 3 + k] = v[vidx[i * 4 + j] * 3 + k];
				}
				++numVertices;
			}
		}
	}
	vbo->numVertices = numVertices;
	vbo->enableBuffers[VERTEX] = true;
	return vbo;
}


// - create a cirlce vbos for rendering
// - create vertices, normals and wireframe vbo
VBO *newCircle(float radius, int slices, int stacks)
{
	int i, j, k, numVertices, numWFVertices, total;
	float theta, thetaNext, increase, v[9], vn[9], vt[6], wf[6];
	VBO *vbo;


	// instance
	vbo = newVBO();

	// allocate vbos' memories
	total = slices * 3;	// numbers of vertices (vertices of triangle mesh)
	vbo->attribs[VERTEX] = (float*)calloc(total * vbo->dataSize[VERTEX], sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(total * vbo->dataSize[NORMAL], sizeof(float));
	//vbo->attribs[TEXCOORD] = (float*)calloc(total * vbo->dataSize[TEXCOORD], sizeof(float));
	total = slices * 2;
	vbo->attribs[WIREFRAME] = (float*)calloc(total * vbo->dataSize[WIREFRAME], sizeof(float));

	// stacks are not proccessed...
	increase = 2 * PI / (float)slices;
	numVertices = 0;
	numWFVertices = 0;
	for (i = 0; i < slices; ++i)
	{
		theta = i * increase;
		thetaNext = (i + 1) * increase;
		v[0] = wf[0] = radius * cos(theta);
		v[1] = wf[1] = radius * sin(theta);
		v[2] = wf[2] = 0;
		v[3] = wf[3] = radius * cos(thetaNext);
		v[4] = wf[4] = radius * sin(thetaNext);
		v[5] = wf[5] = 0;
		v[6] = 0;
		v[7] = 0;
		v[8] = 0;
		vn[0] = vn[3] = vn[6] = 0;
		vn[1] = vn[4] = vn[7] = 0;
		vn[2] = vn[5] = vn[8] = 1;
		for (j = 0; j < 9; ++j) // 3 vertices * 3 values [xyz]
		{
			vbo->attribs[VERTEX][numVertices * 3 + j] = v[j];
			vbo->attribs[NORMAL][numVertices * 3 + j] = vn[j];
		}
		for (j = 0; j < 6; ++j)	// 2 vertices * 3 values [xyz]
		{
			vbo->attribs[WIREFRAME][numWFVertices * 3 + j] = wf[j];
		}
		numVertices += 3;
		numWFVertices += 2;
	}

	// enable setting
	vbo->numVertices = numVertices;
	vbo->numWFVertices = numWFVertices;
	vbo->enableBuffers[VERTEX] = vbo->enableBuffers[NORMAL] = vbo->enableBuffers[WIREFRAME] =  true;

	return vbo;
}

VBO *newCylinder(float baseRadius, float topRadius, float height, int slices, int stacks)
{
	int i, j, k, l, total, numVertices;
	VBO *vbo;
	VBO *topCircle;
	VBO *bottomCircle;
	float radius, radiusNext, increase, u, uNext, v, vNext, v0[3], v1[3], v2[3], v3[3], vertices[18], normals[18], texcoords[12];

	if (slices < 20) slices = 20;
	if (stacks < 0) stacks = 1;

	vbo = newVBO();

	total = slices * stacks * 2 * 3; // faces * 2 tri per face * 3 vert. per tri
	vbo->attribs[VERTEX] = (float*)calloc(total * vbo->dataSize[VERTEX], sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(total * vbo->dataSize[NORMAL], sizeof(float));
	vbo->attribs[TEXCOORD] = (float*)calloc(total * vbo->dataSize[TEXCOORD], sizeof(float));

	increase = (topRadius - baseRadius) / (float)stacks;

	// stacks
	numVertices = 0;
	for (i = 0; i < stacks; ++i)
	{
		radius = baseRadius + increase * i;
		radiusNext = baseRadius + increase * (i + 1);
		// slices
		for (j = 0; j < slices; ++j)
		{
			u = j / (float)slices;
			uNext = (j + 1) / (float)slices;
			v = i / (float)stacks;
			vNext = (i + 1) / (float)stacks;
			v0[0] = radius * cos(u * 2 * M_PI);
			v0[1] = v * height;
			v0[2] = radius * sin(u * 2 * M_PI); 
			v1[0] = radius * cos(uNext * 2 * M_PI);
			v1[1] = v * height;
			v1[2] = radius * sin(uNext * 2 * M_PI); 
			v2[0] = radiusNext * cos(uNext * 2 * M_PI);
			v2[1] = vNext * height;
			v2[2] = radiusNext * sin(uNext * 2 * M_PI);
			v3[0] = radiusNext * cos(u * 2 * M_PI);
			v3[1] = vNext * height;
			v3[2] = radiusNext * sin(u * 2 * M_PI);
			createPolygonVBO(v0, v1, v2, v3, vertices, normals);
			createPolygonTexCoordVBO(u, v, uNext - u, vNext - v, texcoords);
			for (k = 0; k < 6; ++k)
			{
				for (l = 0; l < 3; ++l)
				{
					vbo->attribs[VERTEX][numVertices * 3 + l] = vertices[k * 3 + l];
					vbo->attribs[NORMAL][numVertices * 3 + l] = normals[k * 3 + l];
					if (l < 2) {
						vbo->attribs[TEXCOORD][numVertices * 2 + l] = texcoords[k * 2 + l];
					}
				}
				numVertices++;
			}
		}
	}
	vbo->numVertices = numVertices;
	vbo->enableBuffers[VERTEX] = vbo->enableBuffers[NORMAL] = vbo->enableBuffers[TEXCOORD] = true;

	return vbo;
}

VBO *newAxis(float shaftHeight, float shaftRadius, float pointRadius, float pointHeight)
{
	VBO *vbo;
	VBO *cylinder;
	VBO *cone;
	VBO *circle;
	float transMat[16];
	float rotateMat[16];
	const int slices = 360;
	const int stacks = 100;
	
	// create a cylinder and an cone, then merge them into one
	cylinder = newCylinder(shaftRadius, shaftRadius, shaftHeight, slices, stacks);
	cone = newCylinder(pointRadius, 0.0, pointHeight, slices, stacks);
	translate(0, shaftHeight, 0, transMat);
	multMat4fv(transMat, cone->modelMat, cone->modelMat);
	translateVBO(cone, false);
	vbo = mergeVBO(cylinder, cone);

	deleteVBO(&cylinder);
	deleteVBO(&cone);
	
	// create circles
	circle = newCircle(shaftRadius, slices, stacks);	// cylinder bottom
	rotateX(-90, rotateMat);
	multMat4fv(rotateMat, circle->modelMat, circle->modelMat);
	translateVBO(circle, true);
	vbo = mergeVBO(vbo, circle);

	deleteVBO(&circle);

	circle = newCircle(shaftRadius, slices, stacks);	// cylinder top
	translate(0, shaftHeight, 0, transMat);
	rotateX(90, rotateMat);
	multMat4fv(rotateMat, circle->modelMat, circle->modelMat);
	multMat4fv(transMat, circle->modelMat, circle->modelMat);
	translateVBO(circle, true);
	vbo = mergeVBO(vbo, circle);

	deleteVBO(&circle);

	circle = newCircle(pointRadius, slices, stacks);	// cone bottom
	translate(0, shaftHeight, 0, transMat);
	rotateX(-90, rotateMat);
	multMat4fv(rotateMat, circle->modelMat, circle->modelMat);
	multMat4fv(transMat, circle->modelMat, circle->modelMat);
	translateVBO(circle, true);
	vbo = mergeVBO(vbo, circle);

	deleteVBO(&circle);

	return vbo;
}

VBO *newHammer(float shaftHeight, float shaftRadius, float boxXSize, float boxYSize, float boxZSize)
{
	VBO *vbo;
	VBO *cylinder;
	VBO *box;
	VBO *circle;
	float transMat[16];
	float rotateMat[16];
	float scaleMat[16];
	const int slices = 360;
	const int stacks = 100;

	// create a cylinder and an box, then merge them into one
	cylinder = newCylinder(shaftRadius, shaftRadius, shaftHeight, slices, stacks);
	box = newBox(NULL);
	scale(boxXSize, boxYSize, boxZSize, scaleMat);
	translate(0, shaftHeight, 0, transMat);
	multMat4fv(scaleMat, box->modelMat, box->modelMat);
	multMat4fv(transMat, box->modelMat, box->modelMat);
	translateVBO(box, false);
	vbo = mergeVBO(cylinder, box);

	deleteVBO(&cylinder);
	deleteVBO(&box);

	// create circles
	circle = newCircle(shaftRadius, slices, stacks);	// cylinder bottom
	rotateX(-90, rotateMat);
	multMat4fv(rotateMat, circle->modelMat, circle->modelMat);
	translateVBO(circle, true);
	vbo = mergeVBO(vbo, circle);

	deleteVBO(&circle);

	circle = newCircle(shaftRadius, slices, stacks);	// cylinder top
	translate(0, shaftHeight, 0, transMat);
	rotateX(90, rotateMat);
	multMat4fv(rotateMat, circle->modelMat, circle->modelMat);
	multMat4fv(transMat, circle->modelMat, circle->modelMat);
	translateVBO(circle, true);
	vbo = mergeVBO(vbo, circle);

	deleteVBO(&circle);

	return vbo;
}

void initBox(VBO *vbo, bool face[8])
{
	if (vbo == NULL) return;
	int i, j, k, numFaces, numVertices;
	//	x, y, z position of eight vertices
	float v[24] = 
	  { -0.5, -0.5,  0.5,		//  0
		-0.5,  0.5,  0.5,		//	1
		 0.5,  0.5,  0.5,		//	2
		 0.5, -0.5,  0.5,		//	3
		-0.5, -0.5, -0.5,		//	4
		-0.5,  0.5, -0.5,		//	5
		 0.5,  0.5, -0.5,		//	6
		 0.5, -0.5, -0.5  };	//	7

	//	six box face normals
	float n[24] = 
	  {  1,  0,  0,		//	0
		-1,  0,  0,		//	1
		 0,  1,  0,		//	2
		 0, -1,  0,		//	3
		 0,  0,  1,		//	4
		 0,  0, -1,     //	5
		 0,  0,  0,		// better normal
		 0,  0,  0  };  // better normal

	if (face == NULL){
		numFaces = 6;
	}
	else {
		numFaces = 0;
		for (i = 0; i < 6; ++i) {
			face[i] ? ++numFaces : 0;
		}
	}

	int vidx[36] = 
	  { 2, 3, 7,		//	frnot
		7, 6, 2,
		5, 4, 0,		//	back
		0, 1, 5,
		6, 5, 1,		//	top
		1, 2, 6,
		3, 0, 4,		//	bottom
		4, 7, 3,
		1, 0, 3,		//	right
		3, 2, 1,
		4, 5, 6,		//	left
		6, 7, 4 };

	int nidx[36] = 
	  { 0, 0, 0,		//	fonrt
		0, 0, 0,
		1, 1, 1,		//	back
		1, 1, 1,
		2, 2, 2,		//	top
		2, 2, 2,
		3, 3, 3,		//	bottom
		3, 3, 3,
		4, 4, 4,		//	right
		4, 4, 4,
		5, 5, 5,		//	left
		5, 5, 5 };

	memset(vbo->attribs[VERTEX], 0, 108 * sizeof(float));
	memset(vbo->attribs[NORMAL], 0, 108 * sizeof(float));
	numVertices = 0;
	for (i = 0; i < 6; ++i) {
		if (face == NULL || face[i]) {
			for (j = 0; j < 6; ++j) {
				for (k = 0; k < 3; ++k)
				{
					vbo->attribs[VERTEX][numVertices * 3 + k] = v[vidx[i * 6 + j] * 3 + k];
					vbo->attribs[NORMAL][numVertices * 3 + k] = n[nidx[i * 6 + j] * 3 + k];
				}
				++numVertices;
			}
		}
	}

	vbo->numVertices = numVertices;
	vbo->enableBuffers[VERTEX] = vbo->enableBuffers[NORMAL] = true;
}

void initBoxWF(VBO *vbo, bool face[8])
{
	if (vbo == NULL) return;
	int i, j, k, numFaces, numVertices;
	//	x, y, z position of eight vertices
	float v[24] =
	{ -0.5, -0.5,  0.5,		//  0
	  -0.5,  0.5,  0.5,		//	1
	   0.5,  0.5,  0.5,		//	2
	   0.5, -0.5,  0.5,		//	3
	  -0.5, -0.5, -0.5,		//	4
	  -0.5,  0.5, -0.5,		//	5
	   0.5,  0.5, -0.5,		//	6
	   0.5, -0.5, -0.5 };	//	7

	if (face == NULL) {
		numFaces = 6;
	}
	else {
		numFaces = 0;
		for (i = 0; i < 6; ++i) {
			face[i] ? ++numFaces : 0;
		}
	}

	int vidx[24] = 
	   { 2, 3, 7, 6,		//	frnot
		5, 4, 0, 1,			//	back
		6, 5, 1, 2,			//	top
		3, 0, 4, 7,			//	bottom
		1, 0, 3, 2,			//	right
		4, 5, 6, 7 };		//	left

	numVertices = 0;
	for (i = 0; i < 6; ++i) {
		if (face == NULL || face[i]) {
			for (j = 0; j < 4; ++j) {
				for (k = 0; k < 3; ++k)
				{
					vbo->attribs[VERTEX][numVertices * 3 + k] = v[vidx[i * 4 + j] * 3 + k];
				}
				++numVertices;
			}
		}
	}
	vbo->numVertices = numVertices;
	vbo->enableBuffers[VERTEX] = true;
}