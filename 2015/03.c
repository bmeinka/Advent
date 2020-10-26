#include <core/std.h>
#include <core/set.h>

enum direction {
	UP = '^',
	DOWN = 'v',
	LEFT = '<',
	RIGHT = '>'
};

struct point {
	int32_t x, y;
};

struct santa {
	Set set;
	struct point position;
};

static bool cmp(void *a, void *b) {
	struct point first = *(struct point *)a;
	struct point second = *(struct point *)b;
	return first.x == second.x && first.y == second.y;
}

static uint64_t hash(void *item) {
	return *(uint64_t *)item;
}

static struct santa santa_init() {
	struct point position = {0, 0};
	Set set = set_create(sizeof(position), &hash, &cmp);
	return (struct santa){.set=set, .position=position};
}

static void move(struct point *position, int direction) {
	switch (direction) {
		case UP: position->y++; break;
		case DOWN: position->y--; break;
		case LEFT: position->x--; break;
		case RIGHT: position->x++; break;
	}
}

int main (void) {
	int c;
	int i = 0;
	struct santa p1 = santa_init();
	struct santa p2_santa = santa_init();
	struct santa p2_robot = santa_init();

	while ((c = getc(stdin)) != EOF) {
		set_add(p1.set, &p1.position);
		set_add(p2_santa.set, &p2_santa.position);
		set_add(p2_robot.set, &p2_robot.position);
		move(&p1.position, c);
		if (i % 2 == 0)
			move(&p2_santa.position, c);
		else
			move(&p2_robot.position, c);
		i = (i + 1) % 2;
	}
	Set p2 = set_union(p2_santa.set, p2_robot.set);
	printf("part one: %ld\n", set_count(p1.set));
	printf("part two: %ld\n", set_count(p2));
	return 0;
}
