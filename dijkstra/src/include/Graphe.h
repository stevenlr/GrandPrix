/*
 * Graphe.h
 *
 *  Created on: 20 mars 2014
 *      Author: gautier
 */

#ifndef GRAPHE_H_
#define GRAPHE_H_

#include "List.h"
#include "Position.h"
#include "Map.h"

typedef struct Graphe* Graphe;

/**
 * Create a new Graphe
 * @param w
 * @param h
 * @return Graphe
 */
Graphe Graphe_new(int w, int h);

/**
 * Delete a graphe
 * @param g
 */
void Graphe_delete(Graphe g);

/**
 * Init a Graphe with a map & return a list of cells which have at least a border.
 * @param g
 * @param map
 * @return BorderPos List of cells which have at least a border.
 */
List Graphe_init(Graphe g, Map map);

/**
 * Add arc between 2 positions reachable only with a teleportation.
 * @param g
 * @param map
 * @param borderPos List of cells which are next to a border position
 */
void Graphe_addTeleportationArc(Graphe g, Map map, List borderPos);
/**
 * Retourne une liste des sommets non visités car weight > bestWay au cas où on a besoin de pousser + loin l'algorithme (Position inaccessible)
 * @param g
 * @param begin Start Location
 * @param cellEnds Arrivals Location
 * @param cellNotSeen	//Sorted List by Cell Weight
 * @return cell not seen yet
 */
void Graphe_dijkstra(Graphe g, Position begin, List cellEnds, List cellNotSeen);

/**
 * Return the shortestPath (List of Position)
 * @param g
 * @param begin
 * @return List of Position (between begin to bestEnd)
 * */
List Graphe_shortestPath(Graphe g, Map map, Position begin);

/**
 * Return cellEnds;
 * @param g
 * @param positionArrival list
 * @return List of ends cells
 */
List Graphe_getEnds(Graphe g, List positionArrival) ;

/**
 * Return nextCell Position where the car must go to.
 * @param g
 * @param pos
 * @return
 */
Position Graphe_getPositionNext(Graphe g, Position pos);

/**
 * Get weight of the cell on the position pos
 * @param g
 * @param pos
 * @return weight
 */
int Graphe_getWeight(Graphe g, Position pos);

#endif /* GRAPHE_H_ */
