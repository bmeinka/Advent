#include <core/std.h>

static void increment(char *s) {
	size_t i = strlen(s) - 1;
	s[i]++;
	while (s[i] > 'z') {
		s[i] = 'a';
		s[--i]++;
	}
}

static bool has_seq(const char *s) {
	for (size_t i = 0; i < strlen(s) - 2; i++) {
		if (s[i]+1 == s[i+1] && s[i]+2 == s[i+2])
			return true;
	}
	return false;
}

static bool has_valid_letters(const char *s) {
	char bad[3] = {'i', 'l', 'o'};
	for (size_t i = 0; i < 3; i++) {
		if (strchr(s, bad[i]))
			return false;
	}
	return true;
}

static bool has_pair(const char *s) {
	int n = 0;
	for (size_t i = 0; i < strlen(s) - 1; i++) {
		if (s[i] == s[i+1]) {
			n++;
			i++;
		}
	}
	return n > 1;
}

static bool is_valid(const char *s) {
	return has_seq(s) && has_pair(s) && has_valid_letters(s);
}

int main(void) {
	char pwd[9] = {0};
	fgets(pwd, 9, stdin);

	increment(pwd);
	while (!is_valid(pwd)) increment(pwd);
	printf("part one: %s\n", pwd);

	increment(pwd);
	while (!is_valid(pwd)) increment(pwd);
	printf("part two: %s\n", pwd);

	return 0;
}
