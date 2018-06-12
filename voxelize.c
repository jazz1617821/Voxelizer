#include "voxelize.h"
#include "mymath.h"
#include "ray.h"
#include "stack.h"
#include "image3D.h"
#ifdef  _DEBUG
#include "image.h"
#endif //  _DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define X 0
#define Y 1
#define Z 2

static int g_vobjID = 0;
static int g_vdataID = 0;


// model voxelization function
static void findMinMax(float x0, float x1, float x2, float *valueMin, float *valueMax);
static int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3]);
static bool axis_x01(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v2[3],
	const float boxHS[3]);
static bool axis_x2(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v1[3],
	const float boxHS[3]);
static bool axis_y02(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v2[3],
	const float boxHS[3]);
static bool axis_y1(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v1[3],
	const float boxHS[3]);
static bool axis_z12(const float a, const float b,
	const float fa, const float fb,
	const float v1[3], const float v2[3],
	const float boxHS[3]);
static bool axis_z0(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v1[3],
	const float boxHS[3]);
static bool triBoxOverlap(float boxCenter[3], float boxHS[3], float v[9]);


// find the mininum and maxinum value
static void findMinMax(float v0, float v1, float v2, float *valueMin, float *valueMax)
{
	*valueMin = v0;
	*valueMax = v0;
	if (v1 < *valueMin) *valueMin = v1;
	if (v1 > *valueMax) *valueMax = v1;
	if (v2 < *valueMin) *valueMin = v2;
	if (v2 > *valueMax) *valueMax = v2;
}

static int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3])
{
	int i;
	float vmin[3], vmax[3], v;

	for (i = 0; i < 3; i++)
	{
		v = vert[i];
		if (normal[i] > 0.0f) {
			vmin[i] = -maxbox[i] - v;
			vmax[i] = maxbox[i] - v;
		}
		else {
			vmin[i] = maxbox[i] - v;
			vmax[i] = -maxbox[i] - v;
		}
	}
	precisionZero3fv(vmin, PRECISION4);
	precisionZero3fv(vmax, PRECISION4);
	if (dot3fv(normal, vmin) > 0.0f) return false;
	if (dot3fv(normal, vmax) >= 0.0f) return true;
	return false;
}

// x-tests

static bool axis_x01(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v2[3],
	const float boxHS[3])
{
	float p0, p2, max, min, rad;

	p0 = a * v0[Y] - b * v0[Z];
	p2 = a * v2[Y] - b * v2[Z];

	if (p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fa * boxHS[Y] + fb * boxHS[Z];
	if (precisionCompare(min, rad, PRECISION4) > 0 || precisionCompare(max, -rad, PRECISION4) < 0) // (min > rad || max < -rad)
		return false;
	return true;
}

static bool axis_x2(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v1[3],
	const float boxHS[3])
{
	float p0, p1, max, min, rad;

	p0 = a * v0[Y] - b * v0[Z];
	p1 = a * v1[Y] - b * v1[Z];

	if (p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fa * boxHS[Y] + fb * boxHS[Z];
	if (precisionCompare(min, rad, PRECISION4) > 0 || precisionCompare(max, -rad, PRECISION4) < 0) // (min > rad || max < -rad)
		return false;
	return true;
}

// y-tests

static bool axis_y02(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v2[3],
	const float boxHS[3])
{
	float p0, p2, max, min, rad;

	p0 = -a * v0[X] + b * v0[Z];
	p2 = -a * v2[X] + b * v2[Z];

	if (p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fa * boxHS[X] + fb * boxHS[Z];
	if (precisionCompare(min, rad, PRECISION4) > 0 || precisionCompare(max, -rad, PRECISION4) < 0) // (min > rad || max < -rad)
		return false;
	return true;
}

static bool axis_y1(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v1[3],
	const float boxHS[3])
{
	float p0, p1, max, min, rad;

	p0 = -a * v0[X] + b * v0[Z];
	p1 = -a * v1[X] + b * v1[Z];

	if (p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fa * boxHS[X] + fb * boxHS[Z];

	if (precisionCompare(min, rad, PRECISION4) > 0 || precisionCompare(max, -rad, PRECISION4) < 0) // (min > rad || max < -rad)
		return false;
	return true;
}

// z-tests

static bool axis_z12(const float a, const float b,
	const float fa, const float fb,
	const float v1[3], const float v2[3],
	const float boxHS[3])
{
	float p1, p2, max, min, rad;

	p1 = a * v1[X] - b * v1[Y];
	p2 = a * v2[X] - b * v2[Y];

	if (p2 < p1) {
		min = p2;
		max = p1;
	}
	else {
		min = p1;
		max = p2;
	}
	rad = fa * boxHS[X] + fb * boxHS[Y];
	if (precisionCompare(min, rad, PRECISION4) > 0 || precisionCompare(max, -rad, PRECISION4) < 0) // (min > rad || max < -rad)
		return false;
	return true;
}

static bool axis_z0(const float a, const float b,
	const float fa, const float fb,
	const float v0[3], const float v1[3],
	const float boxHS[3])
{
	float p0, p1, max, min, rad;

	p0 = a * v0[X] - b * v0[Y];
	p1 = a * v1[X] - b * v1[Y];

	if (p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fa * boxHS[X] + fb * boxHS[Y];
	if (precisionCompare(min, rad, PRECISION4) > 0 || precisionCompare(max, -rad, PRECISION4) < 0) // (min > rad || max < -rad)
		return false;
	return true;
}

bool triBoxOverlap(float boxCenter[3], float boxHS[3], float v[9])
{
	float v0[3];
	float v1[3];
	float v2[3];
	float e0[3];
	float e1[3];
	float e2[3];
	float min, max, fex, fey, fez;
	float normal[3];

	subtract3fv((v), boxCenter, v0);
	subtract3fv((v + 3), boxCenter, v1);
	subtract3fv((v + 6), boxCenter, v2);

	subtract3fv(v1, v0, e0);
	subtract3fv(v2, v1, e1);
	subtract3fv(v0, v2, e2);

	fex = fabs(e0[X]);
	fey = fabs(e0[Y]);
	fez = fabs(e0[Z]);

	// SAT
	if (!axis_x01(e0[Z], e0[Y], fez, fey, v0, v2, boxHS))
		return false;

	if (!axis_y02(e0[Z], e0[X], fez, fex, v0, v2, boxHS))
		return false;

	if (!axis_z12(e0[Y], e0[X], fey, fex, v1, v2, boxHS))
		return false;

	fex = fabs(e1[X]);
	fey = fabs(e1[Y]);
	fez = fabs(e1[Z]);

	if (!axis_x01(e1[Z], e1[Y], fez, fey, v0, v2, boxHS))
		return false;

	if (!axis_y02(e1[Z], e1[X], fez, fex, v0, v2, boxHS))
		return false;

	if (!axis_z0(e1[Y], e1[X], fey, fex, v0, v1, boxHS))
		return false;

	fex = fabs(e2[X]);
	fey = fabs(e2[Y]);
	fez = fabs(e2[Z]);

	if (!axis_x2(e2[Z], e2[Y], fez, fey, v0, v1, boxHS))
		return false;

	if (!axis_y1(e2[Z], e2[X], fez, fex, v0, v1, boxHS))
		return false;

	if (!axis_z12(e2[Y], e2[X], fey, fex, v1, v2, boxHS))
		return false;

	// test in X-direction
	findMinMax(v0[X], v1[X], v2[X], &min, &max);
	if (precisionCompare(min,  boxHS[X], PRECISION4) > 0 ||
		precisionCompare(max, -boxHS[X], PRECISION4) < 0)		// (min > boxHS[X] || max < -boxHS[X])
		return false;

	// test in Y-direction
	findMinMax(v0[Y], v1[Y], v2[Y], &min, &max);
	if (precisionCompare(min,  boxHS[Y], PRECISION4) > 0 ||
		precisionCompare(max, -boxHS[Y], PRECISION4) < 0)		// (min > boxHS[Y] || max < -boxHS[Y])
		return false;

	// test in Z-direction
	findMinMax(v0[Z], v1[Z], v2[Z], &min, &max);
	if (precisionCompare(min,  boxHS[Z], PRECISION4) > 0 ||
		precisionCompare(max, -boxHS[Z], PRECISION4) < 0)		// (min > boxHS[Z] || max < -boxHS[Z])
		return false;

	cross3fv(e0, e1, normal);
	normalize3fv(normal);
	precisionZero3fv(normal, PRECISION4);
	if (!planeBoxOverlap(normal, v0, boxHS)) 
		return false;

	return true;
}

void clearTriVoxel(TriVoxel *trivox)
{
	while (!isEmptyEnq(&trivox->item))
	{
		removeEnqTail(&trivox->item);
	}
}

int addTriangleToTriVoxel(int index, TriVoxel *trivox)
{
	TriVoxel *tmp, *newTrivox;

	tmp = (TriVoxel*)trivox->item.flink;
	while (tmp != trivox)
	{
		// duplicate
		if (tmp->index == index) {
			return 0;
		}
		tmp = (TriVoxel*)tmp->item.flink;
	}

	// add the triangle to the list
	newTrivox = (TriVoxel*)calloc(1, sizeof(TriVoxel));
	newTrivox->index = index;
	addEnqTail(&trivox->item, &newTrivox->item);

	return 1;
}


// create voxel data
VoxelData *newVoxelData(void)
{
	VoxelData *vdata;

	// allocate memory for vdata structure, and set the values to zero
	vdata = (VoxelData*)calloc(1, sizeof(VoxelData));
	vdata->bbox = newAABB();

	return vdata;
}

void deleteVoxelData(VoxelData **vdata)
{
	VoxelData *del;

	if (del == NULL) return;

	del = (*vdata);

	// free raw data
	if (del->rawData != NULL) {
		free(del->rawData);
	}
	if (del->df != NULL) {
		deleteDistanceField(&del->df);
	}
	if (del->vbo != NULL) {
		deleteVBO(&del->vbo);
	}
	/*
	// delete trivox
	if (del->lsArray != NULL) {

	}
	*/

	deleteAABB(&del->bbox);
	/*
	if (del->originalDataType == OD_TYPE_MESH_MODEL) {
		deleteModel(&del->model);
	}
	*/

	// set the pointer to null
	free(del);
	*vdata = NULL;
}

VoxelItem *newVoxelItemAnchor(void)
{
	VoxelItem *item;

	item = (VoxelItem*)calloc(1, sizeof(VoxelItem));
	initEnqDList(&item->link);
	item->parent = NULL;
	item->vdata = NULL;

	return item;
}

VoxelItem *newVoxelItem(VoxelData *vdata, VoxelObject *vobj)
{
	VoxelItem *item;

	item = (VoxelItem*)calloc(1, sizeof(VoxelItem));
	initEnqDList(&item->link);
	item->parent = vobj;
	item->vdata = vdata;

	return item;
}

void clearVoxelItems(VoxelItem *items)
{
	VoxelItem *delItem;

	if (items->vdata != NULL)
		return;

	// delete all items
	while (!isEmptyVoxelItem(items))
	{
		delItem = popBackVoxelItem(items);
		free(delItem);
	}

	// link to itself
	items->link.flink = &(items->link);
	items->link.blink = &(items->link);
}

void deleteVoxelItem(VoxelItem **item)
{
	if ((*item)->vdata == NULL) {
		if (isEmptyVoxelItem((*item))) {
			free(*item);
			*item = NULL;
		}
		return;
	}
	// link the left and right items
	(*item)->link.blink->flink = (*item)->link.flink;
	(*item)->link.flink->blink = (*item)->link.blink;
	free(*item);
	*item = NULL;
}

void deleteVoxelItems(VoxelItem **items)
{
	VoxelItem *delItem;

	if (items == NULL)
		return;

	clearVoxelItems(*items);
	free(*items);
	*items = NULL;
}

VoxelItem *firstVoxelItem(VoxelItem *items)
{
	if (items->vdata != NULL)
		return NULL;

	return (VoxelItem*)items->link.flink;
}

VoxelItem *lastVoxelItem(VoxelItem *items)
{
	if (items->vdata != NULL)
		return NULL;

	return (VoxelItem*)items->link.blink;
}

VoxelItem * nextVoxelItem(VoxelItem * item)
{
	return (VoxelItem*)item->link.flink;
}

VoxelItem * prevVoxelItem(VoxelItem * item)
{
	return (VoxelItem*)item->link.blink;
}

void pushFrontVoxelItem(VoxelItem *items, VoxelItem *item)
{
	if (items->vdata != NULL)
		return;

	addEnqHead(&item->link, &item->link);
}

void pushBackVoxelItem(VoxelItem *items, VoxelItem *item)
{
	if (items->vdata != NULL)
		return;

	addEnqTail(&items->link, &item->link);
}

VoxelItem *popFrontVoxelItem(VoxelItem *items)
{
	if (items->vdata != NULL)
		return NULL;

	return (VoxelItem*)removeEnqHead(&items->link);
}

VoxelItem *popBackVoxelItem(VoxelItem *items)
{
	if (items->vdata != NULL)
		return NULL;

	return (VoxelItem*)removeEnqTail(&items->link);
}

bool isAnchorVoxelItem(VoxelItem *items)
{
	return items->vdata == NULL ? true : false;
}

bool isEmptyVoxelItem(VoxelItem *items)
{
	return isEmptyEnq(&items->link);
}

// create voxel object
VoxelObject *newVoxelObject(void)
{
	VoxelObject *vobj;

	// allocate memory for vobj data structure, and set the values to zero
	vobj = (VoxelObject*)calloc(1, sizeof(VoxelObject));
	vobj->bbox = newAABB();

	return vobj;
}

// delete voxel object without recursion.
// record the firstChild in each level of the tree structure in an 1D array.
// delete all vobj from sibling list.
void deleteVoxelObject(VoxelObject **vobj)
{
	
}

void insertVoxelObject(VoxelObject *root, VoxelObject *obj)
{
	insertObjectTreeNode(&root->node, &obj->node);
}

void removeVoxelObject(VoxelObject *obj)
{
	removeObjectTreeNode(&obj->node);
}

VoxelObject *parentOfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)obj->node.parent;
}

VoxelObject *childOfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)obj->node.firstChild;
}

VoxelObject *nextSiblingOfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)obj->node.fsibling;
}

