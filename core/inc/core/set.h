#ifndef CORE_SET
#define CORE_SET

#include <stddef.h>
#include <stdbool.h>
#include "fn.h"

typedef struct set *Set;

/*
 * create a new set
 *
 * the set will be initialized to hold items of the given size, and will use
 * the provided hash and cmp functions to hash and compare items.
 */
Set set_create(size_t item_size, HashFn hash, CmpFn cmp);

/*
 * free all resources used by a set
 *
 * this will de-allocate all items contained in the set and the set itself. the
 * provided set (which is a pointer) will be invalidated.
 */
void set_destroy(Set set);

/*
 * add an item to a set
 *
 * the item is assumed to be the correct size. if the item is the wrong
 * size, a segmentation fault is possible.
 *
 * returns true if the item was added or already present. if a memory
 * allocation error occurs, returns false.
 */
bool set_add(Set set, void *item);

/*
 * remove an item from a set
 *
 * the item is assumed to be the correct size. if the item is the wrong
 * size, a segmentation fault is possible.
 *
 * returns true if the item was removed, or false if it was not found.
 */
bool set_remove(Set set, void *item);

/*
 * determine if a set has an item in it
 * 
 * the item is assumed to be the correct size. if the item is the wrong
 * size, a segmentation fault is possible.
 */
bool set_contains(Set set, void *item);

/*
 * determine how many items are held within a set
 */
size_t set_count(Set set);

#endif
