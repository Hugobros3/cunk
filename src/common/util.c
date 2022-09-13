#include "common_private.h"

#include <assert.h>
#include <string.h>

// MSVC needs not apply.
static_assert(__STDC__ == 1, "A conformant C compiler is required.");
static_assert(__STDC_VERSION__ >= 201112L, "A C11 compiler is required.");

bool string_ends_with(const char* string, const char* suffix) {
    size_t len = strlen(string);
    size_t slen = strlen(suffix);
    if (len < slen)
        return false;
    for (size_t i = 0; i < slen; i++) {
        if (string[len - 1 - i] != suffix[slen - 1 - i])
            return false;
    }
    return true;
}

int64_t swap_endianness(int bytes, int64_t i) {
    int64_t acc = 0;
    for (int byte = 0; byte < bytes; byte++)
        acc |= ((i >> byte * 8) & 0xFF) << (bytes - 1 - byte) * 8;
    return acc;
}
