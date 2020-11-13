#include "macros.h"
#include <core/std.h>
#include <core/iter.h>

static int count[6] = {0};

static void callback(void *arr) {
	char *str = arr;
	if (strcmp(str, "abc") == 0)
		count[0]++;
	else if (strcmp(str, "acb") == 0)
		count[1]++;
	else if (strcmp(str, "bac") == 0)
		count[2]++;
	else if (strcmp(str, "bca") == 0)
		count[3]++;
	else if (strcmp(str, "cab") == 0)
		count[4]++;
	else if (strcmp(str, "cba") == 0)
		count[5]++;
}

static char *test_permute() {
	char str[4] = "abc";
	permute(str, sizeof(char), 3, &callback);

	for (int i = 0; i < 6; i++) {
		assert(count[i] > 0, "incomplete permutation set");
		assert(count[i] == 1, "duplicate permutations");
	}
	return NULL;
}

int main(void) {
	run_test(test_permute);
	return 0;
}
