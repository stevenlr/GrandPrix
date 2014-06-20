/*
 ============================================================================
 Name        : DogeOrange.c
 Author      : Gautier BOËDA
 Version     :
 Copyright   : Don't feed the troll
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/Map.h"
#include "include/Car.h"
#include "include/List.h"
#include "include/Vector.h"
#include "include/GCRegistry.h"
#include "include/Position.h"
#include "include/macros.h"
#include "include/Graphe.h"
#include "include/Path.h"
#include "include/Route.h"

/**
 * Init the Garbage collector
 */
void initGC()
{
	List_initGC();
	Vector_initGC();
}

/**
 * Free the Garbage collector
 */
void freeGC()
{
	GCRegistry_flushAll();
	GCRegistry_deleteAll();
}

/**
 * Send the next offset to the server
 * @param x
 * @param y
 */
void go(int x, int y)
{
	LOGINFO2I("Accelerating : %d %d", x, y);

	fprintf(stdout, "%d %d\n", x, y);
	fflush(stdout);
}

/**
 * Recomputes the distance map.
 * @param map The map.
 * @param cars The cars on the track.
 */
void recomputeDistances(Map map, Car cars[3])
{
	int i;
	List takenPositions = List_new();

	for(i = 0; i < 3; i++)
		if(Car_hasArrived(cars[i]))
			List_insert(takenPositions, Car_getPosition(cars[i]));

	Map_recomputeDistances(map, takenPositions);

	List_empty(takenPositions);
	List_delete(takenPositions);
}

int main(int argc, char* argv[])
{
	Map map;
	Car cars[3] = {NULL, NULL, NULL};
	int x,y, i,carb, rounds = 1, recompute = 0;
	List listBestWay, listEnds, listBorderPos;
	Graphe g;
	Vector vect;
	char c;

	srand(time(NULL));

	fclose(stderr);
	freopen("dogeorangelog.txt", "w+", stderr);

	LOGINFO("Starting");

	initGC();


	map = Map_load(stdin);
	//FILE* stdinn = fopen("map.txt","r"); map = Map_load(stdinn);	//DEBUG

	LOGINFO("Map loaded");

	while(1)
	{
		LOGINFO1I("===== Round number %d =====", rounds);

		for(i = 0; i < 3; i++)
		{
			fscanf(stdin, "%d %d", &x, &y);

			//if (i == 0) { x = 3; y = 5; } else { x = 4; y = 7;}	//DEBUG

			LOGINFO3I("Car number %d, position : %d %d", i, x, y);

			if(cars[i] == NULL)
				cars[i] = Car_new(x, y);
			else
				Car_updatePosition(cars[i], x, y);

			if(Car_updateArrivedStatus(cars[i], map)){
				LOGINFO("recompute");
				recomputeDistances(map, cars);
				LOGINFO("recomputeOK");
				recompute = 1;
				Path_setIsFirst();
			}
		}

		if(rounds == 1){
			g = Graphe_new(Map_getWidth(map), Map_getHeight(map));

			listBorderPos = Graphe_init(g, map);
			LOGINFO("GrapheInitPart1 OK");
			Graphe_addTeleportationArc(g, map, listBorderPos);
			LOGINFO("GrapheInitOK");

			listEnds = Graphe_getEnds(g, Map_getArrivals(map));
			LOGINFO("ListEndsOK");

			Graphe_dijkstra(g, Car_getPosition(cars[0]), listEnds, NULL);
			LOGINFO("DijkstraOK");
			/*listBestWay = Graphe_shortestPath(g, map, Car_getPosition(cars[0]));
			LOGINFO("ListBestWayOK");*/
			recompute = 0;
		}else if(recompute){
			Graphe_dijkstra(g, Car_getPosition(cars[0]), listEnds, NULL);
			LOGINFO("DijkstraOK");
		}
		vect = NULL;

		vect = Path_doPathfinding(map, g, cars);
		LOGINFO("Route OK");

		//pos = Graphe_getPositionNext(g, Car_getPosition(cars[0]));	//V1 IA
		if(vect == NULL){
			vect = Path_getNextOffset(cars, map, g, listBestWay);		//V2 IA
			LOGINFO("PositionSOSNextOK");
		}

		if(Vector_squaredLength(vect) > 2)
			Car_useBoost(cars[0]);

		// LET S GO !
		go(vect->x, vect->y);

		rounds++;
	}

	LOGINFO("End");

	Map_delete(map);
	freeGC();

	return 0;
}
