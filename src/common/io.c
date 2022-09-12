#include "common_private.h"

#include <stdlib.h>
#include <stdio.h>

// this does not work on non-POSIX compliant systems
// Cygwin/MINGW works though.
#include "sys/stat.h"

bool read_file(const char* filename, size_t* out_size, char** out_buffer) {
	char* buffer = NULL;
	long length;
	FILE* f = fopen(filename, "rb");

	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		
		buffer = *out_buffer = malloc(length);
		if (buffer) {
			fread(buffer, 1, length, f);
            *out_size = (size_t) length;
            fclose(f);
            return true;
		}
	}
    return false;
}

bool file_exists(const char* filename) {
    struct stat s = { 0 };
    if (stat(filename, &s) == 0) {
        return true;
    }
    return false;
}

bool folder_exists(const char* filename) {
    struct stat s = { 0 };
    if (stat(filename, &s) == 0) {
        return S_ISDIR(s.st_mode);
    }
    return false;
}
