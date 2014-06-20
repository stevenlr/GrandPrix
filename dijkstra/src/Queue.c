/**
 * @file Queue.c
 * @brief Queue implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "include/macros.h"
#include "include/List.h"
#include "include/Queue.h"

Queue Queue_new()
{
	return List_new();
}

void Queue_delete(Queue queue)
{
	assert(queue);
	List_delete(queue);
}

void Queue_insert(Queue queue, void* ptr)
{
	assert(queue);
	List_insert(queue, ptr);
}

void* Queue_peek(Queue queue)
{
	assert(queue);

	List_head(queue);

	return List_getCurrent(queue);
}

void* Queue_dequeue(Queue queue)
{
	void* ptr;
	
	assert(queue);

	List_head(queue);
	ptr = List_getCurrent(queue);
	List_remove(queue);

	return ptr;
}

int Queue_isEmpty(Queue queue)
{
	assert(queue);

	return List_isEmpty(queue);
}