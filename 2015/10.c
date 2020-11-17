#include <core/std.h>

static char *say(char *s) {
	char *t = malloc(strlen(s) * 2);

	size_t i = 0, j = 0;
	while(s[i] != '\0') {
		size_t n = 0;
		while (s[i+n] == s[i]) n++;
		t[j++] = n + '0';
		t[j++] = s[i];
		i += n;
	}
	free(s);
	return t;
}

int main(void) {
	char line[1024];
	fgets(line, sizeof(line), stdin);
	line[strcspn(line, "\r\n")] = '\0';

	char *s = malloc(strlen(line) + 1);
	strcpy(s, line);
	for (int i = 0; i < 40; i++)
		s = say(s);
	printf("part one: %zu\n", strlen(s));
	for (int i = 0; i < 10; i++)
		s = say(s);
	printf("part two: %zu\n", strlen(s));
	return 0;
}
