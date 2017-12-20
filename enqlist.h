#ifndef _ENQLIST_H
#define _ENQLIST_H

#define ERROR_ENQ_NULL -1
#define ERROR_ENQ_EMPTY -2
#define ERROR_ENQ_ITEM_NULL -3
#define ERROR_ENQ_TARGET_NULL -4

#include <stdbool.h>

// EnqueuableDoubleList 

typedef struct EnqDList
{
	struct EnqDList *flink;
	struct EnqDList *blink;
}EnqDList;

int initEnqDList(EnqDList *item);
int isEnqed(EnqDList *item);
int isEmptyEnq(EnqDList *ls);
int addEnqHead(EnqDList *ls, EnqDList *item);
int addEnqTail(EnqDList *ls, EnqDList *item);
int addEnqBefore(EnqDList *target, EnqDList *item);
int addEnqAfter(EnqDList *target, EnqDList *item);
EnqDList *removeEnqHead(EnqDList *ls);
EnqDList *removeEnqTail(EnqDList *ls);
int removeEnqItem(EnqDList *item);
EnqDList *enqHead(EnqDList *ls);
EnqDList *enqTail(EnqDList *ls);
int countEnq(EnqDList* ls);


#endif // ENQLIST_H
