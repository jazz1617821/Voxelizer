#ifndef _STACK_H
#define _STACK_H

#include "enqlist.h"
#include <stdbool.h>

/*-------------------------------------------------------------------------------------------------------------
 *
 *
 *
 * 
 *        ________________       ________________       ________________       ________________
 *        | stack entity |       | stack entity |       | stack entity |       | stack entity |
 *        |--------------|       |--------------|       |--------------|       |--------------|
 *   <--  |     item     |  <--  |     item     |  <--  |     item     |  <--  |     item     |  <--  
 *   -->  |     NULL     |  -->  |     data     |  -->  |     data     |  -->  |     data     |  -->  
 *        ----------------       ----------------       ----------------       ----------------
 *              root                   head                second data               tail
 *
 *
 *	Usage:
 *		create a root entity (root data will always be null)
 *		push data into stack (create a new entity, and add it to tail)
 *		pop data from stack (remove the entity from tail, and delete it)
---------------------------------------------------------------------------------------------------------------*/


typedef struct Stack
{
	EnqDList item;
	void *data;
}Stack;



Stack *newStack(void);

void clearStack(Stack *stack);

void deleteStack(Stack **stack);

void pushStack(Stack *stack, void *data);

void *popStack(Stack *stack);

void *peekStack(Stack *stack);

int isEmptyStack(Stack *stack);

int countStack(Stack *stack);


#endif // !_STACK_H
