/**
 * @file Car.h
 * @brief Car headers.
 */

#ifndef DEF_CAR
#define DEF_CAR

#include "List.h"
#include "Position.h"
#include "Vector.h"
#include "Map.h"

typedef struct Car* Car;

/**
 * Creates a car.
 * @param x Spawn x coordinate.
 * @param y Spawn y coordinate.
 * @return The created car.
 */
Car Car_new(int x, int y);

/**
 * Deallocates a car.
 * @param car Car to be deallocated.
 */
void Car_delete(Car car);

/**
 * Updates the position of a car.
 * @param car Car to update.
 * @param x New x coordinate.
 * @param y New y coordinate.
 */
void Car_updatePosition(Car car, int x, int y);

/**
 * Returns the positions a car can reach.
 * @param car The car.
 * @param map The map the car is racing on.
 * @return The list of reachable positions.
 */
List Car_getReachablePositions(Car car, Map map);

/**
 * Updates the status describing if a car has arrived or not.
 * @param car Car to update.
 * @param map Map on which the car is.
 * @return 1 if the car has just arrived, 0 otherwise.
 */
int Car_updateArrivedStatus(Car car, Map map);

/**
 * Returns the status of the car.
 * @param car The car.
 * @return 1 if the car has arrived, 0 otherwise.
 */
int Car_hasArrived(Car car);

/**
 * Returns the position of the car.
 * @param car The car.
 * @return The position of the car.
 */
Position Car_getPosition(Car car);

/**
 * Returns the speed of the car.
 * @param car The car.
 * @return The speed of the car.
 */
Vector Car_getSpeed(Car car);

int Car_getBoosts(Car car);

/**
 * Returns the acceleration required to reach a position.
 * @param car The car.
 * @param position The position to reach.
 * @return The acceleration.
 */
Vector Car_getAccelerationToReach(Car car, Position position);

/**
 * Uses a boost.
 * @param car The car.
 */
void Car_useBoost(Car car);

/**
 * Get the nb of Boost remaining
 * @param car
 * @return nb of boost
 */
int Car_getBoost(Car car);
#endif
