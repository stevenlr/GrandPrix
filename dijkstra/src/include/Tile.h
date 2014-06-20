/**
 * @file Tile.h
 * @brief Tile (map cell) header.
 */

#ifndef DEF_TILE
#define DEF_TILE

/**
 * The different types of tiles.
 */
typedef enum {
	ROAD,		//!< Road (spawns are considered as roads)
	WALL,		//!< Wall
	SAND,		//!< Sand
	ARRIVAL		//!< Arrival
} Tile;

/**
 * Converts a char to its corresponding tile type.
 * @param c Char to convert.
 * @return Corresponding tile.
 */
Tile Tile_fromChar(char c);

/**
 * Determines whether a character represents a spawn or not.
 * @param c Char to be tested against.
 * @return 1 si c'est un spawn, 0 sinon.
 */
int Tile_isSpawn(char c); 

/**
 * Determines the if of the spawn.
 * @param c The char representing the spawn.
 * @return The id of the spawn. -1 if it's not a spawn.
 */
int Tile_getSpawnID(char c);

#endif