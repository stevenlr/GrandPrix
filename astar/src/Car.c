/**
 * @file Car.c
 * @brief Car implementation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "include/macros.h"
#include "include/Position.h"
#include "include/Vector.h"
#include "include/Car.h"
#include "include/Map.h"
#include "include/Tile.h"

/**
 * Maximum number of boosts allowed.
 */
#define MAXBOOSTS 5

/**
 * Car.
 */
struct Car {
	int boosts;				//!< Number of boosts remaining.
	Position currentPos;	//!< Current confirmed position.
	Position lastPos;		//!< Last confirmed position.
	Vector speed;			//!< Current speed.
	int hasArrived;			//!< Car has arrived or not.
};

Car Car_new(int x, int y)
{
	Car car;

	car = malloc(sizeof(struct Car));

	if(car == NULL)
	{
		LOGERR("Couldn't allocate car.");
		return NULL;
	}

	car->currentPos = Position_new(x, y);
	car->lastPos = Position_new(x, y);
	car->speed = Vector_new(0, 0);
	car->boosts = MAXBOOSTS;
	car->hasArrived = 0;

	return car;
}

void Car_delete(Car car)
{
	assert(car);

	Position_delete(car->currentPos);
	Position_delete(car->lastPos);
	Vector_delete(car->speed);

	free(car);
}

void Car_updatePosition(Car car, int x, int y)
{
	assert(car);

	Position_delete(car->lastPos);
	car->lastPos = car->currentPos;
	car->currentPos = Position_new(x, y);

	Vector_delete(car->speed);
	car->speed = Position_findOffset(car->lastPos, car->currentPos);
}

List Car_getReachablePositions(Car car, Map map)
{
	assert(car && map);
	return Map_getReachablePositions(map, car->currentPos, car->speed, car->boosts);
}

int Car_updateArrivedStatus(Car car, Map map)
{
	Tile tile;
	int returnValue = 0;

	assert(car && map);

	tile = Map_getTile(map, car->currentPos->x, car->currentPos->y);

	if(tile == ARRIVAL && !car->hasArrived)
		returnValue = 1;

	car->hasArrived = (tile == ARRIVAL);

	return returnValue;
}

int Car_hasArrived(Car car)
{
	assert(car);
	return car->hasArrived;
}

Position Car_getPosition(Car car)
{
	assert(car);
	return car->currentPos;
}

Vector Car_getSpeed(Car car)
{
	assert(car);
	return car->speed;
}

int Car_getBoosts(Car car)
{
	assert(car);
	return car->boosts;
}

Vector Car_getAccelerationToReach(Car car, Position position)
{
	Position nextPosition;
	Vector acceleration;

	assert(car && position);

	nextPosition = Position_copy(car->currentPos);
	Position_add(nextPosition, car->speed);

	acceleration = Position_findOffset(nextPosition, position);

	Position_delete(nextPosition);

	return acceleration;
}

void Car_useBoost(Car car)
{
	assert(car);
	car->boosts--;
}