#include <stdlib.h>
#include <stdio.h>

const char* read_file(char* filename) {
	char* buffer = NULL;
	long length;
	FILE* f = fopen(filename, "rb");

	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		
		buffer = malloc(length);
		if (buffer) {
			fread(buffer, 1, length, f);
		}
		fclose(f);
	}
	
	return buffer;
}
