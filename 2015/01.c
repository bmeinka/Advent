#include <core/std.h>

enum direction {
	UP = '(',	
	DOWN = ')'	
};

int main(void) {
	int c;
	int position = 0;
	int basement_step = 0;
	int step = 0;

	while ((c = getc(stdin)) != EOF) {
		if (c == UP)
			position++;
		else if (c == DOWN)
			position--;
		step++;
		if (basement_step == 0 && position < 0)
			basement_step = step;
	}
	printf("part one: %d\n", position);
	printf("part two: %d\n", basement_step);
	return 0;
}
