#include <stdio.h>

int main(void) {
	int c;
	int floor = 0;
	int basement_step = 0;
	int step = 0;

	while ((c = getc(stdin)) != EOF) {
		if (c == '(')
			floor++;
		else if (c == ')')
			floor--;
		step++;
		if (basement_step == 0 && floor < 0)
			basement_step = step;
	}
	printf("part one: %d\n", floor);
	printf("part two: %d\n", basement_step);
	return 0;
}
