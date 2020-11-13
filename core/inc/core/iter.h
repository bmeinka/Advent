#ifndef CORE_ITER
#define CORE_ITER

#include "fn.h"
#include "std.h"

/* iterate over all perkutations of a sequence
 *
 * `arr` is the sewuence which will be modified in place.
 * `elem_size` is the size of items in the sequence.
 * `n` is the number of elements in the sequence.
 * `cb` is the callback function that will be called with each permutation.
 */
void permute(void *arr, size_t elem_size, size_t n, CallbackFn cb);

#endif
