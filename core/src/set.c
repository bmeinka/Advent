#include <stdlib.h>
#include <core/set.h>
#include <string.h>

// 2^64 / phi
const uint64_t golden_ratio = 11400714819323197439U;

struct set_node {
	void *item;
	struct set_node *next;
};

struct set {
	size_t item_size;
	size_t bits;
	size_t count;
	HashFn hash;
	CmpFn cmp;
	struct set_node **buckets;
};

static size_t get_bucket_index(Set set, void *item) {
	uint64_t hash = set->hash(item) * golden_ratio;
	return hash >> (64 - set->bits);
}

Set set_create(size_t item_size, HashFn hash, CmpFn cmp) {
	Set set = calloc(1, sizeof(struct set));
	if (!set) return NULL;
	set->item_size = item_size;
	set->hash = hash;
	set->cmp = cmp;
	set->bits = 8;
	set->buckets = calloc(1 << 8, sizeof(struct set_node *));
	return set;
}

void set_destroy(Set set) {
	for (size_t i = 0; i < 1 << set->bits; i++) {
		struct set_node *head = set->buckets[i];
		while (head) {
			if (head->item)
				free(head->item);
			struct set_node *next = head->next;
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
		set->buckets[i] = calloc(1, sizeof(struct set_node));
		if (!set->buckets[i])
			return false;
	}
	struct set_node *head = set->buckets[i];
	while (head->item) {
		if (set->cmp(head->item, item))
			return true;
		if (!head->next) {
			head->next = calloc(1, sizeof(struct set_node));
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
	struct set_node *head = set->buckets[i];
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

Set set_union(Set a, Set b) {
	if (a->item_size != b->item_size) return NULL;

	Set set = set_create(a->item_size, a->hash, a->cmp);
	if (!set) return NULL;

	for (size_t i = 0; i < 1 << a->bits; i++) {
		struct set_node *head = a->buckets[i];
		while (head) {
			if (head->item)
				set_add(set, head->item);
			head = head->next;
		}
	}
	for (size_t i = 0; i < 1 << b->bits; i++) {
		struct set_node *head = b->buckets[i];
		while (head) {
			if (head->item)
				set_add(set, head->item);
			head = head->next;
		}
	}
	return set;
}
