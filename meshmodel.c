#include "meshmodel.h"
#include "mymath.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define X 0
#define Y 1
#define Z 2

/////////////////////////////
//     gloabal variable    //
/////////////////////////////

static int g_modelObjID = 0;
static int g_modelID = 0;

/////////////////////////////
//        functions        //
/////////////////////////////

//static int addTexcoordArray(float vt[3], float precision); // no implement
static MeshModelObject *asciiSTL(const char *filename);
static MeshModelObject *binarySTL(const char *filename);


void initVertexArray(VA *va)
{
	if (va == NULL) return;
	va->numVertices = 0;
	if (va->data != NULL)
		free(va->data);
	va->data = NULL;
}


int addVertexArray3D(float v[3], VA *va)
{
	int i;
	int index;

	// initial vertexArray
	if (va->data == NULL) {
		va->data = (float*)calloc(va->capVertices, sizeof(float));
	}
	else if (va->numVertices + 3 >= va->capVertices) {
		va->capVertices *= 2;
		va->data = (float*)realloc(va->data, va->capVertices  *sizeof(float));
	}

	index = va->numVertices;
	// add new vertex x, y, z
	for (i = 0; i < 3; ++i)
	{
		va->data[va->numVertices++] = v[i];
	}
	return index;
}

FreeList *newFreeList(void)
{
	FreeList *ls;

	ls = (FreeList*)calloc(1, sizeof(FreeList));
	ls->index = -1;
	ls->next = NULL;

	return ls;
}

void deleteFreeList(FreeList **ls)
{
	FreeList *curPtr;
	FreeList *nextPtr;

	curPtr = (*ls);
	nextPtr = (*ls)->next;
	while (true)
	{
		free(curPtr);
		curPtr = nextPtr;
		if (curPtr == NULL) break;
		nextPtr = nextPtr->next;
	}
	free(*ls);
	*ls = NULL;	
}

MeshModel *newModel()
{
	MeshModel *model;

	model = (MeshModel*)calloc(1, sizeof(MeshModel));
	model->id = g_modelID++;
	memset(model->name, '\0', 512);
	model->vertexArray.numVertices = 0;
	model->vertexArray.capVertices = 256;
	model->vertexArray.data = NULL;	
	model->normalArray.numVertices = 0;
	model->normalArray.capVertices = 256;
	model->normalArray.data = NULL;
	model->texcoordArray.numVertices = 0;
	model->texcoordArray.capVertices = 64;
	model->texcoordArray.data = NULL;
	model->transformation = newTransformation();
	model->bbox = newAABB();
	model->bs = newBS();

	return model;
}

void deleteModel(MeshModel **model)
{
	initVertexArray(&(*model)->vertexArray);
	initVertexArray(&(*model)->normalArray);
	initVertexArray(&(*model)->texcoordArray);
	
	if ((*model)->faces != NULL) {
		free((*model)->faces);
	}
	if ((*model)->halfedges != NULL) {
		free((*model)->halfedges);
	}
	deleteTransformation(&(*model)->transformation);
	deleteAABB(&(*model)->bbox);

	free(*model);
	*model = NULL;
}

MeshModel *transformModel(MeshModel  *model)
{
	int i;
	float modelMat[16], normalMat[16], center[3];
	MeshModel *ret;
	AABB *bbox;

	if (model->vbo == NULL || model->bbox->vbo == NULL)
		return;

	// copy a model data
	ret = newModel();
	strcpy(ret->name, model->name);
	ret->vertexArray.numVertices = model->vertexArray.numVertices;
	ret->vertexArray.data = (float*)calloc(model->vertexArray.numVertices, sizeof(float));
	memcpy(ret->vertexArray.data,
		   model->vertexArray.data,
		   model->vertexArray.numVertices * sizeof(float));
	ret->normalArray.numVertices = model->normalArray.numVertices;
	ret->normalArray.data = (float*)calloc(model->normalArray.numVertices, sizeof(float));
	memcpy(ret->normalArray.data,
		model->normalArray.data,
		model->normalArray.numVertices * sizeof(float));
	ret->numFaces = model->numFaces;
	ret->faces = (Face*)calloc(model->numFaces, sizeof(Face));
	memcpy(ret->faces, model->faces, model->numFaces * sizeof(Face));
	copyTransformation(model->transformation, ret->transformation);

	// calculate the bounding box of meshes
	bbox = newAABB();
	for (i = 0; i < ret->vertexArray.numVertices; i += 3)
	{
		addBound(bbox, ret->vertexArray.data + i);
	}
	aabbCenter(*bbox, center);

	// transform mesh model
	transformMatrix(ret->transformation, modelMat, center);
	normalM(modelMat, normalMat);
	for (i = 0; i < ret->vertexArray.numVertices; i += 3)
	{
		multMat4x3fv(modelMat, ret->vertexArray.data + i, ret->vertexArray.data + i);
	}
	for (i = 0; i < ret->normalArray.numVertices; i += 3)
	{
		multMat4x3fv(normalMat, ret->normalArray.data + i, ret->normalArray.data + i);
	}

	// calculate bounding box
	for (i = 0; i < ret->vertexArray.numVertices; i += 3)
	{
		addBound(ret->bbox, ret->vertexArray.data + i);
	}
	initTransformation(ret->transformation);
	
	return ret;
}

