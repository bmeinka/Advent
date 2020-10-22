#include <stdlib.h>
#include <core/set.h>
#include <core/list.h>
#include <string.h>

struct set {
	size_t item_size;
	size_t bits;
	size_t count;
	HashFn hash;
	CmpFn cmp;
	List *buckets;
};

static size_t get_bucket_index(Set set, void *item) {
	(void)item;
	(void)set;
	return 0;
}

Set set_create(size_t item_size, HashFn hash, CmpFn cmp) {
	Set set = calloc(1, sizeof(struct set));
	if (!set) return NULL;
	set->item_size = item_size;
	set->hash = hash;
	set->cmp = cmp;
	set->buckets = calloc(1, sizeof(List));
	return set;
}

void set_destroy(Set set) {
	for (size_t i = 0; i < 1; i++) {
		List head = set->buckets[i];
		while (head) {
			if (head->item)
				free(head->item);
			List next = head->next;
			free(head);
			head = next;
		}
	}
	free(set->buckets);
	free(set);
}

bool set_add(Set set, void *item) {
	size_t i = get_bucket_index(set, item);
	if (!set->buckets[i]) {
		set->buckets[i] = calloc(1, sizeof(struct list_node));
		if (!set->buckets[i])
			return false;
	}
	List head = set->buckets[i];
	while (head->item) {
		if (set->cmp(head->item, item))
			return true;
		if (!head->next) {
			head->next = calloc(1, sizeof(struct list_node));
			if (!head->next)
				return false;
		}
		head = head->next;
	}
	head->item = malloc(set->item_size);
	if (!head->item)
		return false;
	memcpy(head->item, item, set->item_size);
	set->count++;
	return true;
}

bool set_contains(Set set, void *item) {
	size_t i = get_bucket_index(set, item);
	List head = set->buckets[i];
	while (head && head->item) {
		if (set->cmp(head->item, item))
			return true;
		head = head->next;
	}
	return false;
}

size_t set_count(Set set) {
	return set->count;
}
