#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a, b) a < b ? a : b
#define max(a, b) a > b ? a : b
#define box_volume(b) b.w * b.h * b.l
#define box_area(b) 2 * (b.w * b.h + b.w * b.l + b.h * b.l)

struct box { int w, h, l; };

/* parse the dimensions of a box from `line` and fill them into `box` */
bool parse_box(struct box *box, char *line) {
	int a, b, c;
	if (sscanf(line, "%dx%dx%d\n", &a, &b, &c) != 3) {
		printf("error parsing box: %s\n", line);
		return false;
	}
	// ensure w*h is always the smallest face
	if (a < b) {
		box->w = a;
		box->h = min(b, c);
		box->l = max(b, c);
	} else 	{
		box->w = b;
		box->h = min(a, c);
		box->l = max(a, c);
	}
	return true;
}

int main(void) {
	char line[1024];
	struct box box;

	int paper = 0;
	int ribbon = 0;

	while(fgets(line, sizeof line, stdin) != NULL) {
		if (!parse_box(&box, line)) return 1;
		paper += box_area(box);
		paper += (box.w * box.h);
		ribbon += box_volume(box);
		ribbon += box.w + box.w + box.h + box.h;
	}

	printf("part one: %d\n", paper);
	printf("part two: %d\n", ribbon);

	return 0;
}
