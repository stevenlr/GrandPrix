/*
 * Path.h
 *
 *  Created on: 24 mars 2014
 *      Author: gautier
 */

#ifndef PATH_H_
#define PATH_H_

#include "Vector.h"
#include "Car.h"
#include "Map.h"
#include "Graphe.h"
#include "List.h"

#define ORDER_MAX 5

/**
 * Compute the next Offset via another way (SOS vector)
 * @param cars
 * @param map
 * @param g
 * @param bestWay
 * @return Next Offset
 */
Vector Path_getNextOffset(Car cars[3], Map map, Graphe g, List bestWay);

/**
 * Remove conflicting positions (TakenArrival/Collision)
 * @param positions
 * @param cars
 */
void Path_removeConflictingPositions(List positions, Car cars[3]);

/**
 * Get the best position between positions
 * @param positions
 * @param bestWay
 * @param g
 * @param map
 * @param car
 * @return best Position
 */
Position Path_getBestPosition(List positions, List bestWay, Graphe g, Map map, Car car);

/**
 * Return the arrival position of positionsList
 * @param positions
 * @param map
 * @return arrival position
 */
Position Path_getArrivalPosition(List positions, Map map);

/**
 * Compute path finding
 * @param map
 * @param g
 * @param cars
 * @return bestPosition
 */
Position Path_doPathfinding(Map map, Graphe g, Car cars[3]);

/**
 * Allow doPathFinding to know if we need to ignore the compute time of Dijkstra or not
 */
void Path_setIsFirst();

#endif /* PATH_H_ */
