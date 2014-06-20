/**
 * @file GarbageCollector.h
 * @brief Headers of a garbage collector.
 */

#ifndef DEF_GARBAGECOLLECTOR
#define DEF_GARBAGECOLLECTOR

typedef struct GarbageCollector* GarbageCollector;

/**
 * Created a garbage collector.
 * @param initialSize Initial size of the garbage collector.
 * @param size Size of an element in the garbage collector.
 * @return The created garbage collector.
 */
GarbageCollector GC_new(int initialSize, size_t size);

/**
 * Deletes a garbage collector. Does NOT free the remaining elements. See GC_flush.
 * @param gc Garbage collector to delete.
 */
void GC_delete(GarbageCollector gc);

/**
 * Removes and deallocate all data contained in the garbage collector.
 * @param gc Garbage collector to flush.
 */
void GC_flush(GarbageCollector gc);

/**
 * Allocated an element from the garbage collector.
 * @param gc Garbage collector from which the element is to be allocated.
 */
void* GC_alloc(GarbageCollector gc);

/**
 * Deallocated an element in the garbage collector.
 * @param gc Garbage collector in which the element is to be deallocated.
 * @param ptr Object to deallocate.
 */
void GC_free(GarbageCollector gc, void* ptr);

#endif