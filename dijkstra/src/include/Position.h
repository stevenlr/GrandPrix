/**
 * @file Position.h
 * @brief Position header.
 */

#ifndef DEF_POSITION
#define DEF_POSITION

#include "Vector.h"

typedef Vector Position;

/**
 * Creates a position.
 * @param x x coordinate.
 * @param y y coordinate.
 * @return The created position.
 */
Position Position_new(int x, int y);

/**
 * Makes a copy of a position.
 * @param pos The position to copy.
 * @return The copy.
 */
Position Position_copy(Position pos);

/**
 * Deletes a position.
 * @param pos Position to delete.
 */
void Position_delete(Position pos);

/**
 * Adds a vector to a position.
 * @param pos Position to add to.
 * @param vect Vector to add.
 */
void Position_add(Position pos, Vector vect);

/**
 * Substracts a vector to a position.
 * @param pos Position to substract from.
 * @param vect Vector to substract.
 */
void Position_substract(Position pos, Vector vect);

/**
 * Finds the offset between two positions.
 * @param from Origin position.
 * @param to Remote position.
 * @return Vector offset between the two positions.
 */
Vector Position_findOffset(Position from, Position to);

/**
 * Tests the equality of two positions.
 * @param p1 The first position.
 * @param p2 The second position.
 * @return 1 if they are equal, 0 otherwise.
 */
int Position_equal(Position p1, Position p2);

#endif