VoxelObject *prevSiblingOfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)obj->node.bsibling;
}

Dequeue *bfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)bfObjectTreeNode(&obj->node);
}

Dequeue *dfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)dfObjectTreeNode(&obj->node);
}

Dequeue *firstNodesOfVoxelObject(VoxelObject *obj)
{
	return firstNodesOfModelObject(&obj->node);
}

Dequeue *leafNodesOfVoxelObject(VoxelObject *obj)
{
	return leafNodesOfObjectTree(&obj->node);
}

int levelOfVoxelObject(VoxelObject *obj)
{
	return levelOfObjectTree(&obj->node);
}

bool isRootVoxelObject(VoxelObject *obj)
{
	return isRootObjectTreeNode(&obj->node);
}

bool isLeafVoxelObject(VoxelObject *obj)
{
	return isLeafObjectTreeNode(&obj->node);
}

void calVoxelObjectBBox(VoxelObject *root)
{
	VoxelObject *childObject, *parentObject, *shownParent;
	Dequeue *dequeue;

	// breadth-first inserted dequeue of object tree
	dequeue = bfObjectTreeNode(&root->node);

	// pop from back of deqeueu (leaf nodes first)
	shownParent = NULL;
	while (!isEmptyDequeue(dequeue))
	{
		childObject = (VoxelObject*)popBackDequeue(dequeue);

		parentObject = (VoxelObject*)childObject->node.parent;
		if (parentObject == NULL)
			break;

#ifdef _DEBUG
		printf("parentObject: %s\n", parentObject->name);
		printf("childObject: %s\n", childObject->name);
#endif
		// initialize parent node's bbox
		if (shownParent != parentObject) {
			shownParent = parentObject;
			initAABB(shownParent->bbox);
		}
		// re-calculate bounding box
		addAABB(shownParent->bbox, childObject->bbox);
#ifdef _DEBUG
		printf("update bbox\n");
		printf("min: %f %f %f\n", shownParent->bbox->min[X],
			shownParent->bbox->min[Y],
			shownParent->bbox->min[Z]);
		printf("max: %f %f %f\n", shownParent->bbox->max[X],
			shownParent->bbox->max[Y],
			shownParent->bbox->max[Z]);
#endif // _DEBUG

	}
	deleteDequeue(&dequeue);

#ifdef _DEBUG
	printf("calVoxelObjectBBox\n");
	printf("min: %f %f %f\n", root->bbox->min[X],
		root->bbox->min[Y],
		root->bbox->min[Z]);
	printf("max: %f %f %f\n", root->bbox->max[X],
		root->bbox->max[Y],
		root->bbox->max[Z]);
	printf("end\n");
#endif // !_DEBUG 

}

void updateVoxelObjectBBox(VoxelObject *obj)
{

}

// create voxel model
VoxelModel *newVoxelModel(void)
{
	VoxelModel *vmodel;

	// allocate memory for vmodel structure, and set the values to zero
	vmodel = (VoxelModel*)calloc(1, sizeof(VoxelModel));
	vmodel->items = newVoxelItemAnchor();
	vmodel->root = newVoxelObject();
	strcpy(vmodel->root->name, "root");

	return vmodel;
}

