#include <core/std.h>

static size_t unescaped(const char *s) {
	size_t n = 2; // surrounding quotes
	while ((s = strstr(s, "\\"))) {
		if (s[1] == 'x') {
			s += 4;
			n += 3;
		}
		else {
			s += 2;
			n += 1;
		}
	}
	return n;
}

static size_t escaped(const char *s) {
	size_t n = 2;
	while (s[strcspn(s, "\"\\")]) {
		s += strcspn(s, "\"\\") + 1;
		n += 1;
	}
	return n;
}

int main(void) {
	size_t p1 = 0, p2 = 0;

	char s[1024];
	while (fgets(s, sizeof(s), stdin)) {
		s[strcspn(s, "\r\n")] = '\0';
		p1 += unescaped(s);
		p2 += escaped(s); 
	}

	printf("part one: %zu\n", p1);
	printf("part two: %zu\n", p2);
	return 0;
}
