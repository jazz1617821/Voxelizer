#include "objecttree.h"
#include <stdio.h>
#include <stdlib.h>

void initObjectTreeNode(ObjectTreeNode *node)
{
	node->parent =
	node->fsibling =
	node->bsibling =
	node->firstChild = NULL;
}

int insertObjectTreeNode(ObjectTreeNode *des, ObjectTreeNode *src)
{
	if (des->firstChild == NULL) {
		des->firstChild = src;
		src->bsibling = src;
	}
	else {
		des->firstChild->bsibling->fsibling = src;
		src->bsibling = des->firstChild->bsibling;
		des->firstChild->bsibling = src;
	}
	src->parent = des;
	src->fsibling = NULL;

	return true;
}

int removeObjectTreeNode(ObjectTreeNode *node)
{
	if (node->parent->firstChild == node) {
		if (node->fsibling != NULL) {
			node->fsibling->bsibling = node->bsibling;
			node->parent->firstChild = node->fsibling;
		}
		else {
			node->parent->firstChild = NULL;
		}
	}
	else {
		node->bsibling->fsibling = node->fsibling;
		if (node->fsibling != NULL) {
			node->fsibling->bsibling = node->bsibling;
		}
		else {
			node->parent->firstChild->bsibling = node->bsibling;
		}
	}
	return true;
}

int isLeafObjectTreeNode(ObjectTreeNode *node)
{
	if (node->firstChild == NULL)
		return true;
	else
		return false;
}

int isRootObjectTreeNode(ObjectTreeNode *root)
{
	if (root->parent == NULL && root->fsibling == NULL && root->bsibling == NULL)
		return true;
	else
		return false;
}

Dequeue *bfObjectTreeNode(ObjectTreeNode *root)
{
	Dequeue *queue, *ret;
	ObjectTreeNode *node, *tmp;

	queue = newDequeue();
	ret = newDequeue();
	pushBackDequeue(queue, root);
	pushBackDequeue(ret, root);
	while (!isEmptyDequeue(queue))
	{
		tmp = (ObjectTreeNode*)popFrontDequeue(queue);
		node = tmp->firstChild;
		while (node != NULL)
		{
			pushBackDequeue(queue, node);
			pushBackDequeue(ret, node);
			node = node->fsibling;
		}
	}
	deleteDequeue(&queue);
	return ret;
}

// example
//           A
//          /   
//         B - C
//        /
//       D - E
//          /
//         F - G
//
// steps:
//                     pop A             pop B             pop D    pop E             pop F    pop G    pop C
// stack			A   ->   N   ->   CB  ->   C   ->  CED  ->   CE  ->   C   ->  CGF  ->   CG  ->   C   ->   N
//
// childStack		N   ->   BC  ->   N   ->   DE  ->   N   ->   N   ->   FG  ->   N   ->   N   ->   N   ->   N
//
//				   head                          tail
// ret				A -> B -> D -> E -> F -> G -> C
Dequeue *dfObjectTreeNode(ObjectTreeNode *root)
{
	Dequeue *ret;
	Stack *stack, *childStack;
	ObjectTreeNode *node, *childNode;

	ret = newDequeue();
	stack = newStack();
	childStack = newStack();
	pushStack(stack, root);
	while (!isEmptyStack(stack))
	{
		node = popStack(stack);
		// output
		pushBackDequeue(ret, node);

		// push child nodes into childStack
		childNode = node->firstChild;
		while (childNode != NULL)
		{
			pushStack(childStack, childNode);
			childNode = childNode->fsibling;
		}
		// pop out all ths nodes from childStack, and push into the stack
		while (!isEmptyStack(childStack))
		{
			pushStack(stack, popStack(childStack));
		}
	}

	deleteStack(&stack);
	deleteStack(&childStack);

	return ret;
}

int levelOfObjectTree(ObjectTreeNode *root)
{
	int ret = 1;
	ObjectTreeNode *node;

	if (root == NULL) return -1;

	node = root;
	while (node->firstChild != NULL)
	{
		ret++;
		node = node->firstChild;
	}

	return ret;
}

Dequeue *firstNodesOfObjectTree(ObjectTreeNode *root)
{
	Dequeue *dequeue;
	ObjectTreeNode *firstChild;

	dequeue = newDequeue();

	firstChild = root->firstChild;
	while (firstChild != NULL)
	{
		pushBackDequeue(dequeue, firstChild);
		firstChild = firstChild->fsibling;
	}

	return dequeue;
}

Dequeue *leafNodesOfObjectTree(ObjectTreeNode *root)
{
	Dequeue *dequeue;
	Dequeue *bfNodes;
	ObjectTreeNode *node;

	dequeue = newDequeue();

	bfNodes = bfObjectTreeNode(root);
	while (!isEmptyDequeue(bfNodes))
	{
		node = popFrontDequeue(bfNodes);
		if (isLeafObjectTreeNode(node)) {
			pushBackDequeue(dequeue, node);
		}
	}

	return dequeue;
}