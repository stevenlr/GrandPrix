/**
 * @file GarbageCollector.c
 * @brief Implementation of a garbage collector.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "include/macros.h"
#include "include/GarbageCollector.h"

/**
 * Garbage collector.
 */
struct GarbageCollector {
	int initialSize;	//!< Default size of the garbage collector.
	int allocated;		//!< Size of the allocated collector.
	int used;			//!< Number of elements in the collector.
	size_t size;		//!< Size of an element.
	void** collector;	//!< Collector.
};

/**
 * Resizes the collector.
 * @param gc The garbage collector to resize.
 * @param newSize The new size.
 */
static void GC_reallocCollector(GarbageCollector gc, int newSize)
{
	void** newCollector;

	assert(gc);
	assert(newSize > 0);

	if(newSize < gc->initialSize)
		newSize = gc->initialSize;
	
	if(newSize == gc->allocated)
		return;

	newCollector = realloc(gc->collector, sizeof(void*) * newSize);

	if(newCollector == NULL)
	{
		LOGERR("Couldn't reallocate garbage collector.");
		return;
	}

	gc->collector = newCollector;
	gc->allocated = newSize;
}

GarbageCollector GC_new(int initialSize, size_t size)
{
	GarbageCollector gc = malloc(sizeof(struct GarbageCollector));

	if(gc == NULL)
	{
		LOGERR("Couldn't allocate garbage collector.");
		return NULL;
	}

	gc->collector = malloc(sizeof(void*) * initialSize);

	if(gc->collector == NULL)
	{
		free(gc);
		LOGERR("Couldn't allocate garbage collector array.");
		return NULL;
	}

	gc->initialSize = initialSize;
	gc->size = size;
	gc->allocated = initialSize;
	gc->used = 0;

	return gc;
}

void GC_delete(GarbageCollector gc)
{
	assert(gc);

	if(gc->used != 0)
		LOGWARNING("Deleting non-empty gargbage collector.");

	free(gc->collector);
	free(gc);
}

void GC_flush(GarbageCollector gc)
{
	int i;

	assert(gc);

	if(gc->used == 0)
		return;

	for(i = 0; i < gc->used; i++)
		free(gc->collector[i]);

	GC_reallocCollector(gc, gc->initialSize);
	gc->used = 0;
}

void* GC_alloc(GarbageCollector gc)
{
	assert(gc);

	if(gc->used == 0)
		return malloc(gc->size);

//	if(gc->allocated > gc->initialSize && gc->used < (gc->allocated >> 1))
//		GC_reallocCollector(gc, (gc->allocated >> 2) * 3);
	
	return gc->collector[--(gc->used)];
}

void GC_free(GarbageCollector gc, void* ptr)
{
	assert(gc && ptr);

	if(gc->used == gc->allocated)
		GC_reallocCollector(gc, (gc->allocated >> 1) * 3);

	gc->collector[(gc->used)++] = ptr;
}