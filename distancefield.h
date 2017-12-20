#ifndef _DISTANCEFIELD_H
#define _DISTANCEFIELD_H

#include "voxel.h"
#include "enqlist.h"
#include <stdbool.h>

typedef enum { DF_LEFT, DF_RIGHT, DF_BOTTOM, DF_TOP, DF_BACK, DF_FRONT }
DF_BOUND;

typedef enum { DT_NONE, DT_CHAMFER, DT_CHESSBOARD, DT_CITYBLOCK, DT_EUCLIDEAN }
DT_TYPE;

typedef struct VoxelData VoxelData;
typedef struct VoxelObject VoxelObject;

// complete distance definition
// store the distance from the voxel center to the triangle 
typedef struct CDD {
	float dist;
	int tri;
}CDD;

// cdd list
// store all the distances from the voxel center to these triangles in descending order
typedef struct CDDList {
	EnqDList link;
	CDD cdd;
}CDDList;

typedef struct DistanceField
{
	int resolution[3];		// df resolution is different from the voxel data
	float voxelSize[3];		// df voxel size is the same as the voxel data
	int bound[6];			// df is bigger then the voxel data with bounds
	float *data;			// distance field data
	CDDList *lsArray;		// complete distacne definition list array
	int type;
	float minValue;			// mininum distance value
	float maxValue;			// maxinum distance value

	 // render staff
	AABB *bbox;
	VBO *vbo;
}DistanceField;

CDDList *newCDDList(int tri, float dist);
CDDList **newCDDListArray(int dfRes[3], float dfSize[3], int bound[6], VoxelData *vdata);
void deleteCDDListArray(CDDList **lsArray);
DistanceField *newDistanceField(int dfRes[3], float dfSize[3], int bound[6], DistanceField *reusage);
void deleteDistanceField(DistanceField **df);
void initDistanceField(DistanceField *df, VoxelObject *obj);
void updateDF(DistanceField *df, int center, int x, int y, int z, float delta);
void CDT(DistanceField *df, VoxelObject *vdata, int type);
float dataValue(int x, int y, int z, DistanceField *df);

/*
float pntTriDist(const float pnt[3], const float tri[9], float projPnt[3]);
CDDArray* newCDDArray(int size);
void deleteCDDArray(CDDArray* ca, int size);
void checkCDDArrayMem(CDDArray* ca);
bool checkCDDTriangles(CDDArray* ca, int tri);
bool addCdd(CDD cdd, CDDArray* ca);
void sortCDD(CDD *cdd, int size, int left, int right);
void swapCDD(CDD *a, CDD *b);
void cdfrDistanceTransform(voxelobj_t* obj, CDDArray* ca);*/

#endif // _DISTANCEFIELD_H
