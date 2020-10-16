#include <stdio.h>

int main(void) {
	int c;
	int position = 0;
	int basement_step = 0;
	int step = 0;

	while ((c = getc(stdin)) != EOF) {
		if (c == '(')
			position++;
		else if (c == ')')
			position--;
		step++;
		if (basement_step == 0 && position < 0)
			basement_step = step;
	}
	printf("part one: %d\n", position);
	printf("part two: %d\n", basement_step);
	return 0;
}
