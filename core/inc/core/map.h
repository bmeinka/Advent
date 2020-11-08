#ifndef CORE_MAP
#define CORE_MAP

#include "std.h"
#include "fn.h"

typedef struct map *Map;

/* create a new map
 *
 * `key_size` is the size in bytes allocated for a key.
 * `value_size` is the size in bytes allocated for a value.
 * `hash` is the hash function used to hash the key to a `uint64_t`.
 * `cmp` is a comparison function to resolve hash collisions.
 *
 * returns a Map object which must be destroyed with `map_destroy`.
 */
Map map_create(size_t key_size, size_t val_size, HashFn hash, CmpFn cmp);

/* destroy a map, releasing all resources that it uses */
void map_destroy(Map map);

/* set the value for a key
 *
 * `map` is the map objrct the key and value will be added to.
 * `key` is the key to use for lookups.
 * `value` is the value to associate with the key.
 *
 * if the key is already present, the value will be copied, overwriting the
 * previous value. if the key is not found, it will be added. both key and value
 * data will be copied into (newly) allocated memory.
 *
 * returns true upon success and false if any memory allocation errors occur.
 */
bool map_set(Map map, void *key, void *value);

/* get the value associated with a key
 *
 * `map` is the map object containing the key and value.
 * `key` is the key to use for looking up a value.
 * `value` is the pointer to be filled with the value data.
 *
 * returns true if the item is found or false if it is not found.
 */
bool map_get(Map map, void *key, void *value);

/* remove an item from the map
 *
 * `map` is the map object to remove the item from.
 * `key` is the key used to look up the item
 *
 * returns true if an item was removed and false if the item was not found.
 */
bool map_remove(Map map, void *key);

#endif
