#ifndef _VBO_H
#define _VBO_H

#include "material.h"
#include <GL\glew.h>
#include <stdbool.h>


#define OBJECT_NAME_SIZE 50
#define BBOX_SIZE 36

enum VERTEX_ATTRIBS {VERTEX, NORMAL, TEXCOORD, COLOR, FACE, WIREFRAME, NUM_ATTRIBS};

// transformation
typedef struct Transformation
{
	float translate[3];
	float rotate[3];
	float scale[3];
}Transformation;

typedef struct VBO
{
	char name[OBJECT_NAME_SIZE];
	bool bboxDetect;
	bool enableBuffers[NUM_ATTRIBS];
	bool enable;
	bool wfEnable;
	bool isGenBuffer;
	int numVertices;
	int numWFVertices;
	int numWFLoop;
	unsigned char dataSize[NUM_ATTRIBS];
	GLuint buffers[NUM_ATTRIBS];
	float *attribs[NUM_ATTRIBS];
	int attribsSize[NUM_ATTRIBS];
	Transformation *transformation;
	float modelMat[16];
	Material *material;
	float clip[4];
	bool isClip;
	float color[4];
}VBO;

typedef struct VBOArray
{
	int numVBOs;
	int capVBOs;
	VBO **vbos;
}VBOArray;

//
//		Transformation
//

Transformation *newTransformation(void);

void deleteTransformation(Transformation **tsfm);

void initTransformation(Transformation *tsfm);

void copyTransformation(Transformation *src, Transformation *dest);

void tsfmTranslate(Transformation *tsfm, float translate[3]);

void tsfmRotate(Transformation *tsfm, float rotate[3]);

void tsfmScale(Transformation *tsfm, float scale[3]);

void tsfmSetTranslate(float translate[3], Transformation *tsfm);

void tsfmSetRotate(float rotate[3], Transformation *tsfm);

void tsfmSetScale(float scale[3], Transformation *tsfm);

void tsfmAddTranslate(float translate[3], Transformation *tsfm);

void tsfmAddRotate(float rotate[3], Transformation *tsfm);

void tsfmAddScale(float scale[3], Transformation *tsfm);

void tsfmAddTransformation(Transformation *src, Transformation *dest);

void transformMatrix(Transformation *tsfm, float mat[16], float center[3]);

//
//		Object
//

// Allocate a vbo struct and initialize data.
VBO *newVBO(void);

// Free data.
void deleteVBO(VBO **vbo);

// Copy the attribute data from one vbo to the other.
// "offsetDst" amd "offsetSrc" is used to offset the address of the attribute
void copyVBOAttribute(int vboAttrib, int offsetDst, int offsetSrc, VBO *dst, VBO *src);

// Get triangle position array
// v0(x, y, z) v1(x, y, z) v2(x, y, z)
//void getTriangle(VBO vbo, int index, float v[9]);

void compNormals(VBO *vbo);

void translateVBO(VBO *vbo, bool isRotated);

void toColorID(unsigned int index, float color[4]);

void toObjectID(const unsigned char color[4], unsigned int *index);

void registerID(VBO *vbo, int id);

void setColor(float r, float g, float b, float a, VBO *vbo);

void setColorVBO(float r, float g, float b, float a, VBO *vbo);

VBO *mergeVBO(VBO *vbo1, VBO *vbo2);

//
//		vboa
//
VBOArray *newVertexBufferObjectArray();

void deleteVertexBufferObjectArray(VBOArray **vboa);

void addVertexBufferObject(VBO *vbo, VBOArray *vboa);

//VBO *loadVoxelModel(const char* filenmae);


//
//		Drawing Stuff
//

void createPolygonVBO(float v0[3], float v1[3], float v2[3], float v3[3], float v[18], float vn[18]);

void createPolygonTexCoordVBO(float u, float v, float du, float dv, float texcoord[12]);

VBO *newPlane(int xSize, int ySize, float gap);

VBO *newBox(bool face[8]);

VBO *newBoxWF(bool face[8]);

VBO *newCircle(float radius, int slices, int stacks);

VBO *newCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);

VBO *newAxis(float shaftHeight, float shaftRadius, float pointRadius, float pointHeight);

VBO *newHammer(float shaftHeight, float shaftRadius, float boxXSize, float boxYSize, float boxZSize);

void initBox(VBO *vbo, bool face[8]);

void initBoxWF(VBO *vbo, bool face[8]);

#endif