#ifndef CORE_FN
#define CORE_FN

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t (*HashFn)(void *);
typedef bool (*CmpFn)(void *, void *);
typedef void (*CallbackFn)(void *);

#endif
