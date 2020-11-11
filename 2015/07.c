#include <core/std.h>
#include <core/map.h>
#include <ctype.h>

static uint16_t get_signal(Map m, char *key);

struct wire {
	bool has_signal;
	uint16_t signal;
	char text[32];
};

struct gate {
	enum { AND, OR, NOT, LSHIFT, RSHIFT } op;
	char lhs[8];
	char rhs[8];
};

static uint64_t hash(void *p) {
	// strcpy to a zero buffer to eliminate noise after the null terminator
	char buffer[8] = {'\0'};
	strcpy(buffer, p);
	return *(uint64_t *)buffer;
}

static bool cmp(void *a, void *b) {
	return (strcmp(a, b) == 0);
}

static void parse_line(Map m, const char *s) {
	static char sep[] = " -> ";
	char name[8];
	struct wire wire = {
		.has_signal = false,
		.text = {'\0'}
	};
	char *end = strstr(s, sep);
	strncpy(wire.text, s, end - s);
	strncpy(name, end + 4, 8);
	map_set(m, name, &wire);
}

static struct gate parse_gate(const char *s) {
	struct gate gate;
	if (strstr(s, "NOT")) {
		gate.op = NOT;
		// rhs is ignored, but still parsed
		strcpy(gate.rhs, "0");
		sscanf(s, "NOT %s", gate.lhs);
		return gate;
	}
	char op[8];
	sscanf(s, "%s %s %s", gate.lhs, op, gate.rhs);
	if (strcmp(op, "AND") == 0)
		gate.op = AND;
	else if (strcmp(op, "OR") == 0)
		gate.op = OR;
	else if (strcmp(op, "LSHIFT") == 0)
		gate.op = LSHIFT;
	else if (strcmp(op, "RSHIFT") == 0)
		gate.op = RSHIFT;
	return gate;
}

static bool is_gate(const char *s) {
	return strstr(s, "AND") ||
	       strstr(s, "OR") ||
	       strstr(s, "NOT") ||
	       strstr(s, "LSHIFT") ||
	       strstr(s, "RSHIFT");
}

static uint16_t eval_gate(Map m, char *text) {
	struct gate gate = parse_gate(text);
	uint16_t left, right;
	if (isalpha(gate.lhs[0]))
		left = get_signal(m, gate.lhs);
	else left = atoi(gate.lhs);
	if (isalpha(gate.rhs[0]))
		right = get_signal(m, gate.rhs);
	else right = atoi(gate.rhs);
	switch (gate.op) {
		case RSHIFT: return left >> right;
		case LSHIFT: return left << right;
		case NOT: return ~left;
		case OR: return left | right;
		case AND:
		default:
			return left & right;
	}
}

static uint16_t get_signal(Map m, char *key) {
	struct wire wire;
	map_get(m, key, &wire);
	if (!wire.has_signal) {
		if (is_gate(wire.text))
			wire.signal = eval_gate(m, wire.text);
		else if (isdigit(wire.text[0]))
			wire.signal = atoi(wire.text);
		else if (isalpha(wire.text[0]))
			wire.signal = get_signal(m, wire.text);
		else {
			printf("unhandled wire: %s\n", wire.text);
			return 0;
		}
		wire.has_signal = true;
		map_set(m, key, &wire);
	}
	return wire.signal;
}

int main(void) {
	char line[32];
	Map mp1 = map_create(sizeof(char) * 8, sizeof(struct wire), &hash, &cmp);
	Map mp2 = map_create(sizeof(char) * 8, sizeof(struct wire), &hash, &cmp);

	while (fgets(line, sizeof(line), stdin)) {
		line[strcspn(line, "\r\n")] = '\0';
		parse_line(mp1, line);
		parse_line(mp2, line);
	}

	char key[8] = "a";
	uint16_t p1 = get_signal(mp1, key);
	printf("part one: %" PRIu16 "\n", p1);

	struct wire b = {
		.has_signal = true,
		.signal = p1
	};
	strcpy(key, "b");
	map_set(mp2, key, &b);

	strcpy(key, "a");
	uint16_t p2 = get_signal(mp2, key);
	printf("part two: %" PRIu16 "\n", p2);

	map_destroy(mp1);
	map_destroy(mp2);
	return 0;
}
