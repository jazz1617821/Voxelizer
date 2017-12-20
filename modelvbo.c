#include "modelvbo.h"
#include "mymath.h"
#include <stdlib.h>
#include <string.h>

VBO *createModelVBO(MeshModel *model)
{
	int i, j, k, id;
	float v[3], vn[3], vt[2], colorID[4], offset[3], center[3], tmp, transMat[16], scaleMat[16];
	VBO *vbo;
	VBO *bboxVBO;
	VBO *bboxWFVBO;

	vbo = newVBO();
	strcpy(vbo->name, model->name);
	vbo->numVertices = model->numFaces * 3;

	// allocate memory
	vbo->enableBuffers[VERTEX] = vbo->enableBuffers[NORMAL] = true;
	//vbo->enableBuffers[COLOR]  = true;
	vbo->enableBuffers[FACE] = true;
	vbo->attribs[VERTEX] = (float*)calloc(vbo->numVertices * vbo->dataSize[VERTEX], sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(vbo->numVertices * vbo->dataSize[NORMAL], sizeof(float));
	//vbo->attribs[COLOR] = (float*)calloc(vbo->numVertices * vbo->dataSize[COLOR], sizeof(float));
	//vbo->attribs[FACE] = (float*)calloc(vbo->numVertices * vbo->dataSize[FACE], sizeof(float));

	// facet loop
	id = 0;
	for (i = 0; i < model->numFaces; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			getVertex3D(model->vertexArray, model->faces[i].v[j], v);
			getVertex3D(model->normalArray, model->faces[i].vn[j], vn);
			for (k = 0; k < 3; ++k)
			{
				vbo->attribs[VERTEX][i * 9 + j * 3 + k] = v[k];
				vbo->attribs[NORMAL][i * 9 + j * 3 + k] = vn[k];
			}
		}
	}
	
	// generating aabb vbo

	// create aabbVBO
	bboxVBO = newBox(NULL);
	bboxWFVBO = newBoxWF(NULL);
	bboxVBO->enableBuffers[FACE] = true;
	bboxVBO->attribs[FACE] = (float*)calloc(144, sizeof(float));
	toColorID(model->id, colorID);
	for (i = 0; i < 144; i += 4)
	{
		bboxVBO->attribs[FACE][i] = colorID[0];
		bboxVBO->attribs[FACE][i + 1] = colorID[1];
		bboxVBO->attribs[FACE][i + 2] = colorID[2];
		bboxVBO->attribs[FACE][i + 3] = colorID[3];
	}

	aabbCenter(*model->bbox, center);
	translate(center[0], center[1], center[2], transMat);
	scale(fabs(model->bbox->max[0] - model->bbox->min[0]),
		fabs(model->bbox->max[1] - model->bbox->min[1]),
		fabs(model->bbox->max[2] - model->bbox->min[2]),
		scaleMat);
	multMat4fv(scaleMat, bboxVBO->modelMat, bboxVBO->modelMat);
	multMat4fv(transMat, bboxVBO->modelMat, bboxVBO->modelMat);
	translateVBO(bboxVBO, false);
	
	model->bbox->vbo = bboxVBO;

	model->vbo = vbo;

	transformMatrix(model->transformation, model->vbo->modelMat, NULL);
	initTransformation(model->transformation);

	return vbo;
}

void setModelID(MeshModel *model, VBO *vbo)
{
	int i, j, k;
	float colorID[4];

	if (vbo->attribs[FACE] == NULL) {
		vbo->attribs[FACE] = (float*)calloc(vbo->numVertices * vbo->dataSize[FACE], sizeof(float));
	}

	// facet loop
	toColorID(model->id, colorID);
	for (i = 0; i < model->numFaces; ++i)
	{
		memcpy_s(vbo->attribs[FACE] + i * 12,
			vbo->numVertices * 4 * sizeof(float),
			colorID, 4 * sizeof(float));
		memcpy_s(vbo->attribs[FACE] + i * 12 + 4,
			vbo->numVertices * 4 * sizeof(float),
			colorID, 4 * sizeof(float));
		memcpy_s(vbo->attribs[FACE] + i * 12 + 8,
			vbo->numVertices * 4 * sizeof(float),
			colorID, 4 * sizeof(float));
	}
}

void setModelFaceID(MeshModel *model, VBO *vbo)
{
	int i, j, k, id;
	float colorID[4];

	if (vbo->attribs[FACE] == NULL) {
		vbo->attribs[FACE] = (float*)calloc(vbo->numVertices * vbo->dataSize[FACE], sizeof(float));
	}

	// facet loop
	id = 0;
	for (i = 0; i < model->numFaces; ++i)
	{
		toColorID(id, colorID);
		memcpy_s(vbo->attribs[FACE] + i * 12,
			vbo->numVertices * 4 * sizeof(float),
			colorID, 4 * sizeof(float));
		memcpy_s(vbo->attribs[FACE] + i * 12 + 4,
			vbo->numVertices * 4 * sizeof(float),
			colorID, 4 * sizeof(float));
		memcpy_s(vbo->attribs[FACE] + i * 12 + 8,
			vbo->numVertices * 4 * sizeof(float),
			colorID, 4 * sizeof(float));
		id++;
	}
}