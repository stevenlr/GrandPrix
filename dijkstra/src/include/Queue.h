/**
 * @file Queue.h
 * @brief Queue headers.
 */

#ifndef DEF_QUEUE
#define DEF_QUEUE

#include "List.h"

typedef List Queue;

/**
 * Creates a new queue.
 * @return The created queue.
 */
Queue Queue_new();

/**
 * Deallocates a queue.
 * @param queue The queue to deallocate.
 */
void Queue_delete(Queue queue);

/**
 * Inserts an element in the queue.
 * @param queue The queue to insert in.
 * @param ptr The pointer to insert.
 */
void Queue_insert(Queue queue, void* ptr);

/**
 * Gets the first element in the queue.
 * @param queue The queue to peek in. NULL if the queue was empty.
 */
void* Queue_peek(Queue queue);

/**
 * Gets the first element in the queue and deletes it from it.
 * @param queue The queue to dequeue from.
 * @return The dequeues pointer. NULL if the queue was empty.
 */
void* Queue_dequeue(Queue queue);

/**
 * Checks if a queue is empty.
 * @param queue The queue to check against.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int Queue_isEmpty(Queue queue);

#endif