#include <core/std.h>

#define SIZE 1000

#define alloc_2d(arr, n) do {\
	arr = calloc(n, sizeof(*arr));\
	for (size_t i = 0; i < n; i++)\
		arr[i] = calloc(n, sizeof(**arr));\
} while(0)

#define free_2d(arr, n) do {\
	for (size_t i = 0; i < n; i++)\
		free(arr[i]);\
	free(arr);\
} while(0)

#define sum_2d(arr, v, n) do {\
	(v) = 0;\
	for (size_t x = 0; x < n; x++)\
		for (size_t y = 0; y < n; y++)\
			(v) += arr[x][y];\
} while (0)

enum operation {
	ON,
	OFF,
	TOGGLE
};

struct instruction {
	size_t min_x;
	size_t max_x;
	size_t min_y;
	size_t max_y;
	enum operation operation;
};

static void execute( struct instruction inst, bool **p1, uint64_t **p2) {
	for (size_t x = inst.min_x; x <= inst.max_x; x++) {
		for (size_t y = inst.min_y; y <= inst.max_y; y++) {
			if (inst.operation == ON) {
				p1[x][y] = true;
				p2[x][y]++;
			}
			else if (inst.operation == OFF) {
				p1[x][y] = false;
				if (p2[x][y] > 0)
					p2[x][y]--;

			}
			else if (inst.operation == TOGGLE) {
				p1[x][y] = !p1[x][y];
				p2[x][y] += 2;
			}
		}
	}
}

static struct instruction parse(const char *str) {
	struct instruction inst = {0};
	if (strstr(str, "turn on")) {
		inst.operation = ON;
		str += 8;
	}
	else if (strstr(str, "turn off")) {
		inst.operation = OFF;
		str += 9;
	}
	else if (strstr(str, "toggle")) {
		inst.operation = TOGGLE;
		str += 7;
	}
	sscanf(str, "%zu,%zu through %zu,%zu",
	       &inst.min_x, &inst.min_y, &inst.max_x, &inst.max_y);
	return inst;
}

int main(void) {
	// allocate light arrays
	bool **p1 = calloc(SIZE, sizeof(bool *));
	uint64_t **p2 = calloc(SIZE, sizeof(uint64_t *));
	for (size_t i = 0; i < SIZE; i++) {
		p1[i] = calloc(SIZE, sizeof(bool));
		p2[i] = calloc(SIZE, sizeof(uint64_t));
	}

	// parse and follow the instructions
	char line[1024];
	struct instruction inst;
	while (fgets(line, sizeof(line), stdin)) {
		inst = parse(line);
		execute(inst, p1, p2);
	}

	// calculate the sum of the light array values
	int total_p1 = 0, total_p2 = 0;
	for (size_t x = 0; x < SIZE; x++) {
		for (size_t y = 0; y < SIZE; y++) {
			total_p1 += p1[x][y];
			total_p2 += p2[x][y];
		}
	}

	// free the light array memory
	for (size_t i = 0; i < SIZE; i++) {
		free(p1[i]);
		free(p2[i]);
	}
	free(p1);
	free(p2);

	// report the answer
	printf("part one: %d\n", total_p1);
	printf("part two: %d\n", total_p2);
	return 0;
}
