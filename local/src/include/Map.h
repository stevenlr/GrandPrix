/**
 * @file Map.h
 * @brief Header of the map structure and related functions.
 */

#ifndef DEF_MAP
#define DEF_MAP

#include "List.h"
#include "Tile.h"
#include "Vector.h"
#include "Position.h"

typedef struct Map* Map;

/**
 * Loads a map from an input stream.
 * @param input Input stream.
 * @return The created map.
 */
Map Map_load(FILE* input);

/**
 * Deallocates a map.
 * @param map Map to be deallocated.
 */
void Map_delete(Map map);

/**
 * Recompute the distances for a map.
 * @param map The map to recompute.
 * @param takenPositions A list of the arrival positions already taken. Set to NULL if none.
 */
void Map_recomputeDistances(Map map, List takenPositions);

/**
 * Returns a tile.
 * @param map The map.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return The tile type.
 */
Tile Map_getTile(Map map, int x, int y);

/**
 * Returns the distance from a tile to the closest arrival point.
 * @param map The map.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return The distance.
 */
int Map_getDistance(Map map, int x, int y);

/**
 * Returns a list of reachable positions.
 * @param map Map the car is on.
 * @param carPosition Position of the car.
 * @param carSpeed Speed of the car.
 * @param boosts Number of remaining boosts.
 * @return The reachable positions.
 */
List Map_getReachablePositions(Map map, Position carPosition, Vector carSpeed, int boosts);

#endif