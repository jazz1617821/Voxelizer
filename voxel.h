#ifndef _VOXEL_H
#define _VOXEL_H

#define VOX_X_SIZE 0
#define VOX_Y_SIZE 1
#define VOX_Z_SIZE 2
#define VOX_X_RESOLUTION 0
#define VOX_Y_RESOLUTION 1
#define VOX_Z_RESOLUTION 2

#define VOX_EMPTY 0
#define VOX_SURFACE 1
#define VOX_SOLID 2
#define VOX_IN_THICK 3
#define VOX_OUT_THICK 4

#define VOX_DATA_MAX_RES 256
#define VOX_DATA_MAX_SIZE (256 * 256 * 256)

#define VOX_NAME_SIZE 2048

#include "boundingbox.h"
#include "objecttree.h"
#include "distancefield.h"
#include "meshmodel.h"
#include <stdbool.h>
#include <stddef.h>


enum ORIGINAL_DATA_TYPE { OD_TYPE_NONE, OD_TYPE_MESH_MODEL, OD_TYPE_VOLUME, OD_TYPE_POINT_CLOUD };

typedef struct Voxel
{
	unsigned char data;
}Voxel;

typedef struct TriVoxel
{
	EnqDList item;
	int index;
}TriVoxel;

typedef struct DistanceField DistanceField;

typedef struct VoxelData
{
	unsigned short id;
	char name[VOX_NAME_SIZE];		//Data Name
	int resolution[3];				//Data resolution	
	float voxelSize[3];				//Data voxle size
	bool isBitCompress;				//Data store type
	Voxel *rawData;					//Raw data
	TriVoxel *lsArray;				// triangle list array
	AABB *bbox;

	// distance field
	DistanceField *df;

	// render staff
	VBO *vbo;

	// voxelize from
	int originalDataType;
	union originalData
	{
		MeshModel *model;
	};
}VoxelData;

typedef struct VoxelItem VoxelItem;

typedef struct VoxelObject
{
	ObjectTreeNode node;
	unsigned short id;
	char name[VOX_NAME_SIZE];				//Object name
	AABB *bbox;								//Axis Aligned Bounding Box
	int numberOfChild;						//Number of child
	VoxelItem *dataItem;					//
}VoxelObject;

typedef struct VoxelItem
{
	EnqDList link;						//Enqueueable double linked list 
	VoxelObject *parent;				//The leaf object which has this dataitem
	VoxelData *vdata;					//Voxel data pointer
}VoxelItem;

typedef struct VoxelModel
{
	char name[VOX_NAME_SIZE];			//Model name
	int resolution[3];					//Max Resolution
	float voxelSize[3];					//Unite all data to one voxel size
	int numberOfVoxelData;				//Raw data count

	VoxelObject *root;					//Root of the object tree
	VoxelItem *items;					//Root of the data item 
}VoxelModel;

#endif //_VOXEL_H