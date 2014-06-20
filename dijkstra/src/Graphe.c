/*
 * Graphe.c
 *
 *  Created on: 20 mars 2014
 *      Author: gautier
 */
#include <stdlib.h>
#include <stdio.h>
 
#include "include/Graphe.h"
#include "include/List.h"
#include "include/Vector.h"
#include "include/Position.h"
#include "include/Map.h"
#include "include/Car.h"
#include "include/macros.h"

#define WEIGHT_ROAD 2
#define WEIGHT_SAND 10

#define VALIDPOSITION(w, h, x, y) (x >= 0 && y >= 0 && x < w && y < h)

//DEFINITION TYPE

typedef struct Cell* Cell;

struct Cell {
	int weight;
	Cell prev;
	List arcs;
	Position pos;
};

typedef struct Arc* Arc;

struct Arc {
	Position begin;
	Position end;
	int weight;
	Vector speed;
};

struct Graphe {
	Cell* data;
	int nbVertices;
	int height;
	int width;
};

//FIN DEFINITION TYPE

//FONCTION CELLULE

Cell Cell_new(int x, int y) {
	Cell c;
	c = malloc(sizeof(struct Cell));

	c->arcs = List_new();
	c->weight = -1;
	c->pos = Position_new(x, y);

	return c;
}

void Cell_delete(Cell c) {
	List_delete(c->arcs);
	Position_delete(c->pos);
	free(c);
}

//FIN FONCTION CELLULE
//FONCTION ARCS

Arc Arc_new(int beginX, int beginY, int endX, int endY, int weight) {
	Arc a;
	a = malloc(sizeof(struct Arc));

	a->begin = Vector_new(beginX, beginY);
	a->end = Vector_new(endX, endY);
	a->weight = weight;
	a->speed = NULL;

	return a;
}

void Arc_delete(Arc a) {
	Vector_delete(a->begin);
	Vector_delete(a->end);
	free(a);
}

//FIN FONCTION ARCS
//DEBUT STATIC FUNCTION
/**
 * Get cell on the position p
 * @param g
 * @param p
 * @return cell
 */
static Cell Graphe_getCell(Graphe g, Position p) {
	return g->data[g->width * p->y + p->x];
}

/**
 * Fonction to compare cell
 * @param n1
 * @param n2
 * @return n1 < n2
 */
static int List_compareCell(void* n1, void* n2) {
	return ((Cell) n1)->weight < ((Cell) n2)->weight;
}
//FIN STATIC FUNCTION

Graphe Graphe_new(int w, int h) {
	Graphe g;
	g = malloc(sizeof(struct Graphe));

	g->nbVertices = w * h;
	g->height = h;
	g->width = w;
	g->data = malloc(sizeof(Cell) * g->nbVertices);

	return g;
}

void Graphe_delete(Graphe g) {
	int i;

	for (i = 0; i < g->nbVertices; i++)
		if (g->data[i] != NULL)
			Cell_delete(g->data[i]);

	free(g->data);
	free(g);
}

List Graphe_init(Graphe g, Map map) {
	int i, j, cout;
	int width = g->width;
	int height = g->height;
	Tile tile;
	List borderPos = List_new();

	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			tile = Map_getTile(map, i, j);

			if (tile == WALL) {
				g->data[width * j + i] = NULL;
				continue;
			} else {
				g->data[width * j + i] = Cell_new(i, j);
			}

			if (tile == ROAD && Map_hasBorder(map, i, j))
				List_insert(borderPos, Position_new(i, j));

			cout = (tile == SAND ? WEIGHT_SAND : WEIGHT_ROAD);

			//Ajout des arcs
			if (VALIDPOSITION(width, height, i-1, j-1) && g->data[width * (j - 1) + (i - 1)] != NULL)
				List_insert(g->data[width * j + i]->arcs, Arc_new(i, j, i - 1, j - 1, cout + 1));

			if (VALIDPOSITION(width, height, i-1, j) && g->data[width * j + (i - 1)] != NULL)
				List_insert(g->data[width * j + i]->arcs, Arc_new(i, j, i - 1, j, cout));

			if (VALIDPOSITION(width, height, i-1, j+1) && g->data[width * (j + 1) + (i - 1)] != NULL)
				List_insert(g->data[width * j + i]->arcs, Arc_new(i, j, i - 1, j + 1, cout + 1));

			if (VALIDPOSITION(width, height, i, j-1) && g->data[width * (j - 1) + i] != NULL)
				List_insert(g->data[width * j + i]->arcs, Arc_new(i, j, i, j - 1, cout));

		}

	//Ajoutez les arcs dans l'autre sens de parcours
	for (i = width - 1; i >= 0; i--)
		for (j = height - 1; j >= 0; j--) {
			tile = Map_getTile(map, i, j);

			if (tile == WALL) {
				continue;
			}

			cout = (tile == SAND ? WEIGHT_SAND : WEIGHT_ROAD);

			//Ajout des arcs
			if (VALIDPOSITION(width, height, i+1, j+1) && g->data[width * (j + 1) + (i + 1)] != NULL)
				List_insert(g->data[width * j + i]->arcs, Arc_new(i, j, i + 1, j + 1, cout + 1));

			if (VALIDPOSITION(width, height, i+1, j) && g->data[width * j + (i + 1)] != NULL)
				List_insert(g->data[width * j + i]->arcs, Arc_new(i, j, i + 1, j, cout));

			if (VALIDPOSITION(width, height, i+1, j-1) && g->data[width * (j - 1) + (i + 1)] != NULL)
				List_insert(g->data[width * j + i]->arcs, Arc_new(i, j, i + 1, j - 1, cout + 1));

			if (VALIDPOSITION(width, height, i, j+1) && g->data[width * (j + 1) + i] != NULL)
				List_insert(g->data[width * j + i]->arcs, Arc_new(i, j, i, j + 1, cout));

		}

	return borderPos;
}

