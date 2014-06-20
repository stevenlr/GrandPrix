/**
 * @file State.c
 * @brief Implementation of a car state for pathfinding.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "include/macros.h"
#include "include/State.h"
 #include "include/GCRegistry.h"

/**
 * A car state, used for A*.
 */
struct State {
	Position position;		//!< Position of the car.
	Vector speed;			//!< Speed of the car.
	int boosts;				//!< Boosts remaining.
	State parent;			//!< State leading to the current state.
	float realCost;			//!< Cost to reach this state.
	float estimatedCost;	//!< Real cost to this state plus estimated cost to the end.
};

float costHeuristic(Map map, Position position)
{
	return ceil((float) Map_getDistance(map, position->x, position->y) / 4);
}

static GarbageCollector statesGC = NULL;

void State_initGC()
{
	statesGC = GC_new(100, sizeof(struct State));
	GCRegistry_add(&statesGC);
}

State State_new(Position position, Vector speed, int boosts, Map map)
{
	State state;

	state = GC_alloc(statesGC);

	if(state == NULL)
	{
		LOGERR("Couldn't create state.");
		return NULL;
	}


	state->position = Position_copy(position);
	state->speed = Vector_copy(speed);
	state->boosts = boosts;
	state->parent = NULL;
	state->realCost = 0;
	state->estimatedCost = costHeuristic(map, position);

	return state;
}

void State_delete(State state)
{
	assert(state);
	Position_delete(state->position);
	Vector_delete(state->speed);
	GC_free(statesGC, state);
}

void State_setParent(State state, State parent)
{
	state->parent = parent;
}

void State_setRealCost(State state, float cost)
{
	state->realCost = cost;
	state->estimatedCost += cost;
}

Position State_getPosition(State state)
{
	return state->position;
}

State State_getParent(State state)
{
	return state->parent;
}

Vector State_getSpeed(State state)
{
	return state->speed;
}

int State_getBoosts(State state)
{
	return state->boosts;
}

float State_getRealCost(State state)
{
	return state->realCost;
}

float State_getEstimateCost(State state)
{
	return state->estimatedCost;
}

int State_compare(void* a, void* b)
{
	float diff = ((State) a)->estimatedCost - ((State) b)->estimatedCost;

	if(diff == 0)
		return 0;

	if(diff < 0)
		return -1;

	return 1;
}