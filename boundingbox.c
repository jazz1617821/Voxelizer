#include "boundingbox.h"
#include "mymath.h"
#include <stdio.h>
#include <stdlib.h>

AABB *newAABB(void)
{
	AABB *bbox;

	bbox = (AABB*)calloc(1, sizeof(AABB));
	initAABB(bbox);
	bbox->vbo = NULL;

	return bbox;
}

void deleteAABB(AABB **bbox)
{
	if ((*bbox)->vbo != NULL) {
		deleteVBO(&(*bbox)->vbo);
	}
	free(*bbox);
	*bbox = NULL;
}

void initAABB(AABB *bbox)
{
	bbox->max[0] = bbox->max[1] = bbox->max[2] = INT_MIN;
	bbox->min[0] = bbox->min[1] = bbox->min[2] = INT_MAX;
}

bool addBound(AABB *bbox, float val[3])
{
	bool isChanged = false;
	int i;

	for (i = 0; i < 3; ++i)
	{
		if (bbox->max[i] < val[i]) {
			bbox->max[i] = val[i];
			isChanged = true;
		}
		if (bbox->min[i] > val[i]) {
			bbox->min[i] = val[i];
			isChanged = true;
		}
	}

	return isChanged;
}

bool addAABB(AABB *des, AABB *src)
{
	bool isChanged = false;
	int i;

	for (i = 0; i < 3; ++i)
	{
		if (des->max[i] < src->max[i]) {
			des->max[i] = src->max[i];
			isChanged = true;
		}
		if (des->min[i] > src->min[i]) {
			des->min[i] = src->min[i];
			isChanged = true;
		}
	}

	return isChanged;
}

void setAABB(AABB *dest, AABB *src)
{
	int i;

	for (i = 0; i < 3; ++i)
	{
		dest->max[i] = src->max[i];
		dest->min[i] = src->min[i];
	}
}

void aabbCenter(AABB bbox, float center[3])
{
	center[0] = (bbox.max[0] + bbox.min[0]) / 2.0;
	center[1] = (bbox.max[1] + bbox.min[1]) / 2.0;
	center[2] = (bbox.max[2] + bbox.min[2]) / 2.0;
}

void aabbSize(AABB bbox, float size[3])
{
	size[0] = bbox.max[0] - bbox.min[0];
	size[1] = bbox.max[1] - bbox.min[1];
	size[2] = bbox.max[2] - bbox.min[2];
}

float aabbMaxSize(AABB bbox)
{
	float size[3];

	aabbSize(bbox, size);
	printf("size: %f %f %f\n", size[0], size[1], size[2]);
	printf("length: %f\n\n", length3fv(size));
	return length3fv(size);
}

void transformAABB(AABB *bbox, Transformation *tsfm)
{
	int i, j;
	float center[3], modelMat[16], transMat[16], scaleMat[16];


	aabbCenter(*bbox, center);
	transformMatrix(tsfm, modelMat, center);
	multMat4fv(modelMat, bbox->vbo->modelMat, modelMat);

	initAABB(bbox);
	for (i = 0; i < bbox->vbo->numVertices; ++i)
	{
		multMat4x3fv(modelMat, bbox->vbo->attribs[VERTEX] + i * 3, bbox->vbo->attribs[VERTEX] + i * 3);
		addBound(bbox, bbox->vbo->attribs[VERTEX] + i * 3);
	}
	identifyMat4fv(bbox->vbo->modelMat);
}

// create a bounding sphere data
BS *newBS(void)
{
	BS *bs;

	bs = (BS*)calloc(1, sizeof(BS*));

	return bs;
}

// delete a bounding sphere data
void deleteBS(BS **bs)
{
	if ((*bs)->vbo != NULL) {
		deleteVBO(&(*bs)->vbo);
	}
	free(*bs);
	*bs = NULL;
}

// initialize a bounding sphere data (vbo will not be initialized)
void initBS(BS *bs)
{
	bs->center[0] = bs->center[1] = bs->center[2] = 0;
	bs->radius = 0;
}

// transform a bounding sphere data
void transformBS(BS *bs, Transformation *tsfm)
{
	int i, j;
	float center[3], modelMat[16], transMat[16], scaleMat[16];


	// transform matrix
	transformMatrix(tsfm, modelMat, bs->center);
	multMat4fv(modelMat, bs->vbo->modelMat, modelMat);

	// calculate radius


	// transform vertex
	for (i = 0; i < bs->vbo->numVertices; ++i)
	{
		multMat4x3fv(modelMat, bs->vbo->attribs[VERTEX] + i * 3, bs->vbo->attribs[VERTEX] + i * 3);
	}
	identifyMat4fv(bs->vbo->modelMat);
}