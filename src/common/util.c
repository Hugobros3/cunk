#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

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

unsigned int needed_bits(unsigned int n) {
    if (n == 0)
        return 0;
    int bits = 1;
    int maxn = 2;
    while (n > maxn) {
        bits++;
        maxn *= 2;
    }
    return bits;
}

uint64_t fetch_bits(const char* buf, size_t bit_pos, unsigned int width) {
    int64_t acc = 0;
    size_t pos = SIZE_MAX;
    char last_fetch;
    for (size_t bit = 0; bit < width; bit++) {
        size_t new_pos = (bit_pos + bit) / CHAR_BIT;
        if (new_pos != pos) {
            pos = new_pos;
            last_fetch = buf[pos];
        }
        int64_t b = (last_fetch >> ((bit_pos + bit) & 0x7)) & 0x1;
        acc |= (b << bit);
    }
    return acc;
}

uint64_t fetch_bits_long_arr(const uint64_t* buf, bool big_endian, size_t bit_pos, unsigned int width) {
#define LONG_BITS (CHAR_BIT * sizeof(int64_t))
    int64_t acc = 0;

    size_t pos = SIZE_MAX;
    uint64_t last_fetch;
    for (size_t bit = 0; bit < width; bit++) {
        size_t new_pos = (bit_pos + bit) / LONG_BITS;
        if (new_pos != pos) {
            pos = new_pos;
            last_fetch = buf[pos];
            if (big_endian)
                last_fetch = swap_endianness(8, last_fetch);
        }
        uint64_t b = (last_fetch >> ((bit_pos + bit) & (LONG_BITS - 1))) & 0x1;
        acc |= (b << bit);
    }
    return acc;
#undef LONG_BITS
}
