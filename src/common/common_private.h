#ifndef CUNK_COMMON_PRIVATE
#define CUNK_COMMON_PRIVATE

#include <stdbool.h>
#include <stddef.h>

bool read_file(const char* filename, size_t* out_size, char** out_buffer);

#endif
