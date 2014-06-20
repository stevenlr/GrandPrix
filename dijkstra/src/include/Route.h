/**
 * @file Route.h
 * @brief Route tree header.
 */

#ifndef DEF_ROUTE
#define DEF_ROUTE

#include "Map.h"
#include "Car.h"
#include "Vector.h"
#include "Position.h"
#include "Graphe.h"

typedef struct Route* Route;

/**
 * Creates a new list of routes from a car.
 * @param car The car at the origin.
 * @param map The map the car is on.
 * @param depth The number of recursions to calculate the route for.
 * @return The routes created.
 */
List Route_createFromCar(Car car, Map map, Graphe g, int depth);

/**
 * Creates a new list of routes.
 * @param position Position of the car at the origin.
 * @param speed Speed of the car at the origin.
 * @param boosts Number of boosts remaining at the origin.
 * @param map The map the car is on.
 * @param depth The number of recursions to calculate the route for.
 * @return The routes created.
 */
List Route_create(Position position, Vector speed, int boosts, Map map, Graphe g, int depth);

/**
 * Deallocates a route.
 * @param route The route to deallocate.
 */
void Route_delete(Route route);

/**
 * Removes the positions occupied by cars.
 * @param route The route.
 * @param cars All the cars on the track.
 */
void Route_removeConflictingPositions(List routes, Car cars[3]);

/**
 * Only keeps the deepest routes.
 * @param route The route.
 */
void Route_keepDeepest(List routes);

/**
 * Finds the best route.
 * @param route The route.
 * @return The acceleration vector leading to the best route.
 */
Vector Route_findBest(List routes);

#endif
