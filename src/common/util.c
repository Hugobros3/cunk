#include <assert.h>

// MSVC needs not apply.
static_assert(__STDC__ == 1, "A conformant C compiler is required.");
static_assert(__STDC_VERSION__ >= 201112L, "A C11 compiler is required.");
