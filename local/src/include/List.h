/**
 * @file List.h
 * @brief Doubly linked list header.
 */

#ifndef DEF_LIST
#define DEF_LIST

#define List_foreach(l, e, i)	\
 	List_head(l);				\
 	for(i = 0, e = List_getCurrent(l); i < List_getSize(l); i++, List_next(l), e = List_getCurrent(l))


typedef struct List* List;

/**
 * Initializes the garbage collector for the nodes.
 */
void List_initGC();

/**
 * Creates a new list.
 * @return The list created.
 */
List List_new();

/**
 * Desallocates a list.
 * @param list The list to destroy.
 */
void List_delete(List list);

/**
 * Gets the size of the list.
 * @param list The list from which the size has to be retrieved.
 * @return The size of the list.
 */
int List_getSize(List list);

/**
 * Tests the emptiness of a list.
 * @param list The list to test against.
 * @return 1 if the list is empty, 0 otherwise.
 */
int List_isEmpty(List list);

/**
 * Tests if we are at the end of the list.
 * @param list The list to test against.
 * @return 1 if the list we are, 0 otherwise.
 */
int List_isTail(List list);

/**
 * Rewinds the current pointer to the beginning of the list.
 * @param list The list to modify.
 */
void List_head(List list);

/**
 * Goes to the end of the list.
 * @param list The list to modify.
 */
void List_tail(List list);

/**
 * Checks if the list has a next node.
 * @param list The list to check.
 * @return 1 if there is a next node, 0 otherwise.
 */
int List_hasNext(List list);

/**
 * Goes to the next node.
 * @param list The list to explore.
 * @return 1 if it succeeded, 0 otherwise.
 */
int List_next(List list);

/**
 * Gets the currently pointed data in the list.
 * @param list List from which the data has to be retrieved.
 * @param Currently pointed data.
 */
void* List_getCurrent(List list);

/**
 * Inserts an element at the end of the list.
 * @param list The list in which the element has to be inserted.
 * @param ptr The pointer to insert.
 */
void List_insert(List list, void* ptr);

/**
 * Removes the current node from the list.
 * @param list The list from which the node has to be removed.
 */
void List_remove(List list);

/**
 * Empties a list.
 * @param list List to empty.
 */
void List_empty(List list);

#endif