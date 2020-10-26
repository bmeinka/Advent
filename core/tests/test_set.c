#include "macros.h"
#include <core/std.h>
#include <core/set.h>

uint64_t hash_int(void *item) {
	int i = *(int *)item;
	return (uint64_t)i;
}

bool cmp_int(void *a, void *b) {
	return (*(int *)a) == (*(int *)b);
}

static char *test_set_add_item() {
	int i, j;
	i = j = 33;
	Set set = set_create(sizeof(int), &hash_int, &cmp_int);

	set_add(set, &i);
	bool contains = set_contains(set, &j);

	assert(contains, "set does not contain added value");

	set_destroy(set);
	return NULL;
}

static char *test_set_does_not_contain() {
	int i = 33;
	int j = 34;
	Set set = set_create(sizeof(int), &hash_int, &cmp_int);

	bool contains = set_contains(set, &i);
	contains |= set_contains(set, &j);

	assert(!contains, "empty set contains values");

	set_destroy(set);
	return NULL;
}

static char *test_set_add_no_duplicates() {
	int i, j;
	i = j = 33;
	Set set = set_create(sizeof(int), &hash_int, &cmp_int);

	set_add(set, &i);
	assert(set_count(set) == 1, "adding item did not increase count");
	set_add(set, &j);
	assert(set_count(set) == 1, "re-adding item changed count");

	set_destroy(set);
	return NULL;
}

static char *test_set_has_count() {
	Set set = set_create(sizeof(int), &hash_int, &cmp_int);

	for (int i = 1; i < 10; i++) {
		set_add(set, &i);
		assert(set_count(set) == (size_t)i, "bad count");
	}

	set_destroy(set);
	return NULL;
}

static char * test_set_union() {
	int i;
	Set a = set_create(sizeof(int), &hash_int, &cmp_int);
	Set b = set_create(sizeof(int), &hash_int, &cmp_int);

	i = 33;
	set_add(a, &i);
	i = 34;
	set_add(a, &i);
	set_add(b, &i);
	i = 35;
	set_add(b, &i);
	Set c = set_union(a, b);

	assert(set_count(c) == 3, "union has wrong count");
	i = 33;
	assert(set_contains(c, &i), "union missing value");
	i = 34;
	assert(set_contains(c, &i), "union missing value");
	i = 35;
	assert(set_contains(c, &i), "union missing value");

	set_destroy(a);
	set_destroy(b);
	set_destroy(c);

	return NULL;
}

static char *test_set_union_bad_size_returns_null() {
	Set a = set_create(4, &hash_int, &cmp_int);
	Set b = set_create(2, &hash_int, &cmp_int);
	Set c = set_union(a, b);

	assert(c == NULL, "invalid union created");

	set_destroy(a);
	set_destroy(b);

	return NULL;
}

int main(void) {
	run_test(test_set_add_item);
	run_test(test_set_does_not_contain);
	run_test(test_set_add_no_duplicates);
	run_test(test_set_has_count);
	run_test(test_set_union);
	run_test(test_set_union_bad_size_returns_null);
	return 0;
}
