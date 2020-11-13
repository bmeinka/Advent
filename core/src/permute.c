#include "core/fn.h"
#include <core/std.h>
#include <core/iter.h>

struct permute_state {
	void *arr;
	size_t size;
	size_t n;
	CallbackFn callback;
};

/* swap elements at the given indices */
static void swap(struct permute_state *p, size_t a, size_t b) {
	char *t = malloc(p->size);
	char *offset_a = (char *)p->arr + (a * p->size);
	char *offset_b = (char *)p->arr + (b * p->size);

	memcpy(t, offset_a, p->size);
	memcpy(offset_a, offset_b, p->size);
	memcpy(offset_b, t, p->size);

	free(t);
}

/* recursive implementation of Heap's Algorithm
 *
 * see: https://en.wikipedia.org/wiki/Heap%27s_algorithm
 */
static void generate(struct permute_state *p, size_t k) {
	if (k == 1) {
		p->callback(p->arr);
		return;
	}
	generate(p, k - 1);
	for (size_t i = 0; i < k - 1; i++) {
		if (k % 2 == 0)
			swap(p, i, k - 1);
		else
			swap(p, 0, k - 1);
		generate(p, k - 1);
	}
}

void permute(void *arr, size_t elem_size, size_t n, CallbackFn cb) {
	struct permute_state state = {
		.arr = arr,
		.size = elem_size,
		.n = n,
		.callback = cb
	};
	generate(&state, state.n);
}
