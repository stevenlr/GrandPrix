/**
 * @file Route.c
 * @brief Route tree implementation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "include/macros.h"
#include "include/Route.h"
#include "include/Tile.h"
#include "include/Map.h"
#include "include/Car.h"
#include "include/Vector.h"
#include "include/Position.h"
#include "include/Graphe.h"

/**
 * A route.
 */
struct Route {
	int dist;
	int depth;
	Vector acceleration;
};

List Route_createFromCar(Car car, Map map, Graphe g, int depth)
{
	return Route_create(Car_getPosition(car), Car_getSpeed(car), Car_getBoosts(car), map, g, depth);
}

void Route_makeRoutes(Position position, Vector speed, int boosts, Map map, Graphe g, int depth, int limitDepth, List routes, Vector originAcceleration, int needBoosts)
{
	List positions;
	Position newPosition, nextPosition;
	Vector newSpeed, acceleration;
	Route route;
	int i, useBoost;
	Tile tile;

	if(depth > limitDepth)
		return;

	nextPosition = Position_copy(position);
	Position_add(nextPosition, speed);

	positions = Map_getReachablePositions(position, speed, (needBoosts) ? boosts : 0, map);

	List_foreach(positions, newPosition, i)
	{

		useBoost = 0;
		newSpeed = Position_findOffset(position, newPosition);
		acceleration = Position_findOffset(nextPosition, newPosition);

		if(depth == 1)
			originAcceleration = Vector_copy(acceleration);

		if(!(Vector_squaredLength(newSpeed) == 0 && Vector_squaredLength(speed) == 0))
		{

			if(Vector_squaredLength(acceleration) > 2)
				useBoost = 1;

			route = malloc(sizeof(struct Route));
			route->dist = Graphe_getWeight(g, newPosition);//Map_getDistance(map, newPosition->x, newPosition->y);
			route->depth = depth;
			route->acceleration = Vector_copy(originAcceleration);

			tile = Map_getTile(map, newPosition->x, newPosition->y);

			if(tile == ARRIVAL)
				route->depth = limitDepth * 2 + 1 - depth;

			List_insert(routes, route);

			Route_makeRoutes(newPosition, newSpeed, boosts - useBoost, map, g, depth + 1, limitDepth, routes, originAcceleration, needBoosts);
		}

		if(depth == 1)
			Vector_delete(originAcceleration);

		Vector_delete(newSpeed);
		Vector_delete(acceleration);
	}

	List_head(positions);
	while(!List_isEmpty(positions))
	{
		Position_delete(List_getCurrent(positions));
		List_remove(positions);
	}

	List_delete(positions);
	Position_delete(nextPosition);
}

static int Route_needBoosts(Position position, Vector speed, Map map, int depth)
{
	Position pos = Position_copy(position);
	int i;

	depth *= 1.5;

	for(i = 0; i < depth; i++)
	{
		Position_add(pos, speed);

		if(Map_getTile(map, pos->x, pos->y) != ROAD)
			return 0;
	}

	return 1;
}

List Route_create(Position position, Vector speed, int boosts, Map map, Graphe g, int depth)
{
	List routes = List_new();
	int needBoosts = Route_needBoosts(position, speed, map, depth);

	Route_makeRoutes(position, speed, boosts, map, g, 1, depth, routes, NULL, needBoosts);

	return routes;
}

void Route_delete(Route route)
{
	assert(route);

	Vector_delete(route->acceleration);
	free(route);
}

void Route_removeConflictingPositions(List routes, Car cars[3])
{
	int i, j, conflict;
	Position p1, p2, nextPosition;
	Route r;

	assert(routes);

	nextPosition = Position_copy(Car_getPosition(cars[0]));
	Position_add(nextPosition, Car_getSpeed(cars[0]));

	List_head(routes);
	for(i = 0; i < List_getSize(routes); i++)
	{
		conflict = 0;
		r = List_getCurrent(routes);
		p1 = Position_copy(nextPosition);
		Position_add(p1, r->acceleration);

		for(j = 0; j < 3; j++)
		{
			p2 = Car_getPosition(cars[j]);

			if(Position_equal(p1, p2))
			{
				conflict = 1;
				break;
			}
		}

		Position_delete(p1);

		if(conflict)
		{
			List_remove(routes);
			Route_delete(r);
			i--;
			continue;
		}

		List_next(routes);
	}

	Position_delete(nextPosition);
}

void Route_keepDeepest(List routes)
{
	int maxDepth = 0;
	Route r;
	int i;

	List_foreach(routes, r, i)
	{
		if(r->depth > maxDepth)
			maxDepth = r->depth;
	}

	List_head(routes);
	for(i = 0; i < List_getSize(routes); i++)
	{
		r = List_getCurrent(routes);

		if(r->depth < maxDepth)
		{
			List_remove(routes);
			Route_delete(r);
			i--;
			continue;
		}

		List_next(routes);
	}
}

Vector Route_findBest(List routes)
{
	int minDist = 100000000;
	Route r;
	int i;
	Vector acceleration = NULL;

	List_foreach(routes, r, i)
	{
		if(r->dist < minDist)
		{
			minDist = r->dist;
			acceleration = r->acceleration;
		}
	}

	return acceleration;
}
