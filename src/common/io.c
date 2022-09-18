#include "cunk/print.h"
#include "common_private.h"

#include <stdlib.h>
#include <stdio.h>

// this does not work on non-POSIX compliant systems
// Cygwin/MINGW works though.
#include "sys/stat.h"

static const char* sanitize_path(const char* path) {
    size_t pathlen = strlen(path);
    bool windows_path = false;
    if (pathlen > 2) {
        if (path[1] == ':')
            windows_path = true;
    }

    if (windows_path) {
        return replace_string(path, "/", "\\");
    }

    return format_string("%s", path);
}

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
    const char* sanitized = sanitize_path(filename);
    struct stat s = { 0 };
    if (stat(filename, &s) == 0) {
        free((char*) sanitized);
        return true;
    }
    free((char*) sanitized);
    return false;
}

bool folder_exists(const char* filename) {
    struct stat s = { 0 };
    if (stat(filename, &s) == 0) {
        return S_ISDIR(s.st_mode);
    }
    return false;
}
