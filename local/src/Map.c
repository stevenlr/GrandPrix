/**
 * @file Map.c
 * @brief Implementation of maps.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "include/macros.h"
#include "include/Map.h"
#include "include/Tile.h"
#include "include/Position.h"
#include "include/List.h"
#include "include/Queue.h"

/**
 * Number of spawns on the map.
 */
#define NSPAWNS 3

#define MAPDIST(m, x, y) m->distances[map->width * y + x]
#define MAPTILE(m, x, y) m->data[map->width * y + x]
#define VALIDPOSITION(m, x, y) (x >= 0 && y >= 0 && x < m->width && y < m->height)

/**
 * A map.
 */
struct Map {
	int width;					//!< Width of the map.
	int height;					//!< height of the map.
	int* distances;				//!< Distance of each case to the nearest arrival.
	Tile* data;					//!< Tiles.
	List arrivals;				//!< List of the arrival positions.
	Position spawns[NSPAWNS];	//!< Positions of the spawns.
};

Map Map_load(FILE* input)
{
	Map map;
	int x, y, id, fuel;
	char c;
	Tile tile;
	Position pos;

	assert(input);

	map = malloc(sizeof(struct Map));

	if(map == NULL)
	{
		LOGERR("Couldn't allocate map.");
		return NULL;
	}

	for(id = 0; id < NSPAWNS; id++)
		map->spawns[id] = NULL;

	fscanf(input, "%d %d %d", &(map->width), &(map->height), &fuel);

	if(map->width < 1 || map->height < 1)
	{
		free(map);
		LOGERR("Wrong input map format, you fool!");
		return NULL;
	}

	map->distances = malloc(sizeof(int) * map->width * map->height);

	if(map->distances == NULL)
	{
		LOGERR("Couldn't allocate distance map.");
		free(map);
		return NULL;
	}

	map->data = malloc(sizeof(Tile) * map->width * map->height);

	if(map->data == NULL)
	{
		LOGERR("Couldn't allocate map data.");
		free(map->distances);
		free(map);
		return NULL;
	}

	map->arrivals = List_new();

	if(map->arrivals == NULL)
	{
		LOGERR("Couldn't allocate map arrival list.");
		free(map->data);
		free(map->distances);
		free(map);
		return NULL;
	}

	while((c = fgetc(input)) != '\n');
	
	for(y = 0; y < map->height; y++)
	{
		for(x = 0; x < map->width; x++)
		{
			if(feof(input))
			{
				LOGERR("Input map data error.");
				return NULL;
			}

			c = fgetc(input);
			tile = Tile_fromChar(c);

			MAPTILE(map, x, y) = tile;

			if(Tile_isSpawn(c))
			{
				id = Tile_getSpawnID(c);
				map->spawns[id] = Position_new(x, y);

				if(map->spawns[id] == NULL)
					LOGERR("Error writing spawn position");
			}
			else if(tile == ARRIVAL)
			{
				pos = Position_new(x, y);

				if(pos == NULL)
					LOGERR("Error writing arrival position");

				List_insert(map->arrivals, pos);
			}
		}

		while((c = fgetc(input)) != '\n');
	}

	Map_recomputeDistances(map, NULL);

	return map;
}

void Map_delete(Map map)
{
	Position pos;
	int i;

	assert(map);

	free(map->distances);
	free(map->data);

	if(!List_isEmpty(map->arrivals))
	{
		List_head(map->arrivals);

		do {
			pos = List_getCurrent(map->arrivals);
			Position_delete(pos);
			List_remove(map->arrivals);
		} while(!List_isEmpty(map->arrivals));
	}

	List_delete(map->arrivals);

	for(i = 0; i < NSPAWNS; i++)
		if(map->spawns[i] != NULL)
			Position_delete(map->spawns[i]);

	free(map);
}

/**
 * Computes distances from a given point.
 * @param map The map to update.
 * @param origin The position from which the distances have to be computed.
 */
