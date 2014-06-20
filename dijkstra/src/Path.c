/*
 * Path.c
 *
 *  Created on: 24 mars 2014
 *      Author: gautier
 */

#include <stdlib.h>

#include "include/Path.h"
#include "include/Vector.h"
#include "include/Car.h"
#include "include/Graphe.h"
#include "include/Map.h"
#include "include/List.h"
#include "include/macros.h"
#include "include/Position.h"
#include "include/Route.h"

#define ROUTE_PREVISION_DEPTH 5

struct Path {
	int order;
	int isArrivalPath;
	int weight;
	Position startPos;
};

typedef struct Path* Path;

Path Path_new(int order, int isArrivalPath, int weight, Position pos) {
	Path path = malloc(sizeof(struct Path));

	path->order = order;
	path->isArrivalPath = isArrivalPath;
	path->weight = weight;
	path->startPos = pos;

	return path;
}

void Path_delete(Path path) {
	free(path);
}

Vector Path_getNextOffset(Car cars[3], Map map, Graphe g, List bestWay) {
	List positions;
	Position position;

	positions = Car_getReachablePositions(cars[0], map);
	Path_removeConflictingPositions(positions, cars);
	position = Path_getArrivalPosition(positions, map);

	if (position == NULL)
		position = Path_getBestPosition(positions, bestWay, g, map, cars[0]);

	return Car_getAccelerationToReach(cars[0], position);
}


int List_PositionEqual(void* ptr1, void* ptr2) {
	return Position_equal((Position) ptr1, (Position) ptr2);
}

Position Path_getBestPosition(List positions, List bestWay, Graphe g, Map map, Car car) {
	Position bestPos;
	Position pos;
	int i, bestWeight = -1;

	List_foreach(positions, pos, i){
		if(Graphe_getWeight(g, pos) < bestWeight || bestWeight < 0){
			bestWeight = Graphe_getWeight(g, pos);
			bestPos = pos;
		}
	}

	return bestPos;
}


void Path_removeConflictingPositions(List positions, Car cars[3]) {
	int i, j, conflict;
	Position p1, p2;

	List_head(positions);
	for (i = 0; i < List_getSize(positions); i++) {
		conflict = 0;
		p1 = List_getCurrent(positions);

		// j = 1 if no self checking (see removeNoAccelerationPosition if speed = 0)
		for (j = 1; j < 3; j++) {
			p2 = Car_getPosition(cars[j]);

			if (Position_equal(p1, p2)) {
				conflict = 1;
				break;
			}
		}

		if (conflict) {
			List_remove(positions);
			Position_delete(p1);
			i--;
			continue;
		}

		List_next(positions);
	}
}

Position Path_getArrivalPosition(List positions, Map map) {
	int i;
	Position pos;

	List_foreach(positions, pos, i) {
		if (Map_getTile(map, pos->x, pos->y) == ARRIVAL)
			return pos;
	}

	return NULL;
}

int isFirst = 1;

void Path_setIsFirst(){
	isFirst = 1;
}

Vector Path_doPathfinding(Map map, Graphe g, Car cars[3])
{
	List routes;
	Vector acceleration;

	if(isFirst){
		routes = Route_createFromCar(cars[0], map, g, 5);
		isFirst = 0;
	}
	else
		routes = Route_createFromCar(cars[0], map, g, ROUTE_PREVISION_DEPTH);
	Route_removeConflictingPositions(routes, cars);

	if(List_isEmpty(routes))
	{
		List_delete(routes);
		return NULL;
	}

	Route_keepDeepest(routes);
	acceleration = Route_findBest(routes);

	while(!List_isEmpty(routes))
	{
		Route_delete(List_getCurrent(routes));
		List_remove(routes);
	}

	List_delete(routes);

	return acceleration;
}
