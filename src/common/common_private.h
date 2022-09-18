#ifndef CUNK_COMMON_PRIVATE
#define CUNK_COMMON_PRIVATE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool read_file(const char* filename, size_t* out_size, char** out_buffer);
bool file_exists(const char* filename);
bool folder_exists(const char* filename);

typedef enum {
    ZLib_Deflate, ZLib_Zlib, ZLib_GZip
} ZLibMode;

typedef struct Growy_ Growy;
bool cunk_inflate(ZLibMode, size_t src_size, const char* input_data, Growy* output);

bool string_ends_with(const char* string, const char* suffix);
int64_t swap_endianness(int bytes, int64_t i);

#endif
