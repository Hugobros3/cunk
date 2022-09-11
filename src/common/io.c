#include "common_private.h"

#include <stdlib.h>
#include <stdio.h>

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
