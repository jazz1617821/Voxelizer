#ifndef _QUEUE_H
#define _QUEUE_H

#include "enqlist.h"
#include <stdbool.h>


// Double-end queue
typedef struct Dequeue
{
	EnqDList item;
	void *data;
}Dequeue;


Dequeue *newDequeue(void);

void clearDequeue(Dequeue *queue);

void deleteDequeue(Dequeue **queue);

void pushFrontDequeue(Dequeue *queue, void *data);

void pushBackDequeue(Dequeue *queue, void *data);

void *popFrontDequeue(Dequeue *queue);

void *popBackDequeue(Dequeue *queue);

void *peekFrontDequeue(Dequeue *queue);

void *peekBackDequeue(Dequeue *queue);

bool isEmptyDequeue(Dequeue *queue);

int countDequeue(Dequeue *queue);







#endif // !_QUEUE_H
