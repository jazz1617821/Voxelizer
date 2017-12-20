#include "enqlist.h"
#include <stdio.h>

int initEnqDList(EnqDList *item)
{
	if (item == NULL) 
		return ERROR_ENQ_ITEM_NULL;

	item->flink = item;
	item->blink = item;

	return true;
}

int isEnqed(EnqDList *item)
{
	if (item == NULL)
		return ERROR_ENQ_ITEM_NULL;

	if (item->flink == item || item->blink == item)
		return false;

	return true;
}

int isEmptyEnq(EnqDList *ls)
{
	if (ls == NULL) 
		return ERROR_ENQ_NULL;

	if (ls->flink == ls && ls->blink == ls)
		return true;

	return false;
}

int addEnqHead(EnqDList *ls, EnqDList *item)
{
	if (ls == NULL)
		return ERROR_ENQ_NULL;

	if(item == NULL)
		return ERROR_ENQ_ITEM_NULL;

	item->flink = ls->flink;
	item->blink = ls;
	ls->flink->blink = item;
	ls->flink = item;

	return true;
}

int addEnqTail(EnqDList  *ls, EnqDList *item)
{
	if (ls == NULL)
		return ERROR_ENQ_NULL;

	if(item == NULL)
		return ERROR_ENQ_ITEM_NULL;
	
	item->flink = ls;
	item->blink = ls->blink;
	ls->blink->flink = item;
	ls->blink = item;

	return true;
}

int addEnqBefore(EnqDList *target, EnqDList *item)
{
	if (target == NULL)
		return ERROR_ENQ_TARGET_NULL;

	if(item == NULL)
		return ERROR_ENQ_ITEM_NULL;

	item->flink = target;
	item->blink = target->blink;
	target->blink->flink = item;
	target->blink = item;

	return true;
}

int addEnqAfter(EnqDList *target, EnqDList *item)
{
	if (target == NULL)
		return ERROR_ENQ_TARGET_NULL;

	if(item == NULL)
		return ERROR_ENQ_ITEM_NULL;

	item->flink = target->flink;
	item->blink = target;
	target->flink->blink = item;
	target->flink = item;

	return true;
}

EnqDList *removeEnqHead(EnqDList* ls)
{
	EnqDList *item;

	if (ls == NULL || isEmptyEnq(ls))
		return NULL;

	item = ls->flink;
	removeEnqItem(item);
	item->flink = item;
	item->blink = item;

	return item;
}

EnqDList *removeEnqTail(EnqDList* ls)
{
	EnqDList *item;

	if (ls == NULL || isEmptyEnq(ls))
		return NULL;

	item = ls->blink;
	removeEnqItem(item);
	item->flink = item;
	item->blink = item;

	return item;
}

int removeEnqItem(EnqDList *item)
{
	if (item == NULL)
		return ERROR_ENQ_ITEM_NULL;

	item->flink->blink = item->blink;
	item->blink->flink = item->flink;

	return true;
}

EnqDList *enqHead(EnqDList *ls)
{
	if (ls == NULL)
		return NULL;

	return ls->flink;
}

EnqDList *enqTail(EnqDList *ls)
{
	if (ls == NULL)
		return NULL;

	return ls->blink;
}

int countEnq(EnqDList * ls)
{
	int ret = 1;
	EnqDList *tmp;
	
	if (ls == NULL)
		return ERROR_ENQ_NULL;

	tmp = ls->flink;
	while (tmp != ls)
	{
		ret++;
		tmp = tmp->flink;
	}

	return ret;
}
