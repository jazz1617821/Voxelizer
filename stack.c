#include "stack.h"
#include <stdio.h>
#include <stdlib.h>


Stack *newStack(void)
{
	Stack *ret;

	ret = (Stack*)calloc(1, sizeof(Stack));
	initEnqDList(&ret->item);

	return ret;
}

void clearStack(Stack *stack)
{
	Stack *tmp;
	
	if (stack == NULL)
		return;

	while (!isEmptyEnq(&stack->item))
	{
		tmp = (Stack*)removeEnqTail(&stack->item);
		free(tmp);
	}
}

// Free all data in the stack.
// No response for freeing the data included in the data structure.
void deleteStack(Stack **stack)
{
	Stack *tmp;

	if(*stack == NULL)
		return;

	clearStack(*stack);
	free(*stack);
	*stack = NULL;
}

// Push data into stack.
// Create a new entity, and assign the value in it.
// Add the entity to the tail.
void pushStack(Stack *stack, void *data)
{
	Stack *newEntity;

	if (stack == NULL) 
		return;

	newEntity = newStack();
	newEntity->data = data;
	addEnqTail(&stack->item, &newEntity->item);
}


// Pop data from stack.
// Remove the data from the last entity
void *popStack(Stack *stack)
{
	void *ret;
	Stack *tmp;

	if (stack == NULL || isEmptyEnq(&stack->item))
		return NULL;

	tmp = (Stack*)removeEnqTail(&stack->item);
	ret = tmp->data;
	free(tmp);

	return ret;
}

// Return the top item of the stack
void *peekStack(Stack *stack)
{
	if (stack == NULL && isEmptyEnq(&stack->item)) 
		return NULL;
	
	return enqTail(&stack->item);
}

// If stack is empty or a NULL pointer, then return true
int isEmptyStack(Stack *stack)
{
	if (stack == NULL) 
		return false;

	return isEmptyEnq(&stack->item);
}

// Return the numbers of data in the stack
int countStack(Stack *stack)
{	
	if (stack == NULL) 
		return -1;

	return countEnq(&stack->item) - 1;
}