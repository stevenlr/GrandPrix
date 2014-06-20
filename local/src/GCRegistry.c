/**
 * @file GCRegistry.c
 * @brief Garbage collector registry implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "include/GCRegistry.h"

/**
 * Maximum number of garbage collectors used.
 */
#define MAX_GC_NUMBER 4

/**
 * Garbage collector registry.
 */
static GarbageCollector* gcRegistry[MAX_GC_NUMBER];

/**
 * Number of garbage collectors in use.
 */
static int gcCount = 0;

void GCRegistry_add(GarbageCollector* gc)
{
	assert(gcCount <= MAX_GC_NUMBER);
	assert(gc);

	gcRegistry[gcCount++] = gc;
}

void GCRegistry_flushAll()
{
	int i;

	for(i = 0; i < gcCount; i++)
		GC_flush(*(gcRegistry[i]));
}

void GCRegistry_deleteAll()
{
	int i;

	for(i = 0; i < gcCount; i++)
	{
		GC_delete(*(gcRegistry[i]));
		*(gcRegistry[i]) = NULL;
	}
}