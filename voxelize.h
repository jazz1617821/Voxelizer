#ifndef _VOXELIZE_H
#define _VOXELIZE_H

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
#define VOX_UNKNOWN 250

#include "voxel.h"
#include "meshmodel.h"
#include <stdio.h>
#include <stdbool.h>

// trivox
void clearTriVoxel(TriVoxel *trivox);
int addTriangleToTriVoxel(int index, TriVoxel *trivox);

// create voxel data
VoxelData *newVoxelData(void);
void deleteVoxelData(VoxelData **vdata);


// create data item
VoxelItem *newVoxelItemAnchor(void);
VoxelItem *newVoxelItem(VoxelData *vdata, VoxelObject *obj);
void clearVoxelItems(VoxelItem *items);
void deleteVoxelItem(VoxelItem **item);
void deleteVoxelItems(VoxelItem **items);
VoxelItem *firstVoxelItem(VoxelItem *items);
VoxelItem *lastVoxelItem(VoxelItem *items);
VoxelItem *nextVoxelItem(VoxelItem *item);
VoxelItem *prevVoxelItem(VoxelItem *item);
void pushFrontVoxelItem(VoxelItem *items, VoxelItem *item);
void pushBackVoxelItem(VoxelItem *items, VoxelItem *item);
VoxelItem *popFrontVoxelItem(VoxelItem *items);
VoxelItem *popBackVoxelItem(VoxelItem *items);
bool isAnchorVoxelItem(VoxelItem *items);
bool isEmptyVoxelItem(VoxelItem *items);


// create voxel object
VoxelObject *newVoxelObject(void);
void deleteVoxelObject(VoxelObject **obj);
void insertVoxelObject(VoxelObject *root, VoxelObject *obj);
void removeVoxelObject(VoxelObject *obj);
VoxelObject *parentOfVoxelObject(VoxelObject *obj);
VoxelObject *childOfVoxelObject(VoxelObject *obj);
VoxelObject *nextSiblingOfVoxelObject(VoxelObject *obj);
VoxelObject *prevSiblingOfVoxelObject(VoxelObject *obj);
Dequeue *bfVoxelObject(VoxelObject *obj);
Dequeue *dfVoxelObject(VoxelObject *obj);
Dequeue *firstNodesOfVoxelObject(VoxelObject *obj);
Dequeue *leafNodesOfVoxelObject(VoxelObject *obj);
int levelOfVoxelObject(VoxelObject *obj);
bool isRootVoxelObject(VoxelObject *obj);
bool isLeafVoxelObject(VoxelObject *obj);
void calVoxelObjectBBox(VoxelObject *root);
void updateVoxelObjectBBox(VoxelObject *obj);


// create voxel model
VoxelModel *newVoxelModel(void);
void deleteVoxelModel(VoxelModel **vmodel);
void delVoxelObjectFromScene(VoxelModel *sc, VoxelObject *obj);
void delVoxelItemFromScene(VoxelModel *sc, VoxelItem *item);
VoxelItem *searchVoxelData(int id, VoxelModel *vmodel);
void addVoxelModel(VoxelModel *oldVModel, VoxelModel *newVModel);


// mesh model voxelization
void modelVoxelizeInfo(MeshModelObject *modelobj, float in[3], float out[3]);
VoxelObject *modelVoxelize(MeshModel *model, float voxSize[3], bool isSolid);
VoxelModel *modelSceneVoxelize(MeshModelScene *scene, float voxSize[3], int voxRes[3], bool isSolid);

// file I/O
void openVM(const char *filepath, VoxelModel *vmodel, int type);
void readVO(const char *filepath, FILE *vmodelfile, VoxelModel *vmodel, int type);
VoxelData *openVD(const char *filepath);
void saveVM(const char *filepath, VoxelModel *vmodel, int type);
void writeVO(const char *filepath, FILE *vmodelfile, VoxelObject *root, int type);
void saveVD(const char *filepath, VoxelData *vdata);

/*
voa_t *newVoxelObjectArray();
void deleteVoxelObjectArray(voa_t **voa);

void addVoxelObject(voxelobj_t *obj, voa_t *voa);
voxelobj_t *baseVoxelize(MeshModel *model, float voxelSize[3]);
bool testWatertight(int x, int y, int z, voxel_t *vox);
void voxRegionGrow(int x, int y, int z, voxel_t *vox);
void solidifyVoxel(voxel_t *vox);
void solidifyVoxelObject(voxelobj_t *obj);

voxelobj_t *readVOFile(const char *fileName);
void writeVOFile(voxelobj_t *obj);
voxel_t *readBinaryVoxel(const char *fileName);
void writeBinaryVoxel(voxel_t *vox, const char *path);
int testRead(const char *fileName);
int voxIndex(voxel_t *vox, int x, int y, int z);
void countVoxels(voxel_t *vox);

*/

#endif //_VOXELIZE_H