void deleteVoxelModel(VoxelModel **vmodel)
{
	if ((*vmodel) == NULL) return;

	if ((*vmodel)->root != NULL) {
		deleteVoxelObject(&(*vmodel)->root);
	}

	free(*vmodel);
	*vmodel = NULL;
}

void delVoxelObjectFromScene(VoxelModel *sc, VoxelObject *obj)
{
	Dequeue *leafNodes;
	VoxelObject *objNode;

	// add model object to the scene graph
	removeVoxelObject(obj);
	// re-calculate the bounding box of all objects
	updateVoxelObjectBBox(parentOfVoxelObject(obj));

	// add model to the item list
	leafNodes = leafNodesOfVoxelObject(obj);
	while (!isEmptyDequeue(leafNodes))
	{
		objNode = (VoxelObject*)popFrontDequeue(leafNodes);
		if (isLeafModelObject(objNode)) {
			delVoxelItemFromScene(sc, objNode->dataItem);
		}
	}
	deleteDequeue(&leafNodes);
	deleteVoxelObject(&obj);
}

void delVoxelItemFromScene(VoxelModel *sc, VoxelItem *item)
{
	VoxelItem *delItem;
	VoxelData *delData;

	if (item == NULL)
		return;

	delItem = firstVoxelItem(sc->items);
	while (delItem != item) {
		delItem = nextVoxelItem(delItem);
	}
	delData = delItem->vdata;

	// free model data
	// Must delete model item first
	// if delete the model, it will be regard as a anchor item
	// that will cause error link state
	deleteVoxelItem(&delItem);
	deleteVoxelData(&delData);

	sc->numberOfVoxelData--;
}

VoxelItem *searchVoxelData(int id, VoxelModel *vmodel)
{
	VoxelItem *item;

	item = firstVoxelItem(vmodel->items);
	while (item->vdata != NULL)
	{
		if (item->vdata->id == id)
			return item;
		item = nextVoxelItem(item);
	}
	return NULL;
}

void modelVoxelizeInfo(MeshModelObject *modelobj, float in[3], float out[3])
{
	int i, j, k, resolution[3], start[3], end[3], tmp, count;
	float boxCenter[3], halfSize[3], origin[3];
	AABB *bbox;

	if (modelobj == NULL)
		return;

	bbox = modelobj->bbox;

	// voxel resolution
	out[X] = ceil((bbox->max[X] - bbox->min[X]) / in[X]);
	out[Y] = ceil((bbox->max[Y] - bbox->min[Y]) / in[Y]);
	out[Z] = ceil((bbox->max[Z] - bbox->min[Z]) / in[Z]);
}

