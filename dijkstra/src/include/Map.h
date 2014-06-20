/**
 * @file Map.h
 * @brief Header of the map structure and related functions.
 */

#ifndef DEF_MAP
#define DEF_MAP

#include "List.h"
#include "Tile.h"
#include "Position.h"
#include "Vector.h"
#include <stdio.h>

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
 * Get Height
 * @param map
 * @return height
 */
int Map_getHeight(Map map);

/**
 * Get Width
 * @param map
 * @return width
 */
int Map_getWidth(Map map);

/**
 * Get arrival positions
 * @param map
 * @return List of arrival positions
 */
List Map_getArrivals(Map map);

/**
 * Get reachable positions
 * @param pos
 * @param speed
 * @param boost
 * @param map
 * @return List of reachable positions
 */
List Map_getReachablePositions(Position pos, Vector speed, int boost,Map map);

/**
 * Test if the pos x,y has a border close to it.
 * @param map
 * @param x
 * @param y
 * @return true if a bordel is close to it else false.
 */
int Map_hasBorder(Map map, int x, int y);
#endif
