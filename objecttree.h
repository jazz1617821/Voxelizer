#ifndef _OBJECT_TREE_H
#define _OBJECT_TREE_H

#include "queue.h"
#include "stack.h"
#include <stdbool.h>

typedef struct ObjectTreeNode
{
	struct ObjectTreeNode *parent;
	struct ObjectTreeNode *fsibling;
	struct ObjectTreeNode *bsibling;
	struct ObjectTreeNode *firstChild;
}ObjectTreeNode;


void initObjectTreeNode(ObjectTreeNode *node);
int insertObjectTreeNode(ObjectTreeNode *des, ObjectTreeNode *src);
int removeObjectTreeNode(ObjectTreeNode *node);
int isLeafObjectTreeNode(ObjectTreeNode *node);
int isRootObjectTreeNode(ObjectTreeNode *root);
Dequeue *bfObjectTreeNode(ObjectTreeNode *root);
Dequeue *dfObjectTreeNode(ObjectTreeNode *root);
int levelOfObjectTree(ObjectTreeNode *root);
Dequeue *firstNodesOfObjectTree(ObjectTreeNode *root);
Dequeue *leafNodesOfObjectTree(ObjectTreeNode *root);


#endif // !_OBJECT_TREE_H
