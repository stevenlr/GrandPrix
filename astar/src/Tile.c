/**
 * @file Tile.c
 * @brief Tile implementation.
 */

#include <stdlib.h>

#include "include/Tile.h"

Tile Tile_fromChar(char c)
{
	switch(c)
	{
		case '1':
		case '2':
		case '3':
		case '#':
			return ROAD;
		case '~':
			return SAND;
		case '=':
			return ARRIVAL;
		case '.':
		default:
			return WALL;
	}
}

inline int Tile_isSpawn(char c)
{
	return (c >= '1' && c <= '3');
}

inline int Tile_getSpawnID(char c)
{
	return (Tile_isSpawn(c)) ? c - '1' : -1;
}