/**
 * @file Vector.c
 * @brief 2D int vectors implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "include/macros.h"
#include "include/GCRegistry.h"
#include "include/Vector.h"

static GarbageCollector vectorsGC = NULL;

void Vector_initGC()
{
	vectorsGC = GC_new(32, sizeof(struct Vector));
	GCRegistry_add(&vectorsGC);
}

Vector Vector_new(int x, int y)
{
	Vector vect = GC_alloc(vectorsGC);

	if(vect == NULL)
	{
		LOGERR("Couldn't allocate vector.");
		return NULL;
	}

	vect->x = x;
	vect->y = y;

	return vect;
}

Vector Vector_copy(Vector vect)
{
	Vector newVect;

	assert(vect);

	newVect = GC_alloc(vectorsGC);

	if(newVect == NULL)
	{
		LOGERR("Couldn't allocate copy of vector.");
		return NULL;
	}

	newVect->x = vect->x;
	newVect->y = vect->y;

	return newVect;
}

void Vector_delete(Vector vect)
{
	assert(vect);
	GC_free(vectorsGC, vect);
}

void Vector_add(Vector v1, Vector v2)
{
	assert(v1 && v2);

	v1->x += v2->x;
	v1->y += v2->y;
}

void Vector_substract(Vector v1, Vector v2)
{
	assert(v1 && v2);

	v1->x -= v2->x;
	v1->y -= v2->y;
}

int Vector_squaredLength(Vector vect)
{
	assert(vect);
	return vect->x * vect->x + vect->y * vect->y;
}

int Vector_equal(Vector v1, Vector v2)
{
	assert(v1 && v2);
	
	return v1->x == v2->x && v1->y == v2->y;
}