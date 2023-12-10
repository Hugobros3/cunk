#ifndef CUNK_COMMON_PRIVATE
#define CUNK_COMMON_PRIVATE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    ZLib_Deflate, ZLib_Zlib, ZLib_GZip
} ZLibMode;

typedef struct Growy_ Growy;
bool cunk_inflate(ZLibMode, size_t src_size, const char* input_data, Growy* output);

bool string_ends_with(const char* string, const char* suffix);
int64_t swap_endianness(int bytes, int64_t i);
unsigned int needed_bits(unsigned int n);
uint64_t fetch_bits(const char* buf, size_t bit_pos, unsigned int width);
uint64_t fetch_bits_long_arr(const uint64_t* buf, bool big_endian, size_t bit_pos, unsigned int width);

#endif
