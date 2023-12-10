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

#endif
