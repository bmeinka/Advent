#include "core/fn.h"
#include <core/std.h>
#include <core/map.h>
#include <core/iter.h>

int routes[8][8];
int shortest = INT_MAX;
int longest = -1;

static uint64_t hash(void *k) {
	uint64_t h = 0;
	uint8_t *s = k;
	for (size_t i = 0; s[i] != 0; i++) {
		h += s[i] << ((i % 8) * 8);
	}
	return h;
}

static bool cmp(char *a, char *b) {
	return strcmp(a, b) == 0;
}

static void parse_routes(void) {
	char line[1024];
	char from[32], to[32];
	int distance;
	int index_from, index_to, next = 0;
	Map places = map_create(sizeof(char) * 32, sizeof(int), &hash, (CmpFn)&cmp);

	while (fgets(line, sizeof(line), stdin)) {
		sscanf(line, "%s to %s = %d", from, to, &distance);
		if (!map_get(places, from, &index_from)) {
			map_set(places, from, &next);
			index_from = next++;
		}
		if (!map_get(places, to, &index_to)) {
			map_set(places, to, &next);
			index_to = next++;

		}
		routes[index_from][index_to] = distance;
		routes[index_to][index_from] = distance;
	}

	map_destroy(places);
}

static void check_route(int *route) {
	int distance = 0;
	for (int i = 1; i < 8; i++) {
		int from = route[i - 1];
		int to = route[i];
		distance += routes[from][to];
	}
	if (distance < shortest)
		shortest = distance;
	if (distance > longest)
		longest = distance;
}

int main(void) {
	parse_routes();

	int indices[] = {0, 1, 2, 3, 4, 5, 6, 7};
	permute(indices, sizeof(int), 8, (CallbackFn)&check_route);

	printf("part one: %d\n", shortest);
	printf("part two: %d\n", longest);
	return 0;
}
