/**
 * @file main.c
 * @brief Projet semestre 2, 1A Informatique, ENSICAEN (2013-2014, promo 2016)
 */

/*
░░░░░░░░░▄░░░░░░░░░░░░░░▄░░░░
░░░░░░░░▌▒█░░░░░░░░░░░▄▀▒▌░░░
░░░░░░░░▌▒▒█░░░░░░░░▄▀▒▒▒▐░░░
░░░░░░░▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐░░░
░░░░░▄▄▀▒░▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐░░░
░░░▄▀▒▒▒░░░▒▒▒░░░▒▒▒▀██▀▒▌░░░ 
░░▐▒▒▒▄▄▒▒▒▒░░░▒▒▒▒▒▒▒▀▄▒▒▌░░
░░▌░░▌█▀▒▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐░░
░▐░░░▒▒▒▒▒▒▒▒▌██▀▒▒░░░▒▒▒▀▄▌░
░▌░▒▄██▄▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▌░
▀▒▀▐▄█▄█▌▄░▀▒▒░░░░░░░░░░▒▒▒▐░
▐▒▒▐▀▐▀▒░▄▄▒▄▒▒▒▒▒▒░▒░▒░▒▒▒▒▌
▐▒▒▒▀▀▄▄▒▒▒▄▒▒▒▒▒▒▒▒░▒░▒░▒▒▐░
░▌▒▒▒▒▒▒▀▀▀▒▒▒▒▒▒░▒░▒░▒░▒▒▒▌░
░▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▒▄▒▒▐░░
░░▀▄▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▄▒▒▒▒▌░░
░░░░▀▄▒▒▒▒▒▒▒▒▒▒▄▄▄▀▒▒▒▒▄▀░░░
░░░░░░▀▄▄▄▄▄▄▀▀▀▒▒▒▒▒▄▄▀░░░░░
░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▀▀░░░░░░░░
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "include/macros.h"
#include "include/GCRegistry.h"
#include "include/List.h"
#include "include/Heap.h"
#include "include/Vector.h"
#include "include/Map.h"
#include "include/Car.h"
#include "include/State.h"

void initGC()
{
	List_initGC();
	Vector_initGC();
	State_initGC();
}

void freeGC()
{
	GCRegistry_flushAll();
	GCRegistry_deleteAll();
}

/**
 * Recomputes the distance map.
 * @param map The map.
 * @param cars The cars on the track.
 */
void recomputeDistances(Map map, Car cars[3])
{
	int i;
	List takenPositions = List_new();

	for(i = 0; i < 3; i++)
		if(Car_hasArrived(cars[i]))
			List_insert(takenPositions, Car_getPosition(cars[i]));

	Map_recomputeDistances(map, takenPositions);

	List_empty(takenPositions);
	List_delete(takenPositions);
}

/**
 * Sends the acceleration vector to the host application.
 * @param x X acceleration.
 * @param y Y acceleration.
 */
void go(int x, int y)
{
	LOGINFO2I("Accelerating : %d %d", x, y);

	fprintf(stdout, "%d %d\n", x, y);
	fflush(stdout);
}

/**
 * If there is no other choice, we go randomly.
 */
void goRandom()
{
	int x, y;

	x = (rand() % 3) - 1;
	y = (rand() % 3) - 1;

	LOGINFO("Brace yourself, going randomly...");

	go(x, y);
}

