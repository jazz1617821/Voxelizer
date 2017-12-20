#ifndef _MESHMODEL_H
#define _MESHMODEL_H

/*-----------------------------------------------------------------
 *	meshmodel header file, created by Lu-Kang.
 *
 *	These functions are for the opengl mesh models.
 *  Including file loader, model rendering and model manager.
 *
 -----------------------------------------------------------------*/

#include "boundingbox.h"
#include "objecttree.h"
#include <stdbool.h>

#define EMPTY_ID 4294967295

typedef struct Edge
{
	int v[2];
}Edge;

typedef struct Face
{
	int v[3];
	int vn[3];
	int vt[3];
}Face;

typedef struct HalfEdge
{
	struct HalfEdge *next;
	struct HalfEdge *previous;
	struct HalfEdge *pair;

	int vertex;
	int face;
	int edge;
}HalfEdge;

// vertex array
typedef struct VA
{
	int numVertices;
	int capVertices;
	float *data;
}VA;

typedef struct FreeList
{
	int index;
	struct FreeList *next;
}FreeList;

typedef struct MeshModel
{
	char name[512];
	unsigned short id;

	VA vertexArray;
	VA normalArray;
	VA texcoordArray;
	int numFaces;
	int capFaces;
	Face *faces;
	int numHalfEdges;
	int capHalfEdges;
	HalfEdge *halfedges;
	float precision;
	AABB *bbox;
	BS *bs;
	Transformation *transformation;

	// render
	VBO *vbo;
}MeshModel;

typedef struct MeshModelItem;

typedef struct MeshModelObject
{
	ObjectTreeNode node;
	char name[512];
	unsigned short id;

	struct MeshModelItem *item;
	AABB *bbox;
	Transformation *transformation;
}MeshModelObject;

typedef struct MeshModelItem
{
	EnqDList link;
	MeshModel *model;			// model instance
	MeshModelObject *object;		// link to object
}MeshModelItem;

typedef struct MeshModelScene
{
	char name[512];
	MeshModelObject *root;
	MeshModelItem *items;
	int numOfModels;
}MeshModelScene;

// vertex array
int addVertexArray3D(float v[3], VA *va);
void initVertexArray(VA *va);
void getVertex3D(VA va, int index, float v[3]);
void calTriangleNormal(float v0[3], float v1[3], float v2[3], float vn[3]);
void getTriangle(MeshModel *model, int face, float triVert[9]);

// empty list
FreeList *newFreeList(void);
void deleteFreeList(FreeList **ls);

// mesh model
MeshModel *newModel(void);
void deleteModel(MeshModel **model);
MeshModel *transformModel(MeshModel *model);

// mesh model item
MeshModelItem *newModelItemAnchor(void);
MeshModelItem *newModelItem(MeshModel *model, MeshModelObject *obj);
void clearModelItems(MeshModelItem *items);
void deleteModelItem(MeshModelItem **item);
void deleteModelItems(MeshModelItem **items);
MeshModelItem *firstModelItem(MeshModelItem *items);
MeshModelItem *lastModelItem(MeshModelItem *items);
MeshModelItem *nextModelItem(MeshModelItem *item);
MeshModelItem *prevModelItem(MeshModelItem *item);
void pushFrontModelItem(MeshModelItem *items, MeshModelItem *item);
void pushBackModelItem(MeshModelItem *items, MeshModelItem *item);
MeshModelItem *popFrontModelItem(MeshModelItem *items);
MeshModelItem *popBackModelItem(MeshModelItem *items);
bool isAnchorModelItem(MeshModelItem *items);
bool isEmptyModelItem(MeshModelItem *items);

// mesh model object
MeshModelObject *newModelObject(void);
void deleteModelObject(MeshModelObject **obj);
void insertModelObject(MeshModelObject *root, MeshModelObject *obj);
void removeModelObject(MeshModelObject *obj);
MeshModelObject *parentOfModelObject(MeshModelObject *obj);
MeshModelObject *childOfModelObject(MeshModelObject *obj);
MeshModelObject *nextSiblingOfModelObject(MeshModelObject *obj);
MeshModelObject *prevSiblingOfModelObject(MeshModelObject *obj);
int levelOfModelObject(MeshModelObject *obj);
bool isRootModelObject(MeshModelObject *obj);
bool isLeafModelObject(MeshModelObject *obj);
void transformModelObject(MeshModelObject *obj);
void calModelObjectBBox(MeshModelObject *root);
void updateModelObjectBBox(MeshModelObject *obj);
Dequeue *bfModelObject(MeshModelObject *obj);
Dequeue *firstNodesOfModelObject(MeshModelObject *obj);
Dequeue *leafNodesOfModelObject(MeshModelObject *obj);

// mesh model scene
MeshModelScene *newModelScene(void);
void deleteModelScene(MeshModelScene **sc);
void addModelObjectToScene(MeshModelScene *sc, MeshModelObject *obj);
void delModelObjectFromScene(MeshModelScene *sc, MeshModelObject *obj);
void delModelItemFromScene(MeshModelScene *sc, MeshModelItem *item);
MeshModelItem *searchModel(int id, MeshModelScene *sc);
void showModelItems(MeshModelScene *sc);

// axis
void turnZUp(MeshModel *model);

// load file
MeshModelObject *loadSTL(const char *filename);



#endif // _MESHMODEL_H
