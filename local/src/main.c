/**
 * @file main.c
 * @brief Projet semestre 2, 1A Informatique, ENSICAEN (2013-2014, promo 2016)
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "include/macros.h"
#include "include/GCRegistry.h"
#include "include/List.h"
#include "include/Vector.h"
#include "include/Map.h"
#include "include/Car.h"
#include "include/Route.h"

#define ROUTE_PREVISION_DEPTH 4

void initGC()
{
	List_initGC();
	Vector_initGC();
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

/**
 * Does all the pathfinding.
 * @param map The map.
 * @param cars The three cars on the map. The first one is the one we are managing.
 */
void doPathfinding(Map map, Car cars[3])
{
	List routes;
	Vector acceleration;

	routes = Route_createFromCar(cars[0], map, ROUTE_PREVISION_DEPTH);
	Route_removeConflictingPositions(routes, cars);
	
	if(List_isEmpty(routes))
	{
		goRandom();
		List_delete(routes);
		return;
	}

	Route_keepDeepest(routes);
	acceleration = Route_findBest(routes);

	if(acceleration == NULL)
	{
		goRandom();
	}
	else
	{
		go(acceleration->x, acceleration->y);

		if(Vector_squaredLength(acceleration) > 2)
			Car_useBoost(cars[0]);
	}
	
	while(!List_isEmpty(routes))
	{
		Route_delete(List_getCurrent(routes));
		List_remove(routes);
	}

	List_delete(routes);
}

int main(int argc, char* argv[])
{
	Map map;
	Car cars[3] = {NULL, NULL, NULL};
	int x, y, i, rounds = 0, fuel;

	srand(time(NULL));

	fclose(stderr);
	freopen("pshrlog.txt", "w+", stderr);

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
				recomputeDistances(map, cars);
		}

		doPathfinding(map, cars);

		rounds++;
	}

	LOGINFO("End");

	Map_delete(map);
	freeGC();

	return 0;
}
