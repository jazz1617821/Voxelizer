#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Dequeue *newDequeue(void)
{
	Dequeue *ret;

	ret = (Dequeue*)calloc(1, sizeof(Dequeue));
	initEnqDList(&ret->item);

	return ret;
}

void clearDequeue(Dequeue *queue)
{
	Dequeue *tmp;

	if (queue == NULL)
		return;

	while (!isEmptyEnq(&queue->item))
	{
		tmp = (Dequeue*)removeEnqHead(&queue->item);
		free(tmp);
	}
}

void deleteDequeue(Dequeue **queue)
{
	if (queue == NULL)
		return;

	clearDequeue(*queue);
	free(*queue);
	*queue = NULL;
}

void pushFrontDequeue(Dequeue *queue, void *data)
{
	Dequeue* newEntity;

	if (queue == NULL)
		return;

	newEntity = (Dequeue*)calloc(1, sizeof(Dequeue));
	newEntity->data = data;
	addEnqHead(&queue->item, &newEntity->item);
}

void pushBackDequeue(Dequeue *queue, void *data)
{
	Dequeue* newEntity;

	if (queue == NULL)
		return;

	newEntity = (Dequeue*)calloc(1, sizeof(Dequeue));
	newEntity->data = data;
	addEnqTail(&queue->item, &newEntity->item);
}

void *popFrontDequeue(Dequeue *queue)
{
	void *ret;
	Dequeue *tmp;

	if (queue == NULL || isEmptyEnq(&queue->item))
		return NULL;

	tmp = (Dequeue*)removeEnqHead(&queue->item);
	ret = tmp->data;
	free(tmp);

	return ret;
}

void *popBackDequeue(Dequeue *queue)
{
	void *ret;
	Dequeue *tmp;

	if (queue == NULL || isEmptyEnq(&queue->item))
		return NULL;

	tmp = (Dequeue*)removeEnqTail(&queue->item);
	ret = tmp->data;
	free(tmp);

	return ret;
}

void *peekFrontDequeue(Dequeue *queue)
{
	if (queue == NULL && isEmptyEnq(&queue->item))
		return NULL;

	return enqHead(&queue->item);
}

void *peekBackDequeue(Dequeue *queue)
{
	if (queue == NULL && isEmptyEnq(&queue->item))
		return NULL;

	return enqTail(&queue->item);
}

bool isEmptyDequeue(Dequeue *queue)
{
	if (queue == NULL)
		return false;

	return isEmptyEnq(&queue->item);
}

int countDequeue(Dequeue *queue)
{
	if (queue == NULL)
		return -1;

	return countEnq(&queue->item) - 1;
}

