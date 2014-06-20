/**
 * @file State.h
 * @brief State of a car.
 */

#ifndef DEF_STATE
#define DEF_STATE

#include "Map.h"
#include "Position.h"
#include "Vector.h"

typedef struct State* State;

/**
 * Initializes the state garbage collector.
 */
void State_initGC();

/**
 * Creates a car state.
 * @param position Position of the car.
 * @param speed Speed of the car.
 * @param boosts Remaining boosts.
 * @param map The map the car is on.
 * @return New state.
 */
State State_new(Position position, Vector speed, int boosts, Map map);

/**
 * Deletes a car state.
 * @param state State to delete.
 */
void State_delete(State state);

/**
 * Sets the parent of a state.
 * @param state The state.
 * @param parent Parent of the state state.
 */
void State_setParent(State state, State parent);

/**
 * Sets the real cost necessary to reach this state.
 * @param state
 * @param cost
 */
void State_setRealCost(State state, float cost);

Position State_getPosition(State state);
State State_getParent(State state);
Vector State_getSpeed(State state);
int State_getBoosts(State state);
float State_getRealCost(State state);
float State_getEstimateCost(State state);

/**
 * Function used to compare two states. Used in Heap.
 * Of type HeapCompareFct.
 */
int State_compare(void* a, void* b);

#endif