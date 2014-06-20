#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "include/macros.h"
#include "include/Heap.h"

/**
 * A heap.
 */
struct Heap {
	int size;				//!< Total size of the heap.
	int lastRowSize;		//!< Size of the last row.
	int used;				//!< Size used.
	void** data;			//!< Heap data.
	HeapCompareFct cmpfct;	//!< Function used to compare data.
};

Heap Heap_new(HeapCompareFct fct)
{
	Heap heap;

	heap = (Heap) malloc(sizeof(struct Heap));

	if(heap == NULL)
	{
		LOGERR("Couldn't allocate heap.")
		return NULL;
	}

	heap->size = 1;
	heap->lastRowSize = 1;
	heap->used = 0;
	heap->cmpfct = fct;
	heap->data = (void**) malloc(sizeof(void*));

	if(heap->data == NULL)
	{
		LOGERR("Couldn't allocate heap data.");
		free(heap);
		return NULL;
	}

	return heap;
}

void Heap_delete(Heap heap)
{
	free(heap->data);
	free(heap);
}

void Heap_insert(Heap heap, void* data)
{
	int index = heap->used;
	void *tmp;

	if(heap->size == heap->used)
	{
		heap->lastRowSize *= 2;
		heap->size += heap->lastRowSize;
		heap->data = realloc(heap->data, heap->size * sizeof(void*));

		if(heap->data == NULL)
		{
			LOGERR("Couldn't reallocate heap.");
			return;
		}
	}

	heap->used++;

	heap->data[index] = data;

	if(index == 0)
		return;

	while(heap->cmpfct(data, heap->data[(int) index / 2]) < 0)
	{
		tmp = heap->data[(int) index / 2];
		heap->data[(int) index / 2] = heap->data[index];
		heap->data[index] = tmp;

		index /= 2;

		if(index == 0)
			break;
	}
}

void* Heap_extractMin(Heap heap)
{
	void *result, *tmp;
	int index = 0;

	if(heap->used == 0)
		return NULL;

	result = heap->data[0];
	heap->data[0] = heap->data[heap->used - 1];
	heap->used--;

	while(1)
	{
		if(index * 2 + 1 < heap->used && heap->cmpfct(heap->data[index], heap->data[index * 2 + 1]) > 0
			&& heap->cmpfct(heap->data[index * 2 + 1], heap->data[index * 2 + 2]) <= 0)
		{
			tmp = heap->data[index * 2 + 1];
			heap->data[index * 2 + 1] = heap->data[index];
			heap->data[index] = tmp;
			index = index * 2 + 1;

		}
		else if(index * 2 + 2 < heap->used && heap->cmpfct(heap->data[index], heap->data[index * 2 + 2]) > 0)
		{
			tmp = heap->data[index * 2 + 2];
			heap->data[index * 2 + 2] = heap->data[index];
			heap->data[index] = tmp;
			index = index * 2 + 2;
		}
		else
			break;
	}

	return result;
}

int Heap_isEmpty(Heap heap)
{
	return heap->used == 0;
}