void Graphe_addTeleportationArc(Graphe g, Map map, List borderPos) {
	int i, j, k;
	int weightCellMax, cout, squaredLength;
	Vector v;
	Position pos;
	Arc arc;

	List_foreach(borderPos, pos, k)
	{
		weightCellMax = Map_getDistance(map, pos->x, pos->y) < 5 ? 1 : Map_getDistance(map, pos->x, pos->y) - 5;

		for (i = -5; i <= 5; i++) {
			for (j = -5; j <= 5; j++) {
				v = Vector_new(i, j);
				squaredLength = Vector_squaredLength(v);

				if (squaredLength > 25 || squaredLength < 2) {
					Vector_delete(v);
					continue;
				}

				Position_add(v, pos);

				if(!VALIDPOSITION(g->width, g->height, v->x, v->y) || Map_getTile(map, v->x, v->y) == WALL){
					Vector_delete(v);
					continue;
				}

				if(Map_getDistance(map, v->x, v->y) < weightCellMax){
					cout = (Map_getTile(map, pos->x, pos->y) == SAND ? WEIGHT_SAND : WEIGHT_ROAD);
					arc = Arc_new(pos->x, pos->y, v->x, v->y, cout);
					arc->speed = Vector_new(i, j);
					List_insert(g->data[g->width * pos->y + pos->x]->arcs, arc);
				}

				Vector_delete(v);
			}
		}
	}
}

void Graphe_insertArcsAuto(Graphe g, Map m, int x, int y, List l){
	int i, j, cout;
	Vector v;
	Position pos;
	Arc arc;

	for(i = -5; i <= 5; i++){
		for(j = -5; j <=5; j++){
			v = Vector_new(i,j);
			if(Vector_squaredLength(v) > 25){
				Vector_delete(v);
				continue;
			}

			pos = Position_new(x,y);
			Position_add(pos, v);

			if(!VALIDPOSITION(g->width, g->height, pos->x, pos->y) || Map_getTile(m, pos->x, pos->y) == WALL){
				Vector_delete(v);
				Position_delete(pos);
				continue;
			}

			cout = Map_getTile(m, pos->x, pos->y) == ROAD ? WEIGHT_ROAD : WEIGHT_SAND;
			arc = Arc_new(x, y, pos->x, pos->y, cout);
			arc->speed = v;
			List_insert(l, arc);

			Position_delete(pos);
		}
	}
}

void Graphe_dijkstra(Graphe g, Position begin, List cellEnds, List cellNotSeenBefore) {
	Cell c, cellEnd, cellStart = Graphe_getCell(g, begin);
	Arc arc;
	List cellNotSeen;
	int i,j, weight, minWeight = -1;

	if (cellNotSeenBefore == NULL)
		cellNotSeen = List_copy(cellEnds);

	while (!List_isEmpty(cellNotSeen)) {
		List_head(cellNotSeen);
		c = List_getCurrent(cellNotSeen);

		if (c->weight >= minWeight && minWeight != -1)
			break;

		List_remove(cellNotSeen);

		List_foreach(c->arcs, arc, i)
		{
			cellEnd = Graphe_getCell(g, arc->end);
			weight = c->weight + arc->weight;

			if (cellEnd->weight > weight || cellEnd->weight < 0) {
				cellEnd->weight = weight;
				cellEnd->prev = c;
				List_insertSorted(cellNotSeen, cellEnd, List_compareCell);
			}

			if (Position_equal(cellStart->pos, cellEnd->pos))
				if (weight < minWeight || minWeight < 0)
					minWeight = weight;
		}
	}

	for (i = 0; i < g->width; i++)
			for (j = 0; j < g->height; j++){
				if (g->data[g->width * j + i] != NULL){
					if(g->data[g->width * j + i]->weight < 0)
						g->data[g->width * j + i]->weight = 50000;
				}
			}
}



List Graphe_getEnds(Graphe g, List positionArrival) {
	int i;
	List list = List_new();
	Position pos;
	Cell c;

	List_foreach(positionArrival, pos, i) {
		c = Graphe_getCell(g, pos);
		List_insert(list, c);
	}

	return list;
}

Position Graphe_getPositionNext(Graphe g, Position pos) {
	Cell c = Graphe_getCell(g, pos);
	return c->prev->pos;
}

int Graphe_getWeight(Graphe g, Position pos) {
	return Graphe_getCell(g, pos)->weight;
}


















