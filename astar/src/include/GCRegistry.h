/**
 * @file GCRegistry.h
 * @brief Garbage collector registry headers.
 */

#ifndef DEF_GCREGISTRY
#define DEF_GCREGISTRY

#include "GarbageCollector.h"

/**
 * Adds a garbage collector to the registry.
 * @param gc Garbage collector to be added.
 */
void GCRegistry_add(GarbageCollector* gc);

/**
 * Flushes all garbage collectors in registry.
 */
void GCRegistry_flushAll();

/**
 * Deletes all garbage collectors in registry.
 */
void GCRegistry_deleteAll();

#endif