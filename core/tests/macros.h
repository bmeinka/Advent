#ifndef TEST_MACROS
#define TEST_MACROS

#include <stdio.h>

#define assert(t, m) if (!(t)) return m

#define run_test(t) do {\
	char *m = t();\
	if (m) {\
		printf("%s: %s\n", #t, m);\
		return 1;\
	}\
} while (0)

#endif