VoxelObject *modelVoxelize(MeshModel *model, float voxSize[3], bool isSolid)
{
	int i, j, k, l, ret, resolution[3], totalSize, index, start[3], end[3];
	float vertex[9], minBound[3], maxBound[3], boxCenter[3], boxHS[3], dir[3], oldProj[3], proj[3];
	VoxelObject *vobj;
	VoxelItem *ditem;
	VoxelData *vdata;
	TriVoxel *trivox, *newTrivox, *tmp, triangles;
	AABB *mAABB;
	bool isInner, isShot, checkState, isFirst;
	int num = 0;
	MeshModel *transModel;
	unsigned char *testImg;
	int imgIdx, imgSize;
	char path[2048], numStr[10];

	transModel = transformModel(model);
	// init
	vobj = newVoxelObject();
	strcpy(vobj->name, transModel->name);
	vdata = newVoxelData();
	vdata->model = model;
	strcpy(vdata->name, transModel->name);
	vdata->voxelSize[X] = voxSize[X];
	vdata->voxelSize[Y] = voxSize[Y];
	vdata->voxelSize[Z] = voxSize[Z];
	ditem = newVoxelItem(vdata, vobj);
	mAABB = transModel->bbox;
	vobj->dataItem = ditem;
	boxHS[X] = voxSize[X] / 2.0;
	boxHS[Y] = voxSize[Y] / 2.0;
	boxHS[Z] = voxSize[Z] / 2.0;

	// calculate the resolutions of voxel data
	vdata->resolution[X] = resolution[X] =
		ceil(fabs(mAABB->max[X] - mAABB->min[X]) / voxSize[X]);
	vdata->resolution[Y] = resolution[Y] =
		ceil(fabs(mAABB->max[Y] - mAABB->min[Y]) / voxSize[Y]);
	vdata->resolution[Z] = resolution[Z] =
		ceil(fabs(mAABB->max[Z] - mAABB->min[Z]) / voxSize[Z]);
	totalSize = resolution[X] * resolution[Y] * resolution[Z];

	vobj->bbox->min[X] = mAABB->min[X];
	vobj->bbox->min[Y] = mAABB->min[Y];
	vobj->bbox->min[Z] = mAABB->min[Z];
	vobj->bbox->max[X] = vobj->bbox->min[X] + vdata->resolution[X] * vdata->voxelSize[X];
	vobj->bbox->max[Y] = vobj->bbox->min[Y] + vdata->resolution[Y] * vdata->voxelSize[Y];
	vobj->bbox->max[Z] = vobj->bbox->min[Z] + vdata->resolution[Z] * vdata->voxelSize[Z];

	// <------------------------------------------------
	//  if voxel data need to split, then add codes here
	// ------------------------------------------------>

	// allocate voxel data
	vdata->rawData = (Voxel*)calloc(totalSize, sizeof(Voxel));
	// store ids of the triangles intersected with voxel to trivox
	if (isSolid) {
		trivox = (TriVoxel*)calloc(totalSize, sizeof(TriVoxel));
		// if index == -1  => anchor point
		for (i = 0; i < totalSize; ++i)
		{
			initEnqDList(&trivox[i].item);
			trivox[i].index = -1;
		}
	}

#ifdef _DEBUG
	printf("Voxelize %s...\n", transModel->name);
	printf("Voxel Res: %d %d %d\n", vdata->resolution[X], vdata->resolution[Y], vdata->resolution[Z]);
#endif

	// based-voxelization
	for (i = 0; i < transModel->numFaces; ++i)
	{
		getTriangle(transModel, i, vertex);
		minBound[X] = minBound[Y] = minBound[Z] = INT_MAX;
		maxBound[X] = maxBound[Y] = maxBound[Z] = INT_MIN;
		// find the bounding index of the triangle in the volume
		for (j = 0; j < 3; ++j)
		{
			for (k = 0; k < 3; ++k)
			{
				if (minBound[k] > vertex[j * 3 + k]) {
					minBound[k] = vertex[j * 3 + k];
				}
				if (maxBound[k] < vertex[j * 3 + k]) {
					maxBound[k] = vertex[j * 3 + k];
				}
			}
		}
		// bound of triangle
		start[X] = floor(fabs(minBound[X] - mAABB->min[X]) / voxSize[X]);
		start[Y] = floor(fabs(minBound[Y] - mAABB->min[Y]) / voxSize[Y]);
		start[Z] = floor(fabs(minBound[Z] - mAABB->min[Z]) / voxSize[Z]);
		end[X] = floor(fabs(maxBound[X] - mAABB->min[X]) / voxSize[X]);
		end[Y] = floor(fabs(maxBound[Y] - mAABB->min[Y]) / voxSize[Y]);
		end[Z] = floor(fabs(maxBound[Z] - mAABB->min[Z]) / voxSize[Z]);

		// check voxels in the bound if intersected with this triangle
		for (j = start[Z]; j <= end[Z]; ++j)
		{
			for (k = start[Y]; k <= end[Y]; ++k)
			{
				for (l = start[X]; l <= end[X]; ++l)
				{
					index = l + k * resolution[X] +
						j * resolution[X] * resolution[Y];
					
					boxCenter[X] = mAABB->min[X] + (l + 0.5) * voxSize[X];
					boxCenter[Y] = mAABB->min[Y] + (k + 0.5) * voxSize[Y];
					boxCenter[Z] = mAABB->min[Z] + (j + 0.5) * voxSize[Z];
					if (triBoxOverlap(boxCenter, boxHS, vertex)) {
						if (vdata->rawData[index].data == VOX_EMPTY) {
							vdata->rawData[index].data = VOX_SURFACE;
							num++;
						}
						if (isSolid) {
							newTrivox = (TriVoxel*)calloc(1, sizeof(TriVoxel));
							newTrivox->index = i; // face id
							addEnqTail(&trivox[index].item, &newTrivox->item);
						}
					}
				} // end x loop
			} // end y loop
		} // end z loop
	}
#ifdef _DEBUG
	printf("Number of voxels: %d\n", num);
	printf("End Voxelization\n");
#endif
	if (isSolid) {
#ifdef _DEBUG
		printf("Solidify...\n");
#endif // _DEBUG
		// solidify
		
		// shoot rays from x-direction to volume
		dir[X] = 1.0;
		dir[Y] = 0.0;
		dir[Z] = 0.0;
		for (i = 0; i < resolution[Z]; ++i)
		{
			for (j = 0; j < resolution[Y]; ++j)
			{
				// avoid the duplicate triangle in each rays
				initEnqDList(&triangles.item);
				triangles.index = -1;
				// set initial state
				isInner = false;
				for (k = 0; k < resolution[X]; ++k)
				{
					index = k + j * resolution[X] + i * resolution[X] * resolution[Y];
					tmp = (TriVoxel*)enqHead(&trivox[index].item);
					boxCenter[X] = mAABB->min[X];
					boxCenter[Y] = mAABB->min[Y] + (j + 0.5) * voxSize[Y];
					boxCenter[Z] = mAABB->min[Z] + (i + 0.5) * voxSize[Z];
					// check first intersected
					isFirst = true;
					// test ray and triangles
					while (tmp != &trivox[index])
					{
						// add the index of triangle to the list, and check if a duplicate triangle
						ret = addTriangleToTriVoxel(tmp->index, &triangles);
						// duplicate
						if (ret == 0) {
							tmp = (TriVoxel*)tmp->item.flink;
							continue;
						}
						getTriangle(transModel, tmp->index, vertex);
						isShot = shootTriangle(boxCenter, dir, vertex, proj);
						if(isShot)
							isInner = !isInner;
						// record the intersected point
						if (isShot && isFirst) {
							oldProj[X] = proj[X];
							oldProj[Y] = proj[Y];
							oldProj[Z] = proj[Z];
							checkState = isInner;
							isFirst = false;
						}
						else if(isShot && !precisionCompare3fv(oldProj, proj, PRECISION4)) {
							isInner = checkState;
						}
						else {
							checkState = isInner;
						}
						tmp = (TriVoxel*)tmp->item.flink;
					} // end while
					if (isInner){
						if (vdata->rawData[index].data == VOX_EMPTY) {
							vdata->rawData[index].data = VOX_SOLID;
							num++;
						}
					}
				} // end x loop
				 // clear list
				clearTriVoxel(&triangles);
			} // end y loop
		} // end z loop
		// end x-direction
		
		// shoot rays from y-direction to volume
		dir[X] = 0.0;
		dir[Y] = 1.0;
		dir[Z] = 0.0;
		for (i = 0; i < resolution[Z]; ++i)
		{
			for (j = 0; j < resolution[X]; ++j)
			{
				// avoid the duplicate triangle in each rays
				initEnqDList(&triangles.item);
				triangles.index = -1;
				// set initial state
				isInner = false;
				for (k = 0; k < resolution[Y]; ++k)
				{
					index = j + k * resolution[X] + i * resolution[X] * resolution[Y];
					tmp = (TriVoxel*)enqHead(&trivox[index].item);
					boxCenter[X] = mAABB->min[X] + (j + 0.5) * voxSize[X];
					boxCenter[Y] = mAABB->min[Y];
					boxCenter[Z] = mAABB->min[Z] + (i + 0.5) * voxSize[Z];
					// check first intersected
					isFirst = true;
					// test ray and triangles
					while (tmp != &trivox[index])
					{
						// add the index of triangle to the list, and check if a duplicate triangle
						ret = addTriangleToTriVoxel(tmp->index, &triangles);
						// duplicate
						if (ret == 0) {
							tmp = (TriVoxel*)tmp->item.flink;
							continue;
						}
						getTriangle(transModel, tmp->index, vertex);
						isShot = shootTriangle(boxCenter, dir, vertex, proj);
						if (isShot)
							isInner = !isInner;
						// record the intersected point
						if (isShot && isFirst) {
							oldProj[X] = proj[X];
							oldProj[Y] = proj[Y];
							oldProj[Z] = proj[Z];
							checkState = isInner;
							isFirst = false;
						}
						else if (isShot && !precisionCompare3fv(oldProj, proj, PRECISION4)) {
							isInner = checkState;
						}
						else {
							checkState = isInner;
						}
						tmp = (TriVoxel*)tmp->item.flink;
					} // end while
					// clear the redundant voxels
					if (!isInner) {
						if (vdata->rawData[index].data == VOX_SOLID) {
							vdata->rawData[index].data = VOX_EMPTY;
							num--;
						}
					}
				} // end x loop
				  // clear list
				clearTriVoxel(&triangles);
			} // end y loop
		} // end z loop
		// end y-direction

		// shoot rays from z-direction to volume
		dir[X] = 0.0;
		dir[Y] = 0.0;
		dir[Z] = 1.0;
		for (i = 0; i < resolution[X]; ++i)
		{
			for (j = 0; j < resolution[Y]; ++j)
			{
				// avoid the duplicate triangle in each rays
				initEnqDList(&triangles.item);
				triangles.index = -1;
				// set initial state
				isInner = false;
				for (k = 0; k < resolution[Z]; ++k)
				{
					index = i + j * resolution[X] + k * resolution[X] * resolution[Y];
					tmp = (TriVoxel*)enqHead(&trivox[index].item);
					boxCenter[X] = mAABB->min[X] + (i + 0.5) * voxSize[X];
					boxCenter[Y] = mAABB->min[Y] + (j + 0.5) * voxSize[Y];
					boxCenter[Z] = mAABB->min[Z];
					// check first intersected
					isFirst = true;
					// test ray and triangles
					while (tmp != &trivox[index])
					{
						// add the index of triangle to the list, and check if a duplicate triangle
						ret = addTriangleToTriVoxel(tmp->index, &triangles);
						// duplicate
						if (ret == 0) {
							tmp = (TriVoxel*)tmp->item.flink;
							continue;
						}
						getTriangle(transModel, tmp->index, vertex);
						isShot = shootTriangle(boxCenter, dir, vertex, proj);
						if (isShot)
							isInner = !isInner;
						// record the intersected point
						if (isShot && isFirst) {
							oldProj[X] = proj[X];
							oldProj[Y] = proj[Y];
							oldProj[Z] = proj[Z];
							checkState = isInner;
							isFirst = false;
						}
						else if (isShot && !precisionCompare3fv(oldProj, proj, PRECISION4)) {
							isInner = checkState;
						}
						else {
							checkState = isInner;
						}
						tmp = (TriVoxel*)tmp->item.flink;
					} // end while
					  // clear the redundant voxels
					if (!isInner) {
						if (vdata->rawData[index].data == VOX_SOLID) {
							vdata->rawData[index].data = VOX_EMPTY;
							num--;
						}
					}
				} // end x loop
				  // clear list
				clearTriVoxel(&triangles);
			} // end x loop
		} // end y loop
		  // end z-direction

		  // shoot rays from reverse x-direction to volume
		dir[X] = -1.0;
		dir[Y] = 0.0;
		dir[Z] = 0.0;
		for (i = 0; i > resolution[Z]; ++i)
		{
			for (j = 0; j < resolution[Y]; ++j)
			{
				// avoid the duplicate triangle in each rays
				initEnqDList(&triangles.item);
				triangles.index = -1;
				// set initial state
				isInner = false;
				for (k = resolution[X]-1; k >= 0; --k)
				{
					index = i + j * resolution[Z] + k * resolution[Z] * resolution[Y];
					tmp = (TriVoxel*)enqHead(&trivox[index].item);
					boxCenter[X] = mAABB->min[X];
					boxCenter[Y] = mAABB->min[Y] + (j + 0.5) * voxSize[Y];
					boxCenter[Z] = mAABB->min[Z] + (i + 0.5) * voxSize[Z];
					// check first intersected
					isFirst = true;
					// test ray and triangles
					while (tmp != &trivox[index])
					{
						// add the index of triangle to the list, and check if a duplicate triangle
						ret = addTriangleToTriVoxel(tmp->index, &triangles);
						// duplicate
						if (ret == 0) {
							tmp = (TriVoxel*)tmp->item.flink;
							continue;
						}
						getTriangle(transModel, tmp->index, vertex);
						isShot = shootTriangle(boxCenter, dir, vertex, proj);
						if (isShot)
							isInner = !isInner;
						// record the intersected point
						if (isShot && isFirst) {
							oldProj[X] = proj[X];
							oldProj[Y] = proj[Y];
							oldProj[Z] = proj[Z];
							checkState = isInner;
							isFirst = false;
						}
						else if (isShot && !precisionCompare3fv(oldProj, proj, PRECISION4)) {
							isInner = checkState;
						}
						else {
							checkState = isInner;
						}
						tmp = (TriVoxel*)tmp->item.flink;
					} // end while
					  // clear the redundant voxels
					if (!isInner) {
						if (vdata->rawData[index].data == VOX_SOLID) {
							vdata->rawData[index].data = VOX_EMPTY;
							num--;
						}
					}
				} // end x loop
				  // clear list
				clearTriVoxel(&triangles);
			} // end x loop
		} // end y loop
		  // end reverse x-direction
		
		// dilate solid
		voxelDilationCity3(vdata, VOX_SOLID);

#ifdef _DEBUG
		printf("Number of voxels: %d\n", num);
		printf("End Solidify\n");
#endif
		vdata->lsArray = trivox;
	} // end solidify


// debug bmp
#ifdef _DEBUG
	/*imgSize = resolution[X] * resolution[Z];
	testImg = (unsigned char*)calloc(imgSize, sizeof(unsigned char));

	printf("Write voxel data to bmp file\n");
	num = 0;
	for (i = 0; i < resolution[Y]; ++i)
	{
		for (j = 0; j < resolution[Z]; ++j)
		{
			for (k = 0; k < resolution[X]; ++k)
			{
				index = k + i * resolution[X] + j * resolution[X] * resolution[Y];
				imgIdx = k + j * resolution[X];
				if (vdata->rawData[index].data != VOX_EMPTY)
					testImg[imgIdx] = 255;
			}
		}
		strcpy(path, "Bmp\\Test");
		sprintf(numStr, "%d", num++);
		strcat(path, numStr);
		strcat(path, ".bmp");
		saveBMP(path, resolution[X], resolution[Z], testImg);
		memset(testImg, 0, imgSize);
	}
	printf("Done...\n");*/
#endif // _DEBUG


	return vobj;
}

