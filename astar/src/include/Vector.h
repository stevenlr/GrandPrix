/**
 * @file Vector.h
 * @brief 2D int vectors header.
 */

#ifndef DEF_VECTOR
#define DEF_VECTOR

/**
 * Vector.
 */
struct Vector {
	int x;	//!< x coordinate.
	int y;	//!< y coordinate.
};

typedef struct Vector* Vector;

/**
 * Initializes the vector garbage collector.
 */
void Vector_initGC();

/**
 * Creates a vector.
 * @param x x coordinate.
 * @param y y coordinate.
 * @return The created vector.
 */
Vector Vector_new(int x, int y);

/**
 * Makes a copy of a vector.
 * @param vect The vector to copy.
 * @return The copy.
 */
Vector Vector_copy(Vector vect);

/**
 * Deletes a vector.
 * @param vect vector to delete.
 */
void Vector_delete(Vector vect);

/**
 * Adds a vector to another.
 * @param v1 Vector to add to.
 * @param v2 Vector to add.
 */
void Vector_add(Vector v1, Vector v2);

/**
 * Substracts a vector to another.
 * @param v1 Vector to substract from.
 * @param v2 Vector to substract.
 */
void Vector_substract(Vector v1, Vector v2);

/**
 * Gets the squared length of the vector.
 * @param vect Vector from which get the length.
 * @return Squared length of the vector.
 */
int Vector_squaredLength(Vector vect);

/**
 * Tests the equality of two vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return 1 if they are equal, 0 otherwise.
 */
int Vector_equal(Vector v1, Vector v2);

#endif