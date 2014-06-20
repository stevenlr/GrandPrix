/**
 * @file Position.c
 * @brief Position implementation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "include/macros.h"
#include "include/Vector.h"
#include "include/Position.h"

Position Position_new(int x, int y)
{
	return Vector_new(x, y);
}

Position Position_copy(Position pos)
{
	return Vector_copy(pos);
}

void Position_delete(Position pos)
{
	Vector_delete(pos);
}

void Position_add(Position pos, Vector vect)
{
	Vector_add(pos, vect);
}

void Position_substract(Position pos, Vector vect)
{
	Vector_substract(pos, vect);
}

Vector Position_findOffset(Position from, Position to)
{
	Vector offset;

	assert(from && to);

	offset = Vector_new(to->x - from->x, to->y - from->y);

	if(offset == NULL)
		LOGERR("Couldn't create new vector.");

	return offset;
}

int Position_equal(Position p1, Position p2)
{
	return Vector_equal(p1, p2);
}