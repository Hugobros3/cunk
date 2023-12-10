#ifndef CUNK_UTIL_H
#define CUNK_UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

bool string_ends_with(const char* string, const char* suffix);
int64_t swap_endianness(int bytes, int64_t i);
unsigned int needed_bits(unsigned int n);
uint64_t fetch_bits(const char* buf, size_t bit_pos, unsigned int width);
uint64_t fetch_bits_long_arr(const uint64_t* buf, bool big_endian, size_t bit_pos, unsigned int width);

#endif
