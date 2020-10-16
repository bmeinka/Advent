#include <stdio.h>
#include <stdlib.h>

struct box { int w, h, l; };

struct box parse_box(char *line) {
	struct box box;
	int a, b, c;
	if (sscanf(line, "%dx%dx%d\n", &a, &b, &c) != 3) {
		printf("error parsing box: %s\n", line);
		exit(1);
	}
	// ensure w*h is always the smallest face
	if (a < b) {
		box.w = a;
		box.h = b < c ? b : c;
		box.l = b < c ? c : b;
	} else 	{
		box.w = b;
		box.h = a < c ? a : c;
		box.l = a < c ? c : a;
	}
	return box;
}

int box_area(struct box b) {
	return ((b.w * b.h) + (b.w * b.l) + (b.h * b.l)) * 2;
}

int box_volume(struct box b) {
	return b.w * b.h * b.l;
}

void print_box(struct box box) {
	printf("Box(w=%d,h=%d,l=%d)\n", box.w, box.h, box.l);
}

int main(void) {
	char line[1024];

	int paper = 0;
	int ribbon = 0;

	while(fgets(line, sizeof line, stdin) != NULL) {
		struct box box = parse_box(line);
		paper += box_area(box);
		paper += (box.w * box.h);
		ribbon += box_volume(box);
		ribbon += box.w + box.w + box.h + box.h;
	}

	printf("part one: %d\n", paper);
	printf("part two: %d\n", ribbon);

	return 0;
}