static void Map_computeDistance(Map map, Position origin)
{
	Queue queue;
	Position position;
	int offsets[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
	int originDist, x, y, i, dist;
	int add;

	assert(map);
	assert(origin);

	queue = Queue_new();

	if(queue == NULL)
		return;

	Queue_insert(queue, Position_copy(origin));

	while(!Queue_isEmpty(queue))
	{
		position = Queue_dequeue(queue);
		originDist = MAPDIST(map, position->x, position->y);

		for(i = 0; i < 4; i++)
		{
			x = position->x + offsets[i][0];
			y = position->y + offsets[i][1];

			if(!VALIDPOSITION(map, x, y))
				continue;

			if(MAPTILE(map, x, y) == ARRIVAL)
				continue;

			dist = MAPDIST(map, x, y);
			add = (MAPTILE(map, x, y) == SAND) ? 5 : 1;

			if(dist > 0 && dist > originDist + add)
			{
				MAPDIST(map, x, y) = originDist + add;
				Queue_insert(queue, Position_new(x, y));
			}
		}

		Position_delete(position);
	}

	Queue_delete(queue);
}

void Map_recomputeDistances(Map map, List takenPositions)
{
	int x, y, maxdist, i, j, taken;
	Tile tile;
	Position pos, takenPos;

	assert(map);

	maxdist = map->width * map->height + 1;

	for(y = 0; y < map->height; y++)
	{
		for(x = 0; x < map->width; x++)
		{
			tile = MAPTILE(map, x, y);

			switch(tile)
			{
				case ARRIVAL:
					MAPDIST(map, x, y) = 0;
					break;
				case SAND:
				case ROAD:
					MAPDIST(map, x, y) = maxdist;
					break;
				default:
					MAPDIST(map, x, y) = -1;
					break;
			}
		}
	}

	List_foreach(map->arrivals, pos, i)
	{
		taken = 0;

		if(takenPositions != NULL)
		{
			List_foreach(takenPositions, takenPos, j)
			{
				if(Position_equal(pos, takenPos))
				{
					taken = 1;
					break;
				}
			}
		}

		if(!taken)
			Map_computeDistance(map, pos);
	}
}

Tile Map_getTile(Map map, int x, int y)
{
	assert(map);

	if(!VALIDPOSITION(map, x, y))
		return WALL;

	return MAPTILE(map, x, y);
}

int Map_getDistance(Map map, int x, int y)
{
	assert(map);

	if(!VALIDPOSITION(map, x, y))
		return -1;

	return MAPDIST(map, x, y);
}

List Map_getReachablePositions(Map map, Position carPosition, Vector carSpeed, int boosts)
{
	List positions;
	Tile tile;
	Position newPosition;
	Vector speedVariation;
	int inSand, maxSpeed, radius, x, y;
	int currentDist;

	positions = List_new();

	assert(map);

	if(positions == NULL)
		return NULL;

	currentDist = MAPDIST(map, carPosition->x, carPosition->y);
	inSand = (MAPTILE(map, carPosition->x, carPosition->y) == SAND) ? 1 : 0;
	maxSpeed = (inSand) ? 1 : 25;
	radius = (boosts > 0) ? 2 : 1;

	for(y = -radius; y <= radius; y++)
	{
		for(x = -radius; x <= radius; x++)
		{
			speedVariation = Vector_copy(carSpeed);
			speedVariation->x += x;
			speedVariation->y += y;

			if(Vector_squaredLength(speedVariation) > maxSpeed)
			{
				Vector_delete(speedVariation);
				continue;
			}

			newPosition = Position_copy(carPosition);
			Position_add(newPosition, speedVariation);

			if(!VALIDPOSITION(map, newPosition->x, newPosition->y))
			{
				Position_delete(newPosition);
				Vector_delete(speedVariation);
				continue;
			}

			if(currentDist <= MAPDIST(map, newPosition->x, newPosition->y))
			{
				Position_delete(newPosition);
				Vector_delete(speedVariation);
				continue;
			}

			tile = MAPTILE(map, newPosition->x, newPosition->y);

			if(tile != WALL)
				List_insert(positions, newPosition);
			else
				Position_delete(newPosition);

			Vector_delete(speedVariation);
		}
	}

	return positions;
}