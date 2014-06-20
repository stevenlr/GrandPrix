/**
 * @file List.c
 * @brief Doubly linked list implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "include/macros.h"
#include "include/List.h"
#include "include/GCRegistry.h"

/**
 * List node.
 */
struct Node {
	void* ptr;			//!< Pointer to the data.
	struct Node* next;	//!< Next node.
	struct Node* prev;	//!< Previous node.
};

typedef struct Node* Node;

static GarbageCollector nodesGC = NULL;

/**
 * Doubly linked list.
 */
struct List {
	int size;			//!< Number of elements in the list.
	Node head;			//!< Pointer to the first element of the list.
	Node tail;			//!< Pointer to the last element of the list.
	Node current;		//!< Pointer to the current node.
};

void List_initGC()
{
	nodesGC = GC_new(32, sizeof(struct Node));
	GCRegistry_add(&nodesGC);
}

static Node Node_new()
{
	Node node = GC_alloc(nodesGC);

	if(node == NULL)
	{
		LOGERR("Couldn't allocate node.");
		return NULL;
	}

	node->ptr = NULL;
	node->next = NULL;
	node->prev = NULL;

	return node;
}

static void Node_delete(Node node)
{
	assert(node);
	GC_free(nodesGC, node);
}

List List_new()
{
	List list = malloc(sizeof(struct List));

	if(list == NULL)
	{
		LOGERR("Couldn't allocate list.");
		return NULL;
	}

	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	list->current = NULL;

	return list;
}

void List_delete(List list)
{
	assert(list);

	List_head(list);

	if(!List_isEmpty(list))
		LOGWARNING("Deleting a non-empty list");

	while(!List_isEmpty(list))
		List_remove(list);

	free(list);
}

int List_getSize(List list)
{
	assert(list);
	return list->size;
}

int List_isEmpty(List list)
{
	assert(list);

	return list->head == NULL;
}

int List_isTail(List list)
{
	assert(list);

	if(List_isEmpty(list))
		return 1;

	return list->current == list->tail;
}

void List_head(List list)
{
	assert(list);

	list->current = list->head;
}

void List_tail(List list)
{
	assert(list);

	list->current = list->tail;
}

int List_hasNext(List list)
{
	assert(list);

	if(list->current == NULL)
		return 0;

	return list->current->next != NULL;
}

int List_next(List list)
{
	assert(list);

	if(!List_hasNext(list))
		return 0;

	list->current = list->current->next;
	return 1;
}

void* List_getCurrent(List list)
{
	assert(list);

	if(list->current == NULL)
		return NULL;

	return list->current->ptr;
}

void List_insert(List list, void* ptr)
{
	Node node, tail;

	assert(list);

	node = Node_new();

	if(node == NULL)
		return;

	node->ptr = ptr;

	tail = list->tail;
	list->size++;

	if(tail == NULL)
	{
		list->current = node;
		list->head = node;
		list->tail = node;
		return;
	}

	list->tail = node;
	tail->next = node;
	node->prev = tail;
}

void List_remove(List list)
{
	Node node, prev, next;

	assert(list);

	node = list->current;

	if(node == NULL)
		return;

	list->size--;
	prev = node->prev;
	next = node->next;

	Node_delete(node);

	if(prev != NULL)
	{
		if(next != NULL)
		{
			prev->next = next;
			next->prev = prev;
			list->current = next;
			return;
		}

		prev->next = NULL;
		list->tail = prev;
		list->current = prev;
		return;
	}

	if(next != NULL)
	{
		next->prev = NULL;
		list->head = next;
		list->current = next;
		return;
	}

	list->head = NULL;
	list->tail = NULL;
	list->current = NULL;
}

void List_empty(List list)
{
	assert(list);

	List_head(list);

	while(!List_isEmpty(list))
		List_remove(list);
}


void List_insertSorted(List list, void* ptr, List_compare ptrList_compare){
	Node node, current;
	int beforeTail = 1;
	int isHead = 1;

	assert(list);

	node = Node_new();

	if(node == NULL)
		return;

	node->ptr = ptr;

	if(list->current == NULL)
	{
		list->current = node;
		list->head = node;
		list->tail = node;
		return;
	}

	List_head(list);
	if(list->current->ptr == ptr)
		List_remove(list);

	while ((*ptrList_compare)(list->current->ptr, ptr) && beforeTail) {
		isHead = 0;
		beforeTail = List_next(list);
		if(list->current->ptr == ptr)
			List_remove(list);
	}

	current = list->current;
	list->size++;

	if(beforeTail){
		node->next = current;
		if(isHead)
			list->head = node;
		else{
			node->prev = current->prev;
			current->prev->next = node;
		}
		current->prev = node;
	}
	else{
		current->next = node;
		node->prev = current;
		list->tail = node;
	}
}

void List_insertHead(List list, void* ptr){
	Node node, head;

	assert(list);

	node = Node_new();

	if(node == NULL)
		return;

	node->ptr = ptr;

	head = list->head;
	list->size++;

	if(head == NULL)
	{
		list->current = node;
		list->head = node;
		list->tail = node;
		return;
	}

	list->head = node;
	head->prev = node;
	node->next = head;
}

List List_copy(List list){
	List listCopy = List_new();
	void* ptr;
	int i;

	List_foreach(list, ptr, i){
		List_insert(listCopy,ptr);
	}

	return listCopy;
}

int List_belongTo(List list, void* ptr, List_equal ptrList_equal){
	void* ptr1;
	int i;

	List_foreach(list, ptr1, i){
		if( (*ptrList_equal)(ptr1,ptr) )
			return 1;
	}
	return 0;
}
