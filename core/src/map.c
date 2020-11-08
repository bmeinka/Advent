#include <core/map.h>

// 2^64 / phi
const uint64_t golden_ratio = 11400714819323197439U;

struct map_node {
	void *key;
	void *val;
	struct map_node *next;
};

struct map {
	size_t key_size;
	size_t val_size;
	size_t bits;
	size_t count;
	HashFn hash;
	CmpFn cmp;
	struct map_node **buckets;
};


Map map_create(size_t key_size, size_t val_size, HashFn hash, CmpFn cmp) {
	Map map = calloc(1, sizeof(struct map));
	if (!map) return NULL;
	map->key_size = key_size;
	map->val_size = val_size;
	map->hash = hash;
	map->cmp = cmp;
	map->bits = 8;
	map->buckets = calloc(1 << 8, sizeof(struct map_node *));
	return map;
}

void map_destroy(Map map) {
	for (size_t i = 0; i < (1 << map->bits); i++) {
		struct map_node *head = map->buckets[i];
		while (head) {
			if (head->key) free(head->key);
			if (head->val) free(head->val);
			struct map_node *next = head->next;
			free(head);
			head = next;
		}
	}
	free(map->buckets);
	free(map);
}

static size_t get_bucket_index(Map map, void *item) {
	uint64_t hash = map->hash(item) * golden_ratio;
	return hash >> (64 - map->bits);
}

static struct map_node *get_node(Map map, void *key) {
	size_t i = get_bucket_index(map, key);
	if (!map->buckets[i]) {
		map->buckets[i] = calloc(1, sizeof(struct map_node));
		if (!map->buckets[i]) return NULL;
	}
	struct map_node *head = map->buckets[i];
	while (head->key) {
		if (map->cmp(head->key, key))
			return head;
		if (!head->next) {
			head->next = calloc(1, sizeof(struct map_node));
			if (!head->next) return NULL;
		}
		head = head->next;
	}
	head->key = malloc(map->key_size);
	if (!head->key) return NULL;
	memcpy(head->key, key, map->key_size);
	return head;
}

bool map_set(Map map, void *key, void *value) {
	struct map_node *node = get_node(map, key);
	if (!node) return false;
	if (!node->val) {
		node->val = malloc(map->val_size);
		if (!node->val) return false;
	}
	memcpy(node->val, value, map->val_size);
	map->count++;
	return true;
}

bool map_get(Map map, void *key, void *value) {
	size_t i = get_bucket_index(map, key);
	struct map_node *head = map->buckets[i];
	while (head && head->key) {
		if (map->cmp(head->key, key)) {
			memcpy(value, head->val, map->val_size);
			return true;
		}
		head = head->next;
	}
	return false;
}
