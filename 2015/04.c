#include <core/std.h>
#include <stdint.h>

// functions for the four rounds of md5
#define F(b, c, d) (((b) & (c)) | ((~(b)) & (d)))
#define G(b, c, d) (((d) & (b)) | ((~(d)) & (c)))
#define H(b, c, d) ((b) ^ (c) ^ (d))
#define I(b, c, d) ((c) ^ ((b) | (~(d))))
#define rotate(n, c) (((n) << c) | ((n) >> (32 - c)))

// md5 shift values
static uint32_t s[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

// pre-computed md5 constants
static uint32_t K[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

void md5_hash(char msg[64], char digest[16]) {
	size_t i, g;
	uint32_t *a, *b, *c, *d, f, *m, state[4];

	m = (uint32_t *)msg;

	state[3] = 0x67452301; // a
	state[2] = 0xefcdab89; // b
	state[1] = 0x98badcfe; // c
	state[0] = 0x10325476; // d

	for (i = 0; i < 64; i++) {
		a = &state[(i + 3) % 4];
		b = &state[(i + 2) % 4];
		c = &state[(i + 1) % 4];
		d = &state[(i + 0) % 4];

		if (i < 16) {
			g = i;
			f = F(*b, *c, *d);
		}
		else if (i < 32) {
			g = (5 * i + 1) % 16;
			f = G(*b, *c, *d);
		}
		else if (i < 48) {
			g = (3 * i + 5) % 16;
			f = H(*b, *c, *d);
		}
		else {
			g = (7 * i) % 16;
			f = I(*b, *c, *d);
		}
		f += *a + K[i] + m[g];
		*a = *b + rotate(f, s[i]);
	}
	state[3] += 0x67452301; 
	state[2] += 0xefcdab89;
	state[1] += 0x98badcfe;
	state[0] += 0x10325476;

	((uint32_t *)digest)[0] = state[3];
	((uint32_t *)digest)[1] = state[2];
	((uint32_t *)digest)[2] = state[1];
	((uint32_t *)digest)[3] = state[0];
}

struct message {
	char buffer[64];
	char *n_start;
	char *n_end;
	uint64_t *length;
};

static void get_message(struct message *message) {
	// clear the buffer and read the secret from stdin
	memset(message->buffer, 0, 64);
	fread(message->buffer, sizeof(char), 8, stdin);

	// make sure the pointers are pointing to the right places
	message->length = &((uint64_t *)message->buffer)[7];
	message->n_start = &message->buffer[strlen(message->buffer)];
	message->n_end = message->n_start;

	// add the pad bit and set the length
	message->n_start[0] = '0';
	*message->length = strlen(message->buffer) * 8;
	message->n_end[1] = 128;
}

static void increment_message(struct message *msg) {
	char *tail = msg->n_end;
	(*tail)++;
	while (tail > msg->n_start) {
		if (*tail > '9') {
			*(tail--) = '0';
			(*tail)++;
		}
		else break;
	}
	if (*msg->n_start > '9') {
		msg->n_end++;
		*msg->n_start = '1';
		msg->n_end[0] = '0';
		msg->n_end[1] = 128;
		*msg->length += 8;
	}
}

static char *get_answer(struct message *msg) {
	static char answer[32];
	memset(answer, 0, 32);
	strncpy(answer, msg->n_start, 32);
	// drop the padding
	answer[strlen(answer) - 1] = '\0';
	return answer;
}

static bool check_hash(int diff, char *hash) {
	int bits = diff * 4;
	size_t offset = 0;
	while (bits > 8) {
		if (hash[offset] > 0)
			return false;
		offset++; bits -= 8;
	}
	return (hash[offset] >> (8 -bits)) == 0;
}

int main(void) {
	char digest[16] = {0xff};
	struct message msg;
	get_message(&msg);

	int diff = 5;
	while (!check_hash(diff, digest)) {
		increment_message(&msg);
		md5_hash(msg.buffer, digest);
	}
	char *answer = get_answer(&msg);
	printf("part one: %s\n", answer);
	diff++;
	while (!check_hash(diff, digest)) {
		increment_message(&msg);
		md5_hash(msg.buffer, digest);
	}
	answer = get_answer(&msg);
	printf("part two: %s\n", answer);
	return 0;
}