VoxelModel *modelSceneVoxelize(MeshModelScene *scene, float voxSize[3], int voxRes[3], bool isSolid)
{
	int numOfVoxelData, numOfChild;
	MeshModel *model;
	MeshModelItem *modelItem;
	MeshModelObject *modelParent, *modelChild;
	VoxelObject *root, *voxelParent, *voxelChild;
	VoxelModel *vmodel;
	Dequeue *mobjDequeue, *vobjDequeue;
	
	// create a voxel model
	vmodel = newVoxelModel();
	strcpy(vmodel->name, "vmodel");
	vmodel->resolution[X] = voxRes[X];
	vmodel->resolution[Y] = voxRes[Y];
	vmodel->resolution[Z] = voxRes[Z];
	vmodel->voxelSize[X] = voxSize[X];
	vmodel->voxelSize[Y] = voxSize[Y];
	vmodel->voxelSize[Z] = voxSize[Z];

	// build voxel object tree and voxelize each models
	mobjDequeue = bfObjectTreeNode(&scene->root->node);
	vobjDequeue = newDequeue();
	
	// create root object and push to a dequeue
	root = newVoxelObject();
	strcpy(root->name, scene->root->name);
	pushBackDequeue(vobjDequeue, root);
	numOfVoxelData = 0;
	while (!isEmptyDequeue(mobjDequeue))
	{
		// get a parent node from a dequeue
		modelParent = (MeshModelObject*)popFrontDequeue(mobjDequeue);
		voxelParent = (VoxelObject*)popFrontDequeue(vobjDequeue);
		if (modelParent == NULL)
			break;
		// create voxel objects (children)
		modelChild = childOfModelObject(modelParent);
		numOfChild = 0;
		while (modelChild != NULL)
		{
			// voxelize the model and return data items
			if (isLeafModelObject(modelChild)) {
				modelItem = modelChild->item;
				model = modelItem->model;
				voxelChild = modelVoxelize(model, voxSize, isSolid);
				insertVoxelObject(voxelParent, voxelChild);
				pushBackDequeue(vobjDequeue, voxelChild);
				pushBackVoxelItem(vmodel->items, voxelChild->dataItem);

				numOfVoxelData++;
			}
			modelChild = nextSiblingOfModelObject(modelChild);
			numOfChild++;
		}
		voxelParent->numberOfChild = numOfChild;
	}
	deleteDequeue(&mobjDequeue);
	deleteDequeue(&vobjDequeue);

	calVoxelObjectBBox(root);
	vmodel->root = root;
	vmodel->numberOfVoxelData = numOfVoxelData;

	return vmodel;
}

void openVM(const char *filepath, VoxelModel *vmodel, int type)
{
	FILE *fp;
	VoxelObject *node;

	if (vmodel == NULL) {
#ifdef _DEBUG
		fprintf(stderr, "Error in openVM()...\nvmodel == null\n");
#endif // _DEBUG
		exit(1);
	}

#ifdef _DEBUG
	printf("Load start...\n");
#endif	// _DEBUG

	// start file
	fp = fopen(filepath, "r");

	// write the voxel model file
	fscanf(fp, "Voxel Model:\n");
	fscanf(fp, "Name:%s\n", vmodel->name);
	fscanf(fp, "Resolution:%dx%dx%d\n", &vmodel->resolution[X], &vmodel->resolution[Y], &vmodel->resolution[Z]);
	fscanf(fp, "Voxelsize:%f:%f:%f\n", &vmodel->voxelSize[X], &vmodel->voxelSize[Y], &vmodel->voxelSize[Z]);
	fscanf(fp, "Number of voxel data:%d\n", &vmodel->numberOfVoxelData);
	readVO(filepath, fp, vmodel, type);
	calVoxelObjectBBox(vmodel->root);
	fclose(fp);
	// end file

#ifdef _DEBUG
	printf("Load done...\n");
#endif	// !_DEBUG
}

void readVO(const char *filepath, FILE *vmodelfile, VoxelModel *vmodel, int type)
{
	int i;
	Dequeue *dequeue;
	VoxelObject *root, *parentNode, *childNode;
	VoxelItem *item;
	VoxelData *vdata;
	char vdataName[1024] = { '\0' }, path[2048] = { '\0' }, *ptr;

	// initial (root)
	root = vmodel->root;
	fscanf(vmodelfile, "Name:%s\n", root->name);
	fscanf(vmodelfile, "Max bound:%fx%fx%f\n", &root->bbox->max[X], &root->bbox->max[Y], &root->bbox->max[Z]);
	fscanf(vmodelfile, "Min bound:%fx%fx%f\n", &root->bbox->min[X], &root->bbox->min[Y], &root->bbox->min[Z]);
	fscanf(vmodelfile, "Number of child:%d\n", &root->numberOfChild);

	dequeue = newDequeue();
	// push to the dequeue
	for (i = 0; i < root->numberOfChild; ++i)
	{
		pushBackDequeue(dequeue, root);
	}
	// loops
	while (!isEmptyDequeue(dequeue))
	{
		if (type == 0) {
			parentNode = popBackDequeue(dequeue);
		}
		else if (type == 1) {
			parentNode = popFrontDequeue(dequeue);
		}
		// create new node
		childNode = newVoxelObject();
		childNode->id = g_vobjID++;
		fscanf(vmodelfile, "Name:%s\n", childNode->name);
		fscanf(vmodelfile, "Max bound:%fx%fx%f\n", &childNode->bbox->max[X], &childNode->bbox->max[Y], &childNode->bbox->max[Z]);
		fscanf(vmodelfile, "Min bound:%fx%fx%f\n", &childNode->bbox->min[X], &childNode->bbox->min[Y], &childNode->bbox->min[Z]);
		fscanf(vmodelfile, "Number of child:%d\n", &childNode->numberOfChild);
		insertVoxelObject(root, childNode);
		// push the node to the dequeue for [numberOfChild] times
		for (i = 0; i < childNode->numberOfChild; ++i)
		{
			pushBackDequeue(dequeue, childNode);
		}
		// leaf node
		if (childNode->numberOfChild == 0) {
			fscanf(vmodelfile, "Voxel data filename:%s\n", vdataName);
			ptr = strrchr(filepath, '/');
			ptr = ptr + 1;
			strncpy(path, filepath, ptr - filepath);
			strcat(path, vdataName);
			strcat(path, ".vdat");
			vdata = openVD(path);
			setAABB(vdata->bbox, childNode->bbox);
			vdata->id = g_vdataID++;
			item = newVoxelItem(vdata, childNode);
			childNode->dataItem = item;
			// push item into voxel model
			pushBackVoxelItem(vmodel->items, item);
		}
	}

	deleteDequeue(&dequeue);
	return root;
}

VoxelData *openVD(const char *filepath)
{
	char vdataName[1024], *ptr;
	int total, ret;
	VoxelData *vdata;
	FILE *fp;

	fp = fopen(filepath, "rb");

	vdata = newVoxelData();
	
	// voxel data name
	ptr = strrchr(filepath, '/');
	ptr = ptr + 1;
	strcpy(vdataName, filepath + (ptr - filepath));
	ptr = strstr(vdataName, ".vdat");
	vdataName[ptr - vdataName] = '\0';
	strcpy(vdata->name, vdataName);

	ret = fread(vdata->resolution, sizeof(int), 3, fp);
	total = vdata->resolution[X] * vdata->resolution[Y] * vdata->resolution[Z];
	// allocate memory for raw data
	vdata->rawData = (Voxel*)calloc(total, sizeof(Voxel));
	ret = fread(vdata->voxelSize, sizeof(float), 3, fp);
	ret = fread(&vdata->isBitCompress, sizeof(int), 1, fp);
	ret = fread(vdata->rawData, sizeof(Voxel), total, fp);
	if (ret != total) {
		printf("Error in openVD...\nfread != total\n");
	}
	
	fclose(fp);


	return vdata;
}

