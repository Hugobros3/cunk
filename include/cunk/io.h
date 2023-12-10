#ifndef CUNK_IO_H
#define CUNK_IO_H

#include <stdbool.h>
#include <stddef.h>

bool read_file(const char* filename, size_t* out_size, char** out_buffer);
bool file_exists(const char* filename);
bool folder_exists(const char* filename);

#endif