MeshModelItem *newModelItemAnchor(void)
{
	MeshModelItem *item;

	item = (MeshModelItem*)calloc(1, sizeof(MeshModelItem));
	initEnqDList(&item->link);
	item->model = NULL;
	item->object = NULL;

	return item;
}

MeshModelItem *newModelItem(MeshModel *model, MeshModelObject *obj)
{
	MeshModelItem *item;

	item = (MeshModelItem*)calloc(1, sizeof(MeshModelItem));
	initEnqDList(&item->link);
	item->model = model;
	item->object = obj;

	return item;
}

void clearModelItems(MeshModelItem *items)
{
	MeshModelItem *delItem;

	if (items->model != NULL)
		return;

	// delete all items
	while (!isEmptyModelItem(items))
	{
		delItem = popBackModelItem(items);
		free(delItem);
	}

	// link to itself
	items->link.flink = &(items->link);
	items->link.blink = &(items->link);
}

void deleteModelItem(MeshModelItem **item)
{
	if ((*item)->model == NULL) {
		if (isEmptyModelItem((*item))) {
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

void deleteModelItems(MeshModelItem **items)
{
	MeshModelItem *delItem;

	if (items == NULL)
		return;

	clearModelItems(*items);
	free(*items);
	*items = NULL;
}

MeshModelItem *firstModelItem(MeshModelItem *items)
{
	if (items->model != NULL) 
		return NULL;

	return (MeshModelItem*)items->link.flink;
}

MeshModelItem *lastModelItem(MeshModelItem *items)
{
	if (items->model != NULL) 
		return NULL;

	return (MeshModelItem*)items->link.blink;
}

MeshModelItem * nextModelItem(MeshModelItem * item)
{
	return (MeshModelItem*)item->link.flink;
}

MeshModelItem * prevModelItem(MeshModelItem * item)
{
	return (MeshModelItem*)item->link.blink;
}

void pushFrontModelItem(MeshModelItem *items, MeshModelItem *item)
{
	if (items->model != NULL)
		return;

	addEnqHead(&item->link, &item->link);
}

void pushBackModelItem(MeshModelItem *items, MeshModelItem *item)
{
	if (items->model != NULL)
		return;

	addEnqTail(&items->link, &item->link);
}

MeshModelItem *popFrontModelItem(MeshModelItem *items)
{
	if (items->model != NULL)
		return NULL;

	return (MeshModelItem*)removeEnqHead(&items->link);
}

MeshModelItem *popBackModelItem(MeshModelItem *items)
{
	if (items->model != NULL)
		return NULL;

	return (MeshModelItem*)removeEnqTail(&items->link);
}

bool isAnchorModelItem(MeshModelItem *items)
{
	return items->model == NULL ? true : false;
}

bool isEmptyModelItem(MeshModelItem *items)
{
	return isEmptyEnq(&items->link);
}

MeshModelObject *newModelObject()
{
	MeshModelObject *obj;

	obj = (MeshModelObject*)calloc(1, sizeof(MeshModelObject));
	initObjectTreeNode(&obj->node);
	memset(obj->name, '\0', 512);
	obj->id = g_modelObjID++;
	obj->transformation = newTransformation();
	obj->bbox = newAABB();
	obj->item = NULL;

	return obj;
}

void deleteModelObject(MeshModelObject **obj)
{
	MeshModelObject *deleteObj;
	Dequeue *deleteQueue;

	deleteQueue = bfObjectTreeNode(&(*obj)->node);
	while (!isEmptyDequeue(deleteQueue))
	{
		deleteObj = (MeshModelObject*)popFrontDequeue(deleteQueue);
		free(deleteObj);
	}
}

void insertModelObject(MeshModelObject *root, MeshModelObject *obj)
{
	insertObjectTreeNode(&root->node, &obj->node);
}

void removeModelObject(MeshModelObject *obj)
{
	removeObjectTreeNode(&obj->node);
}

MeshModelObject * parentOfModelObject(MeshModelObject * obj)
{
	return (MeshModelObject*)obj->node.parent;
}

MeshModelObject *childOfModelObject(MeshModelObject *obj)
{
	return (MeshModelObject*)obj->node.firstChild;
}

MeshModelObject *nextSiblingOfModelObject(MeshModelObject *obj)
{
	return (MeshModelObject*)obj->node.fsibling;
}

MeshModelObject *prevSiblingOfModelObject(MeshModelObject *obj)
{
	return (MeshModelObject*)obj->node.bsibling;
}

int levelOfModelObject(MeshModelObject *obj)
{
	return levelOfObjectTree(&obj->node);
}

bool isRootModelObject(MeshModelObject *obj)
{	
	return isRootObjectTreeNode(&obj->node);
}

bool isLeafModelObject(MeshModelObject *obj)
{
	return isLeafObjectTreeNode(&obj->node);
}

void transformModelObject(MeshModelObject *obj)
{
	//
}

MeshModelScene *newModelScene()
{
	MeshModelScene *sc;

	sc = (MeshModelScene*)calloc(1, sizeof(MeshModelScene));
	memset(sc->name, '\0', 512);
	sc->root = newModelObject();
	strcpy(sc->root->name, "root");
	sc->items = newModelItemAnchor();
	initEnqDList(&sc->items->link);
	return sc;
}


// Only this function can delete the models' data
void deleteModelScene(MeshModelScene **sc)
{
	int i;
	MeshModelItem *item;

	// delete models
	item = (MeshModelItem*)(*sc)->items->link.flink;
	while (item->model != NULL)
	{
		deleteModel(&item->model);
		item = (MeshModelItem*)item->link.flink;
	}

	// delete model items
	deleteModelItems(&(*sc)->items);

	// delete model objects
	deleteModelObject(&(*sc)->root);

	// safe free scene
	free(*sc);
	*sc = NULL;
}

// call this function when adding a new model object (e.q. opening an STL or OBJ file)
void addModelObjectToScene(MeshModelScene *sc, MeshModelObject *obj)
{	
	Dequeue *leafNodes;
	MeshModelObject *objNode;

	// add model object to the scene graph
	insertModelObject(sc->root, obj);
	// re-calculate the bounding box of all objects
	calModelObjectBBox(sc->root);

	// add model to the item list
	leafNodes = leafNodesOfModelObject(obj);
	while (!isEmptyDequeue(leafNodes))
	{
		objNode = (MeshModelObject*)popFrontDequeue(leafNodes);
		if (isLeafModelObject(objNode)) {
			pushBackModelItem(sc->items, objNode->item);
			sc->numOfModels++;
		}
	}
	deleteDequeue(&leafNodes);
}

void delModelObjectFromScene(MeshModelScene *sc, MeshModelObject *obj)
{
	Dequeue *leafNodes;
	MeshModelObject *objNode;

	// delete model object from the scene graph
	removeModelObject(obj);
	// re-calculate the bounding box of all objects
	updateModelObjectBBox(parentOfModelObject(obj));

	// delete model item from the item list
	leafNodes = leafNodesOfModelObject(obj);
	while (!isEmptyDequeue(leafNodes))
	{
		objNode = (MeshModelObject*)popFrontDequeue(leafNodes);
		if (isLeafModelObject(objNode)) {
			//deleteModelItem(&objNode->item);
			delModelItemFromScene(sc, obj->item);
		}
	}
	deleteDequeue(&leafNodes);
	deleteModelObject(&obj);
}

void delModelItemFromScene(MeshModelScene *sc, MeshModelItem *item)
{
	MeshModelItem *delItem;
	MeshModel *delModel;

	if (item == NULL)
		return;

	delItem = firstModelItem(sc->items);
	while (delItem != item) {
		delItem = nextModelItem(delItem);
	}
	delModel = delItem->model;

	// free model data
	// Must delete model item first
	// if delete the model, it will be regard as a anchor item
	// that will cause error link state
	deleteModelItem(&delItem);
	deleteModel(&delModel);

	sc->numOfModels--;
}

MeshModelItem *searchModel(int id, MeshModelScene *sc)
{
	MeshModelItem *item;

	item = (MeshModelItem*)sc->items->link.flink;
	while (item->model != NULL)
	{
		if (item->model->id == id)
			return item;
		item = (MeshModelItem*)item->link.flink;
	}
	return NULL;
}

void showModelItems(MeshModelScene *sc)
{
	MeshModelItem *item;

	item = (MeshModelItem*)sc->items->link.flink;
	while (item->model != NULL)
	{
		printf("%s\n", item->model->name);
		item = (MeshModelItem*)item->link.flink;
	}
	printf("\n");
}

static MeshModelObject *asciiSTL(const char *filename)
{
	FILE *fp;
	char buffer[1024];
	char *ptr;
	char *prev;
	int i, j, k, num, index;
	float val[3], offset[3], rotation[3], transMat[16], colorID[4], scaleMat[16];
	MeshModelObject *obj;
	MeshModelItem *item;
	MeshModel *model;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can not open %s...\\n", filename);
		return false;
	}
	
	// create entities
	model = newModel();
	obj = newModelObject();
	item = newModelItem(model, obj);

	// get filename
	strcpy(buffer, filename);
	ptr = strtok(buffer, "\\\\/");
	while (ptr != NULL) {
		prev = ptr;
		ptr = strtok(NULL, "\\\\/");
	}
	strcpy(model->name, strtok(prev, "."));

	// count how many tirangles in this file
	num = 0;
	while (fgets(buffer, 1000, fp) != NULL) {
		ptr = strstr(buffer, "facet normal");
		if (ptr != NULL) {
			++num;
		}
	}
	rewind(fp);
	model->numFaces = num;
	model->faces = (Face*)calloc(model->numFaces, sizeof(Face));

	// solid name
	ptr = fgets(buffer, 1000, fp);
	//ptr = strstr(buffer, "solid");
	//ret = sscanf_s(ptr, "solid %s\\n", buffer);

	// facet loop
	initAABB(model->bbox);
	for (i = 0; i < num; ++i)
	{
		ptr = fgets(buffer, 1000, fp);
		ptr = strstr(buffer, "facet normal");
		sscanf(ptr, "facet normal %f %f %f", &val[0], &val[1], &val[2]);
		index = addVertexArray3D(val, &model->normalArray);
		for (j = 0; j < 3; ++j) 
		{
			model->faces[i].vn[j] = index;
		} // end normal
		ptr = fgets(buffer, 1000, fp);
		ptr = strstr(buffer, "outer loop");
		for (j = 0; j < 3; ++j) 
		{
			ptr = fgets(buffer, 1000, fp);
			ptr = strstr(buffer, "vertex");
			sscanf(ptr, "vertex %f %f %f", &val[0], &val[1], &val[2]);
			addBound(model->bbox, val);	// find boundary
			index = addVertexArray3D(val, &model->vertexArray);
			model->faces[i].v[j] = index;
		} // end vertex
		ptr = fgets(buffer, 1000, fp);
		ptr = strstr(buffer, "endloop");
		ptr = fgets(buffer, 1000, fp);
		ptr = strstr(buffer, "endfacet");
		
	}
	fclose(fp);

	// temp : turn z axis to up
	turnZUp(model);

	// set model's center to origin
	offset[0] = -(model->bbox->max[0] + model->bbox->min[0]) / 2.0;
	//offset[1] = -(model->bbox->max[1] + model->bbox->min[1]) / 2.0;
	offset[1] = 0;
	offset[2] = -(model->bbox->max[2] + model->bbox->min[2]) / 2.0;
	translate(offset[0], offset[1], offset[2], transMat);
	for (i = 0; i < model->vertexArray.numVertices; i += 3)
	{
		multMat4x3fv(transMat, model->vertexArray.data + i, model->vertexArray.data + i);
	}
	multMat4x3fv(transMat, model->bbox->max, model->bbox->max);
	multMat4x3fv(transMat, model->bbox->min, model->bbox->min);
	/*
	// set model above the plane
	offset[0] = 0;
	offset[1] = (model->bbox->max[1] - model->bbox->min[1]) / 2.0;
	offset[2] = 0;
	tsfmSetTranslate(offset, model->transformation);
	*/
	// reset bbox
	translate(offset[0], offset[1], offset[2], transMat);
	multMat4x3fv(transMat, model->bbox->max, model->bbox->max);
	multMat4x3fv(transMat, model->bbox->min, model->bbox->min);


	// set variable
	obj->item = item;
	strcpy(obj->name, model->name);
	setAABB(obj->bbox, model->bbox);

	return obj;
}


static MeshModelObject *binarySTL(const char *filename)
{
	FILE *fp;
	char buffer[1024];
	char *ptr;
	char *prev;
	unsigned short attr;
	int i, j, k, num, index;
	float offset[3], transMat[16], val[3], colorID[4], scaleMat[16];
	MeshModelObject *obj;
	MeshModelItem *item;
	MeshModel *model;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Can not open %s...\\n", filename);
		return false;
	}

	// create entities
	model = newModel();
	obj = newModelObject();
	item = newModelItem(model, obj);


	// get filename
	strcpy(buffer, filename);
	ptr = strtok(buffer, "\\\\/");
	while (ptr != NULL) {
		prev = ptr;
		ptr = strtok(NULL, "\\\\/");
	}
	strcpy(model->name, strtok(prev, "."));

	// header [80 bytes]
	fread(buffer, sizeof(char), 80, fp);
	// numOfTri [4 bytes]
	fread(&num, sizeof(int), 1, fp);
	model->numFaces = num;
	model->faces = (Face*)calloc(model->numFaces, sizeof(Face));
	
	// facet loop
	initAABB(model->bbox);
	for (i = 0; i < num; ++i)
	{
		// normal [4 bytes  *3]
		fread(val, sizeof(float), 3, fp);
		index = addVertexArray3D(val, &model->normalArray);
		for (j = 0; j < 3; ++j)
		{
			model->faces[i].vn[j] = index;
		} // end normal

		// vertex [4 bytes  *3  *3]
		for (j = 0; j < 3; ++j)
		{
			fread(val, sizeof(float), 3, fp);
			index = addVertexArray3D(val, &model->vertexArray);
			model->faces[i].v[j] = index;
			addBound(model->bbox, val);
		}// end vertex
		// attributes [2 bytes]
		fread(&attr, sizeof(unsigned short), 1, fp);
	}
	fclose(fp);

	// temp : turn z axis to up
	turnZUp(model);

	// set model's center to origin
	offset[0] = -(model->bbox->max[0] + model->bbox->min[0]) / 2.0;
	offset[1] = 0;
	offset[2] = -(model->bbox->max[2] + model->bbox->min[2]) / 2.0;
	translate(offset[0], offset[1], offset[2], transMat);
	for (i = 0; i < model->vertexArray.numVertices; i += 3)
	{
		multMat4x3fv(transMat, model->vertexArray.data + i, model->vertexArray.data + i);
	}
	multMat4x3fv(transMat, model->bbox->max, model->bbox->max);
	multMat4x3fv(transMat, model->bbox->min, model->bbox->min);
	/*
	// set model above the plane
	offset[0] = 0;
	offset[1] = (model->bbox->max[1] - model->bbox->min[1]) / 2.0;
	offset[2] = 0;
	tsfmSetTranslate(offset, model->transformation);
	*/
	// reset bbox
	translate(offset[0], offset[1], offset[2], transMat);
	multMat4x3fv(transMat, model->bbox->max, model->bbox->max);
	multMat4x3fv(transMat, model->bbox->min, model->bbox->min);
	
	
	// set variable
	obj->item = item;
	strcpy(obj->name, model->name);
	setAABB(obj->bbox, model->bbox);

	return obj;
}


MeshModelObject *loadSTL(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	char buffer[100];

	if (fp == NULL) {
		return false;
	}

	// if there is a string named "solid", then it will be an ascii file (Not 100%)
	fgets(buffer, 100, fp);
	fclose(fp);
	if (strstr(buffer, "solid")) {
		return asciiSTL(filename);
	}
	else {
		return binarySTL(filename);
	}
}

void getTriangle(MeshModel *model, int index, float triVert[9])
{
	int i, j;

	for (i = 0; i < 3; ++i)
		for (j = 0; j < 3; ++j)
			triVert[i * 3 + j] = model->vertexArray.data[model->faces[index].v[i] + j];
}

void calModelObjectBBox(MeshModelObject *root)
{
	MeshModelObject *childObject, *parentObject, *shownParent;
	Dequeue *dequeue;

	// breadth-first inserted dequeue of object tree
	dequeue = bfObjectTreeNode(&root->node);

	// pop from back of deqeueu (leaf nodes first)
	shownParent = NULL;	
	while (!isEmptyDequeue(dequeue))
	{
		childObject = (MeshModelObject*)popBackDequeue(dequeue);
		if (isLeafModelObject(childObject))
			setAABB(childObject->bbox, childObject->item->model->bbox);

		parentObject = (MeshModelObject*)childObject->node.parent;
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
	printf("calModelObjectBBox\n");
	printf("min: %f %f %f\n", root->bbox->min[X],
							  root->bbox->min[Y],
							  root->bbox->min[Z]);
	printf("max: %f %f %f\n", root->bbox->max[X],
							  root->bbox->max[Y],
							  root->bbox->max[Z]);
	printf("end\n");
#endif // !_DEBUG 

}


// Call this function after adding or removing an object from the tree.
// The parameter must be the parent node of the added or removed node.
void updateModelObjectBBox(MeshModelObject *obj)
{
	MeshModelObject *childObject, *parentObject;


	parentObject = obj;
	while (childObject != NULL)
	{
		// initialize parent node's bounding box
		initAABB(parentObject->bbox);
		// loop in each child objects and re-calculate the bounding box of the parent objcet
		childObject = (MeshModelObject*)parentObject->node.firstChild;
		while (childObject != NULL)
		{
			addAABB(parentObject->bbox, childObject->bbox);
			// next child
			childObject = (MeshModelObject*)childObject->node.fsibling;
		}
	}
}

Dequeue *bfModelObject(MeshModelObject *obj)
{
	return bfObjectTreeNode(&obj->node);
}

Dequeue *firstNodesOfModelObject(MeshModelObject *obj)
{
	return firstNodesOfModelObject(&obj->node);
}

Dequeue *leafNodesOfModelObject(MeshModelObject *obj)
{
	return leafNodesOfObjectTree(&obj->node);
}

void turnZUp(MeshModel *model)
{
	int i;
	float modelMat[16], normalMat[16];


	rotateX(90, modelMat);
	normalM(modelMat, normalMat);
	for (i = 0; i < model->vertexArray.numVertices; i += 3)
	{
		multMat4x3fv(modelMat, model->vertexArray.data + i, model->vertexArray.data + i);
	}
	for (i = 0; i < model->normalArray.numVertices; i += 3)
	{
		multMat4x3fv(normalMat, model->normalArray.data + i, model->normalArray.data + i);
	}

	initAABB(model->bbox);
	for (i = 0; i < model->vertexArray.numVertices; i += 3)
	{
		addBound(model->bbox, model->vertexArray.data + i);
	}
}

void getVertex3D(VA va, int index, float v[3])
{
	v[0] = va.data[index];
	v[1] = va.data[index + 1];
	v[2] = va.data[index + 2];
}

void calTriangleNormal(float v0[3], float v1[3], float v2[3], float vn[3])
{
	float a[3], b[3], normal[3];

	a[0] = v1[0] - v0[0];
	a[1] = v1[1] - v0[1];
	a[2] = v1[2] - v0[2];
	b[0] = v2[0] - v0[0];
	b[1] = v2[1] - v0[1];
	b[2] = v2[2] - v0[2];

	cross3fv(a, b, normal);

	vn[0] = normal[0];
	vn[1] = normal[1];
	vn[2] = normal[2];
}