void saveVM(const char *filepath, VoxelModel *vmodel, int type)
{
	FILE *fp;
	
	if (vmodel == NULL) return;

	fp = fopen(filepath, "w");

	// write the voxel model file
	fprintf(fp, "Voxel Model:\n");
	fprintf(fp, "Name:%s\n", vmodel->name);
	fprintf(fp, "Resolution:%dx%dx%d\n", vmodel->resolution[0], vmodel->resolution[1], vmodel->resolution[2]);
	fprintf(fp, "Voxelsize:%.2f:%.2f:%.2f\n", vmodel->voxelSize[0], vmodel->voxelSize[1], vmodel->voxelSize[2]);
	fprintf(fp, "Number of voxel data:%d\n", vmodel->numberOfVoxelData);
	writeVO(filepath, fp, vmodel->root, type);

	fclose(fp);
}

void writeVO(const char *filepath, FILE *vmodelfile, VoxelObject *root, int type)
{
	VoxelObject *node;
	Dequeue *dequeue;

	if (type == 0) {
		dequeue = dfVoxelObject(root);
	}
	else if (type == 1) {
		dequeue = bfVoxelObject(root);
	}
	
	while (!isEmptyDequeue(dequeue))
	{
		node = (VoxelObject*)popFrontDequeue(dequeue);
		// write the voxel object file
		fprintf(vmodelfile, "Name:%s\n", node->name);
		fprintf(vmodelfile, "Max bound:%fx%fx%f\n", node->bbox->max[X], node->bbox->max[Y], node->bbox->max[Z]);
		fprintf(vmodelfile, "Min bound:%fx%fx%f\n", node->bbox->min[X], node->bbox->min[Y], node->bbox->min[Z]);
		fprintf(vmodelfile, "Number of child:%d\n", node->numberOfChild);

		// if the node is a leaf node, then save voxel data
		if (isLeafVoxelObject(node)) {
			fprintf(vmodelfile, "Voxel data filename:%s\n", node->dataItem->vdata->name);
			saveVD(filepath, node->dataItem->vdata);
		}
	}

	deleteDequeue(&dequeue);
}

void saveVD(const char *filepath, VoxelData *vdata)
{
	int total;
	char *str = (char*)malloc(sizeof(char) * (strlen(filepath) + strlen(vdata->name) + 5));
	strcpy(str, filepath);
	char *i = strrchr(str, '/');
	str[i - str + 1] = '\0';
	strcat(str, vdata->name);
	strcat(str, ".vdat");
	printf("%s\n", str);
	FILE *fp = fopen(str, "wb");

	total = vdata->resolution[X] * vdata->resolution[Y] * vdata->resolution[Z];
	if (fp) {
		fwrite(vdata->resolution, sizeof(int), 3, fp);
		fwrite(vdata->voxelSize, sizeof(float), 3, fp);
		fwrite(&vdata->isBitCompress, sizeof(int), 1, fp);
		fwrite(vdata->rawData, sizeof(Voxel), total, fp);
		printf("Finish write %s\n", vdata->name);
		fclose(fp);
	}
	free(str);
}

void addVoxelModel(VoxelModel *oldVModel, VoxelModel *newVModel)
{

}

/*
VoxelModel *openVM(const char *filepath)
{
	VoxelModel *vmodel;

	FILE *fp = fopen(filepath, "w");

	vmodel = newVoxelModel();
	
	// read vmodel info
	fscanf(fp, "Voxel Model:\n");
	fscanf(fp, "Name:%s\n", vmodel->name);
	fscanf(fp, "Resolution:%dx%dx%d\n", &vmodel->resolution[X], &vmodel->resolution[Y], &vmodel->resolution[Z]);
	fscanf(fp, "Voxelsize:%.2f:%.2f:%.2f\n", &vmodel->voxelSize[X], &vmodel->voxelSize[Y], &vmodel->voxelSize[Z]);
	fscanf(fp, "Number of voxel data:%d\n", &vmodel->numberOfVoxelData);

	//DFS read voxel object

	fclose(fp);

	return vmodel;
}

void addVoxelModel(VoxelModel *oldVModel, VoxelModel *newVModel);
{

}


void writeVO(const char *filepath, FILE *vmodelfile, VoxelObject *vobj)
{
	if (isLeafVoxelObject(vobj)) {
		fprintf(vmodelfile, "Voxel data filename:%s\n", vobj->dataItem->vdata->name);
		writeVD(filepath, vobj->dataItem->vdata);
		return;
	}
	else {
		//Write this object
		fprintf(vmodelfile, "Name:%s\n", vobj->name);
		fprintf(vmodelfile, "Max bound:%fx%fx%f\n", vobj->bbox->max[X], vobj->bbox->max[Y], vobj->bbox->max[Z]);
		fprintf(vmodelfile, "Min bound:%fx%fx%f\n", vobj->bbox->min[X], vobj->bbox->min[Y], vobj->bbox->min[Z]);
		fprintf(vmodelfile, "Number of child:%d\n", vobj->numberOfChild);
		VoxelObject* voxelObjPtr = childOfVoxelObject(vobj);
		while (voxelObjPtr != NULL) {
			writeVO(filepath, vmodelfile, voxelObjPtr);
			voxelObjPtr = nextSiblingOfVoxelObject(voxelObjPtr);
		}
	}
}

void writeVD(const char *filepath, VoxelData *vdata)
{
	int total;
	char *str = (char*)malloc(sizeof(char) * (strlen(filepath) + strlen(vdata->name) + 5));
	strcpy(str, filepath);
	char *i = strrchr(str, '/');
	str[i - str + 1] = '\0';
	strcat(str, vdata->name);
	strcat(str, ".vdat");
	printf("%s\n", str);
	FILE *vdatafile = fopen(str, "wb");

	total = vdata->resolution[X] * vdata->resolution[Y] * vdata->resolution[Z];
	if (vdatafile) {
		fwrite(vdata->resolution, sizeof(int), 3, vdatafile);
		fwrite(vdata->voxelSize, sizeof(float), 3, vdatafile);
		fwrite(&vdata->isBitCompress, sizeof(int), 1, vdatafile);
		fwrite(&vdata->rawData, sizeof(Voxel), total, vdatafile);
		printf("Finish write %s\n", vdata->name);
		fclose(vdatafile);
	}

	free(str);

	return;
}
*/

