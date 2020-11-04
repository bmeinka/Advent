#include <core/std.h>

static bool nice_p1(const char *str) {
	// check for naughty words
	if (strstr(str, "ab")) return false;
	if (strstr(str, "cd")) return false;
	if (strstr(str, "pq")) return false;
	if (strstr(str, "xy")) return false;

	// count vowels and look for double letters
	static char *vowels = "aeiou";
	size_t count = 0;
	bool has_double = false;
	for (size_t i = 0; i < strlen(str); i++) {
		if (str[i] == str[i+1])
			has_double = true;
		if (strchr(vowels, str[i]))
			count++;
	}
	if (count < 3) return false;
	return has_double;
}

static bool nice_p2(const char *str) {
	char pair[3] = {0};
	bool has_gap = false;
	bool has_pair = false;
	for (size_t i = 0; i < strlen(str) - 2; i++) {
		if (str[i] == str[i+2])
			has_gap = true;
		strncpy(pair, &str[i], 2);
		if (strstr(&str[i+2], pair))
			has_pair = true;
	}
	return has_gap && has_pair;
}

int main(void) {
	char line[1024];
	int p1 = 0, p2 = 0;
	while (fgets(line, sizeof(line), stdin)) {
		if (nice_p1(line)) p1++;
		if (nice_p2(line)) p2++;
	}
	printf("part one: %d\n", p1);
	printf("part two: %d\n", p2);
	return 0;
}