List doPathfinding(Map map, Car cars[3])
{
	Heap openSet = Heap_new(State_compare);
	List closedSet = List_new();
	List finalPath = List_new();
	List neighbors;
	Position neighbor;
	State state, newState;
	Vector newSpeed, acceleration;
	int end = 0, i, j, useBoost, positionTaken, distance;
	float cost;

	LOGINFO("A* doin' da werk!");

	state = State_new(Car_getPosition(cars[0]), Car_getSpeed(cars[0]), Car_getBoosts(cars[0]), map);
	Heap_insert(openSet, state);

	while(!Heap_isEmpty(openSet) && !end)
	{
		state = Heap_extractMin(openSet);

		if(Map_getTile(map, State_getPosition(state)->x, State_getPosition(state)->y) == ARRIVAL)
		{
			end = 1;
			break;
		}

		distance = Map_getDistance(map, State_getPosition(state)->x, State_getPosition(state)->y);
		neighbors = Map_getReachablePositions(map, State_getPosition(state), State_getSpeed(state), State_getBoosts(state));

		List_foreach(neighbors, neighbor, i)
		{
			if(Map_getDistance(map, neighbor->x, neighbor->y) > distance)
			{
				Position_delete(neighbor);
				continue;
			}

			cost = State_getRealCost(state) + 1;
			newSpeed = Position_findOffset(State_getPosition(state), neighbor);
			acceleration = Vector_copy(newSpeed);
			Vector_substract(acceleration, State_getSpeed(state));
			useBoost = 0;
			positionTaken = 0;

			if(Vector_squaredLength(acceleration) > 2)
			{
				useBoost = 1;
			}

			for(j = 1; j < 3; j++)
			{
				if(Position_equal(neighbor, Car_getPosition(cars[j])))
				{
					positionTaken = 1;
				}
			}

			if(!positionTaken)
			{
				newState = State_new(neighbor, newSpeed, State_getBoosts(state) - useBoost, map);
				State_setRealCost(newState, cost);
				State_setParent(newState, state);

				Heap_insert(openSet, newState);
			}

			Vector_delete(newSpeed);
			Vector_delete(acceleration);
			Position_delete(neighbor);
		}

		List_insert(closedSet, state);

		List_empty(neighbors);
		List_delete(neighbors);
	}

	while(state != NULL)
	{
		List_insert(finalPath, Position_copy(State_getPosition(state)));

		state = State_getParent(state);
	}

	List_head(closedSet);
	while(!List_isEmpty(closedSet))
	{
		state = List_getCurrent(closedSet);
		List_remove(closedSet);
		State_delete(state);
	}

	List_delete(closedSet);

	while((state = Heap_extractMin(openSet)) != NULL)
	{
		State_delete(state);
	}

	Heap_delete(openSet);

	LOGINFO("A* is done mate");

	return finalPath;
}

void freePath(List path)
{
	List_head(path);

	while(!List_isEmpty(path))
	{
		Position_delete(List_getCurrent(path));
		List_remove(path);
	}

	List_delete(path);
}

int main(int argc, char* argv[])
{
	Map map;
	Car cars[3] = {NULL, NULL, NULL};
	int x, y, i, rounds = 0, needPathfinding = 0, firstLoop = 1;
	List path;
	Position position;
	Vector acc;
	int fuel;

	srand(time(NULL));

	freopen("pshr4log.txt", "w+", stderr);
	// freopen("map.txt", "r", stdin);

	LOGINFO("Starting");

	initGC();

	map = Map_load(stdin);

	LOGINFO("Map loaded");

	while(!feof(stdin))
	{
		LOGINFO1I("===== Round number %d =====", rounds);

		for(i = 0; i < 3; i++)
		{
			fscanf(stdin, "%d %d", &x, &y);

			LOGINFO3I("Car number %i, position : %d %d", i, x, y);

			if(cars[i] == NULL)
				cars[i] = Car_new(x, y);
			else
				Car_updatePosition(cars[i], x, y);

			if(Car_updateArrivedStatus(cars[i], map))
			{
				recomputeDistances(map, cars);
				LOGINFO("I see you've arrived, let me see where I can park now...");
				needPathfinding = 1;
			}
		}

		if(firstLoop)
		{
			firstLoop = 0;
			path = doPathfinding(map, cars);
			List_tail(path);
			List_prev(path);
		}

		position = List_getCurrent(path);

		for(i = 1; i < 3; i++)
		{
			if(Position_equal(position, Car_getPosition(cars[i])))
			{
				needPathfinding = 1;
				LOGINFO("YOU TOOK MY SPOT JERK");
			}
		}

		if(needPathfinding)
		{
			freePath(path);
			path = doPathfinding(map, cars);
			List_tail(path);
			List_prev(path);
			needPathfinding = 0;
			position = List_getCurrent(path);
		}

		if(position == NULL)
		{
			goRandom();
		}
		else
		{
			LOGINFO2I("Going to %d %d", position->x, position->y);
			acc = Car_getAccelerationToReach(cars[0], position);
			go(acc->x, acc->y);

			if(Vector_squaredLength(acc) > 2)
			{
				Car_useBoost(cars[0]);
				LOGINFO("Using a boost");
			}

			Vector_delete(acc);
		}

		List_prev(path);

		rounds++;
	}

	LOGINFO("End");

	freePath(path);

	for(i = 0; i < 3; i++)
	{
		Car_delete(cars[i]);
	}

	Map_delete(map);
	freeGC();

	return 0;
}