/*

/////////////////////////////
//     gloabal variable    //
/////////////////////////////
int g_voxelBvhID = 0;
int g_voxelID = 0;

int g_bound = 2;


// x->y->z increament
const int g_nbMat[26][3] =
{
	{ -1, -1, -1 },{ 0, -1, -1 },{ 1, -1, -1 },
	{ -1,  0, -1 },{ 0,  0, -1 },{ 1,  0, -1 },
	{ -1,  1, -1 },{ 0,  1, -1 },{ 1,  1, -1 },
	{ -1, -1,  0 },{ 0, -1,  0 },{ 1, -1,  0 },
	{ -1,  0,  0 },{ 1,  0,  0 },
	{ -1,  1,  0 },{ 0,  1,  0 },{ 1,  1,  0 },
	{ -1, -1,  1 },{ 0, -1,  1 },{ 1, -1,  1 },
	{ -1,  0,  1 },{ 0,  0,  1 },{ 1,  0,  1 },
	{ -1,  1,  1 },{ 0,  1,  1 },{ 1,  1,  1 }
};


const int g_cityMat[6][3] =
{
	{ 0,  0, -1 },
	{ 0, -1,  0 },
	{ -1,  0,  0 },
	{ 1,  0,  0 },
	{ 0,  1,  0 },
	{ 0,  0,  1 }
};


static Voxel *modelVoxelize(MeshModel *model, float origin[3], float voxelSize[3])
{
	int i, j, k, l, m, n, o, index, resolution[3];
	float boxCenter[3], boxHalf[3], triVert[9], minBound[3], maxBound[3], start[3], end[3], values[3];
	Voxel *voxel;

	// initialize
	voxel = (Voxel*)calloc(1, sizeof(Voxel));
	memcpy(voxel->voxelSize, voxelSize, 3 * sizeof(float));
	for (i = 0; i < 3; ++i)
	{
		voxel->minBound[i] = ceil((model->bbox->min[i] - origin[i]) / voxelSize[i]) - g_bound;
		voxel->maxBound[i] = ceil((model->bbox->max[i] - origin[i]) / voxelSize[i]) + g_bound;
		voxel->resolution[i] = voxel->maxBound[i] - voxel->minBound[i];
		resolution[i] = voxel->resolution[i];

		boxHalf[i] = voxelSize[i] / 2.0;
	}
	voxel->data = (unsigned char*)calloc(resolution[0] *
		resolution[1] *
		resolution[2],
		sizeof(unsigned char));

	for (i = 0; i < 3; ++i)
	{
		voxel->bbox.max[i] = (model->bbox->max[i] + model->bbox->min[i] + resolution[i] * voxelSize[i]) / 2.0;
		voxel->bbox.min[i] = (model->bbox->max[i] + model->bbox->min[i] - resolution[i] * voxelSize[i]) / 2.0;
	}

	voxel->numVoxels = 0;
	for (i = 0; i < model->numFaces; ++i)
	{
		getTriangle(model, i, triVert);
		minBound[X] = minBound[Y] = minBound[Z] = INT_MAX;
		maxBound[X] = maxBound[Y] = maxBound[Z] = INT_MIN;
		// find bound of triangle
		for (j = 0; j < 3; ++j)
		{
			for (k = 0; k < 3; ++k)
			{
				if (minBound[k] > triVert[j * 3 + k]) {
					minBound[k] = triVert[j * 3 + k];
				}
				if (maxBound[k] < triVert[j * 3 + k]) {
					maxBound[k] = triVert[j * 3 + k];
				}
			}
		}

		start[0] = floor(fabs(minBound[0] - origin[0]) / voxelSize[X]);
		start[1] = floor(fabs(minBound[1] - origin[1]) / voxelSize[Y]);
		start[2] = floor(fabs(minBound[2] - origin[2]) / voxelSize[Z]);
		end[0] = ceil(fabs(maxBound[0] - origin[0]) / voxelSize[X]);
		end[1] = ceil(fabs(maxBound[1] - origin[1]) / voxelSize[Y]);
		end[2] = ceil(fabs(maxBound[2] - origin[2]) / voxelSize[Z]);

		// voxelize
		for (j = start[0]; j <= end[0]; ++j)
		{
			for (k = start[1]; k <= end[1]; ++k)
			{
				for (l = start[2]; l <= end[2]; ++l)
				{
					index = j + k * resolution[X] + l * resolution[X] * resolution[Y];
					boxCenter[0] = origin[0] + (j + 0.5) * voxelSize[X];
					boxCenter[1] = origin[1] + (k + 0.5) * voxelSize[Y];
					boxCenter[2] = origin[2] + (l + 0.5) * voxelSize[Z];
					if (triBoxOverlap(boxCenter, boxHalf, triVert)) {
						if (voxel->data[index] == 0) {
							voxel->data[index] = 1;
							voxel->numVoxels++;
						}
					} // tri box overlap testing
				} // end loop l
			} // end loop k
		} // end loop j
	} // end triangle loop

	return voxel;
}


voxelobj_t *baseVoxelize(MeshModel *model, float voxelSize[3])
{
	int i, j, k, resolution[3], start[3], end[3], tmp, count;
	float boxCenter[3], halfSize[3], origin[3];
	char path[1024];
	voxelobj_t *voxobj;
	voxelbvh_t *voxbvh;
	Voxel *voxel;
	mvt_t *table;

	//initialize
	voxobj = (voxelobj_t*)calloc(1, sizeof(voxelobj_t));
	voxobj->numVoxels = 1;
	voxobj->voxels = (Voxel**)calloc(voxobj->numVoxels, sizeof(voxelobj_t*));

	table = (mvt_t*)calloc(voxobj->numVoxels, sizeof(mvt_t));

	// volume resolution overall
	resolution[0] = ceil((model->bbox->max[0] - model->bbox->min[0]) / voxelSize[X]) + (g_bound * 2);
	resolution[1] = ceil((model->bbox->max[1] - model->bbox->min[1]) / voxelSize[Y]) + (g_bound * 2);
	resolution[2] = ceil((model->bbox->max[2] - model->bbox->min[2]) / voxelSize[Z]) + (g_bound * 2);

	boxCenter[0] = (model->bbox->min[0] + model->bbox->max[0]) / 2.0;
	boxCenter[1] = (model->bbox->min[1] + model->bbox->max[1]) / 2.0;
	boxCenter[2] = (model->bbox->min[2] + model->bbox->max[2]) / 2.0;

	origin[0] = boxCenter[0] - (resolution[0] * voxelSize[X]) / 2.0;
	origin[1] = boxCenter[1] - (resolution[1] * voxelSize[Y]) / 2.0;
	origin[2] = boxCenter[2] - (resolution[2] * voxelSize[Z]) / 2.0;

	voxobj->voxelSize[X] = voxelSize[X];
	voxobj->voxelSize[Y] = voxelSize[Y];
	voxobj->voxelSize[Z] = voxelSize[Z];

	for (i = 0; i < 3; ++i)
	{
		voxobj->resolution[i] = resolution[i];
	}


	voxobj->voxels[0] = modelVoxelize(model, origin, voxelSize);
	strcpy(voxobj->voxels[0]->name, model->name);
	voxobj->voxels[0]->id = g_voxelID++;
	table->model = model;
	table->voxel = voxobj->voxels[0];



	// build bvh
	//voxbvh = buildVoxelBVH(modbvh, modobj->numModels, table);

	//voxobj->root = voxbvh;
	voxobj->root = NULL;
	//strcpy(voxobj->name, modobj->name);
	strcpy(voxobj->name, "scene");
	//printf("numVoxels: %d\n", voxobj->voxels[0]->numVoxels);
	return voxobj;
}

bool testWatertight(int x, int y, int z, Voxel *vox)
{
	int i, j, k, nb, index, offset[3];
	bool nbTest[26];


	for (nb = 0; nb < 26; ++nb)
	{
		nbTest[nb] = false;
		offset[X] = g_nbMat[nb][X];
		offset[Y] = g_nbMat[nb][Y];
		offset[Z] = g_nbMat[nb][Z];
		i = x + offset[X];
		j = y + offset[Y];
		k = z + offset[Z];
		while (i >= 0 && i < vox->resolution[X] &&
			j >= 0 && j < vox->resolution[Y] &&
			k >= 0 && k < vox->resolution[Z])
		{
			index = i + j * vox->resolution[X] + k * vox->resolution[X] * vox->resolution[Y];
			if (vox->data[index] == VOX_SURFACE) {
				nbTest[nb] = true;
				break;
			}
			i += offset[X];
			j += offset[Y];
			k += offset[Z];
		}
	} // end loopp nb
	for (nb = 0; nb < 26; ++nb)
	{
		if (nbTest[nb] == false)
			break;
	}
	if (nb == 26) {
		return true;
	}
	return false;
}

void voxRegionGrow(int x, int y, int z, Voxel *vox)
{
	int i, j, k, l, city, curIdx, nbIdx, offset[3], total;
	bool update;

	total = vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z];
	curIdx = x + y * vox->resolution[X] + z * vox->resolution[X] * vox->resolution[Y];
	vox->data[curIdx] = VOX_SOLID;
	update = true;
	int num = 0;
	while (update)
	{
		update = false;
		for (i = 1; i < vox->resolution[Z] - 1; ++i)
		{
			for (j = 1; j < vox->resolution[Y] - 1; ++j)
			{
				for (k = 1; k < vox->resolution[X] - 1; ++k)
				{
					curIdx = k + j * vox->resolution[X] + i * vox->resolution[X] * vox->resolution[Y];
					if (vox->data[curIdx] == VOX_EMPTY) {
						for (city = 0; city < 6; ++city)
						{
							offset[X] = g_cityMat[city][X]; offset[Y] = g_cityMat[city][Y]; offset[Z] = g_cityMat[city][Z];
							nbIdx = (k + offset[X]) +
								(j + offset[Y]) * vox->resolution[X] +
								(i + offset[Z]) * vox->resolution[X] * vox->resolution[Y];
							if (vox->data[nbIdx] == VOX_SOLID) {
								vox->data[curIdx] = VOX_SOLID;
								num++;
								update = true;
								break;
							}
						} // end loop city
					}
				} // end loop k
			} // end loop j
		} // end loop i
	} // end while loop
	printf("num: %d\n", num);
}

void solidifyVoxel(Voxel *vox)
{
	int i, j, k, index;

	// select growing point
	for (i = 0; i < vox->resolution[Z] - 1; ++i)
	{
		for (j = 0; j < vox->resolution[Y]; ++j)
		{
			for (k = 0; k < vox->resolution[X]; ++k)
			{
				index = k + j * vox->resolution[X] + i * vox->resolution[X] * vox->resolution[Y];
				if (vox->data[index] == VOX_SURFACE &&
					vox->data[index + vox->resolution[X] * vox->resolution[Y]] == VOX_EMPTY) {
					if (testWatertight(k, j, i + 1, vox)) {
						voxRegionGrow(k, j, i + 1, vox);
						goto test;
					}
				}
			} // end loop k
		} // end loop j
	} // end loop i

test:
	int solid = 0;
	for (i = 0; i < vox->resolution[Z]; ++i)
	{
		for (j = 0; j < vox->resolution[Y]; ++j)
		{
			for (k = 0; k < vox->resolution[X]; ++k)
			{
				index = k + j * vox->resolution[X] + i * vox->resolution[X] * vox->resolution[Y];
				if (vox->data[index] == VOX_SOLID) {
					solid++;
					//vox->data[index] = VOX_SURFACE;
				}
			}
		}
	}
	printf("solid: %d\n", solid);
}

void solidifyVoxelObject(voxelobj_t *obj)
{
	int i;

	for (i = 0; i < obj->numVoxels; ++i)
	{
		solidifyVoxel(obj->voxels[i]);
	}
}

void addVoxelObject(voxelobj_t *obj, voa_t *voa)
{
	// realloc
	if (voa->numObjects >= voa->cavboects) {
		voa->cavboects *= 2;
		voa->objects =
			(voxelobj_t**)realloc(voa->objects,
				voa->cavboects * sizeof(voxelobj_t*));
	}
	// add
	voa->objects[voa->numObjects] = obj;
	voa->numObjects++;
}

voa_t *newVoxelObjectArray()
{
	voa_t *voa;

	voa = (voa_t*)calloc(1, sizeof(voa_t));
	voa->cavboects = 1;
	voa->objects = (voxelobj_t**)calloc(1, sizeof(voxelobj_t*));

	return voa;
}

static voxelbvh_t *deleteBVH(voxelbvh_t *root)
{
	int i;

	if (root->children != NULL) {
		for (i = 0; i < root->numChildren; ++i)
		{
			root->children[i] = deleteBVH(root->children[i]);
		}
	}
	// delete
	if (root->voxel != NULL) {
		free(root->voxel);
	}
	free(root);
	return NULL;
}

void deleteVoxelObjectArray(voa_t **voa)
{
	int i;
	voxelobj_t *obj;

	for (i = 0; i < (*voa)->numObjects; ++i)
	{
		obj = (*voa)->objects[i];
		free(obj->voxels);
		obj->voxels = NULL;
		obj->root = deleteBVH(obj->root);
		free(obj);
	}
	free(*voa);
	*voa = NULL;
}

voxelobj_t *readVOFile(const char* fileName)
{
	int i, j, ret;
	float origin[3], center[3];
	FILE* fp;
	char buf[1024] = { '\0' }, path[1024] = { '\0' }, voxelName[1024] = { '\0' };
	const char* ptr;
	const char* pName;
	voxelobj_t *obj;

	obj = (voxelobj_t*)calloc(1, sizeof(voxelobj_t));

	fp = fopen(fileName, "r");

	// get path
	strcpy(buf, fileName);
	ptr = strrchr(buf, '/');
	strcpy(voxelName, ptr + 1);
	pName = strtok(voxelName, ".");
	strcpy(obj->name, pName);
	strncpy(path, buf, (int)(ptr - buf + 1));

	// parse .vo
	fscanf(fp, "VoxelSize %f %f %f\n", &obj->voxelSize[X],
		&obj->voxelSize[Y],
		&obj->voxelSize[Z]);
	fscanf(fp, "Resolution %d*%d*%d\n", &obj->resolution[X],
		&obj->resolution[Y],
		&obj->resolution[Z]);
	fscanf(fp, "Amount %d\n", &obj->numVoxels);
	fscanf(fp, "VoxelData\n");

	origin[0] = -obj->resolution[0] * obj->voxelSize[X] / 2.0;
	origin[1] = 0;
	origin[2] = -obj->resolution[2] * obj->voxelSize[Z] / 2.0;

	obj->voxels = (Voxel**)calloc(obj->numVoxels, sizeof(Voxel*));
	for (i = 0; i < obj->numVoxels; ++i)
	{
		fscanf(fp, "%1024[^\n]", voxelName);
		strcpy(buf, path);
		strcat(buf, voxelName);
		obj->voxels[i] = readBinaryVoxel(buf);
		for (j = 0; j < 3; ++j)
		{
			obj->voxels[i]->bbox.min[j] = origin[j] + obj->voxels[i]->minBound[j] * obj->voxelSize[j];
			obj->voxels[i]->bbox.max[j] = origin[j] + obj->voxels[i]->maxBound[j] * obj->voxelSize[j];
		}
	}

	fclose(fp);

	return obj;
}

void writeVOFile(voxelobj_t *obj)
{
	int i, j, voxRes[3];
	char path[1024], fileName[1024];
	FILE* fp;

	// get project path
	GetCurrentDirectoryA(1024, path);
	strcat(path, "\\Voxels\\");
	if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(path, 0);
	}
	strcat(path, obj->name);
	strcat(path, "\\");
	// create folder
	CreateDirectoryA(path, NULL);

	strcpy(fileName, path);
	strcat(fileName, obj->name);
	strcat(fileName, ".vo");
	fp = fopen(fileName, "w");
	if (fp == NULL) {
		fprintf(stderr, "Fail to open %s...\n", fileName);
	}
	// write voxel object file
	// format:
	//		VoxelSize float
	//		Resolution int*int*int
	//		Amount int
	//		VoxelData
	//		[Voxel Name]
	//			...
	//			...
	//		
	fprintf(fp, "VoxelSize %f %f %f\n", obj->voxelSize[X],
		obj->voxelSize[Y],
		obj->voxelSize[Z]);
	fprintf(fp, "Resolution %d*%d*%d\n", obj->resolution[X],
		obj->resolution[Y],
		obj->resolution[Z]);
	fprintf(fp, "Amount %d\n", obj->numVoxels);
	fprintf(fp, "VoxelData\n");
	for (i = 0; i < obj->numVoxels; ++i)
	{
		fprintf(fp, "%s.binvox\n", obj->voxels[i]->name);
	}
	// BVH... I will finished later :(

	// end BVH
	fclose(fp);

	// write voxel data
	for (i = 0; i < obj->numVoxels; ++i)
	{
		writeBinaryVoxel(obj->voxels[i], path);
	}
}

Voxel *readBinaryVoxel(const char* fileName)
{
	FILE *fp;
	const char* ptr;
	Voxel *vox;

	vox = (Voxel*)calloc(1, sizeof(Voxel));
	ptr = strrchr(fileName, '/');
	strcpy(vox->name, ptr + 1);
	fp = fopen(fileName, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Fail to open file...\n");
	}
	fread(&vox->numVoxels, sizeof(int), 1, fp);
	fread(&vox->minBound, sizeof(int), 3, fp);
	fread(&vox->maxBound, sizeof(int), 3, fp);
	fread(&vox->resolution, sizeof(int), 3, fp);
	fread(&vox->voxelSize, sizeof(float), 3, fp);
	vox->data = (unsigned char*)calloc(sizeof(unsigned char), vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z]);
	fread(vox->data, sizeof(unsigned char), vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z], fp);
	fclose(fp);

	return vox;
}

void writeBinaryVoxel(Voxel *vox, const char* path)
{
	FILE *fp;
	char fileName[1024];

	strcpy(fileName, path);
	strcat(fileName, vox->name);
	strcat(fileName, ".binvox");
	fp = fopen(fileName, "wb");
	fwrite(&vox->numVoxels, sizeof(int), 1, fp);
	fwrite(&vox->minBound, sizeof(int), 3, fp);
	fwrite(&vox->maxBound, sizeof(int), 3, fp);
	fwrite(&vox->resolution, sizeof(int), 3, fp);
	fwrite(&vox->voxelSize, sizeof(float), 3, fp);
	fwrite(vox->data, sizeof(unsigned char), vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z], fp);
	fclose(fp);
}

int testRead(const char* fileName)
{
	FILE* fp;
	int i, num, res[3];
	unsigned char* data;

	res[0] = 58;
	res[1] = 90;
	res[2] = 32;
	data = (unsigned char*)calloc(res[0] * res[1] * res[2], sizeof(unsigned char));
	fp = fopen(fileName, "rb");
	fread(data, sizeof(unsigned char), res[0] * res[1] * res[2], fp);
	num = 0;
	for (i = 0; i < res[0] * res[1] * res[2]; ++i)
	{
		if (data[i] == 1) {
			num++;
		}
	}
	printf("%d\n", num);
	return num;
}

int voxIndex(Voxel *vox, int x, int y, int z)
{
	return x +
		y * vox->resolution[X] +
		z * vox->resolution[X] * vox->resolution[Y];
}

void countVoxels(Voxel *vox)
{
	int i, total;

	total = vox->resolution[X] * vox->resolution[Y] * vox->resolution[Z];
	vox->numVoxels = 0;
	for (i = 0; i < total; ++i)
	{
		if (vox->data[i] != VOX_EMPTY)
			vox->numVoxels++;
	}
}
*/