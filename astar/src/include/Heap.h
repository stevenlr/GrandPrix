/**
 * @file Heap.h
 * @brief Heap header.
 */

#ifndef DEF_HEAP
#define DEF_HEAP

typedef struct Heap* Heap;

/**
 * Function used to compare data in the heap.
 * @param a object a
 * @param b object b
 * @return 0 if equals, 1 if a > b, -1 if a < b.
 */
typedef int (*HeapCompareFct)(void*, void*);

/**
 * Allocated a new heap.
 * @param fct Function used to compare data in the heap.
 * @return The created heap.
 */
Heap Heap_new(HeapCompareFct fct);

/**
 * Deletes a heap.
 * @param heap The heap to delete.
 */
void Heap_delete(Heap heap);

/**
 * Insert an element in the heap.
 * @param heap
 * @param data
 */
void Heap_insert(Heap heap, void* data);

/**
 * Extracts the minimum element of the heap.
 * @param heap
 * @return The minimum element.
 */
void* Heap_extractMin(Heap heap);

/**
 * Tests wether the heap is empty or not.
 * @param heap
 * @return 1 if the heap is empty, 0 otherwise.
 */
int Heap_isEmpty(Heap heap);

#endif