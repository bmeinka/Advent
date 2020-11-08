#include "macros.h"
#include <core/std.h>
#include <core/map.h>

static uint64_t hash_int(void *item) {
	int i = *(int *)item;
	return (uint64_t)i;
}

static bool cmp_int(void *a, void *b) {
	return (*(int *)a) == (*(int *)b);
}

static char *test_map_set_and_get_item() {
	Map m = map_create(sizeof(int), sizeof(int), &hash_int, &cmp_int);
	int v, k = 1, a = 33, b = 34;

	map_set(m, &k, &a);
	map_get(m, &k, &v);
	assert(v == a, "get does not return the set value");

	map_set(m, &k, &b);
	map_get(m, &k, &v);
	assert(v == b, "get does not return updated value");

	map_destroy(m);
	return NULL;
}

static char *test_map_set_multiple_keys() {
	Map m = map_create(sizeof(int), sizeof(int), &hash_int, &cmp_int);
	int v, a = 1, b = 2;

	v = 33;
	map_set(m, &a, &v);
	v = 34;
	map_set(m, &b, &v);

	map_get(m, &a, &v);
	assert(v == 33, "first item is incorrect");
	map_get(m, &b, &v);
	assert(v == 34, "second item is incorrect");

	map_destroy(m);
	return NULL;
}

int main(void) {
	run_test(test_map_set_and_get_item);
	run_test(test_map_set_multiple_keys);
	return 0;
}
