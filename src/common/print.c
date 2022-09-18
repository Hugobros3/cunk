#include "cunk/print.h"
#include "cunk/memory.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#pragma GCC diagnostic error "-Wswitch"

typedef enum {
    PoFile, PoGrowy
} PrinterOutput;

struct Printer_ {
    PrinterOutput output;
    union {
        FILE* file;
        Growy* growy;
    };
    int indent;
};

Printer* cunk_open_file_as_printer(void* f) {
    Printer* p = calloc(1, sizeof(Printer));
    p->output = PoFile;
    p->file = (FILE*) f;
    return p;
}

Printer* cunk_open_growy_as_printer(Growy* g) {
    Printer* p = calloc(1, sizeof(Printer));
    p->output = PoGrowy;
    p->growy = g;
    return p;
}

static void cunk_print_bare(Printer* p, size_t len, const char* str) {
    assert(strlen(str) >= len);
    switch(p->output) {
        case PoFile: fwrite(str, sizeof(char), len, p->file); break;
        case PoGrowy: cunk_growy_append_bytes(p->growy, len, str);
    }
}

void cunk_flush(Printer* p) {
    switch(p->output) {
        case PoFile: fflush(p->file); break;
        case PoGrowy: break;
    }
}

void cunk_indent(Printer* p) {
    p->indent++;
}

void cunk_deindent(Printer* p) {
    p->indent--;
}

void cunk_newline(Printer* p) {
    cunk_print_bare(p, 1, "\n");
    for (int i = 0; i < p->indent; i++)
        cunk_print_bare(p, 4, "    ");
}

#define LOCAL_BUFFER_SIZE 32

Printer* cunk_print(Printer* p, const char* f, ...) {
    size_t len = strlen(f) + 1;
    if (len == 1)
        return p;

    // allocate a bit more to have space for formatting
    size_t bufsize = (len + 1) + len / 2;

    char buf[LOCAL_BUFFER_SIZE];
    char* alloc = NULL;

    // points to either the contents of buf, or alloc, depending on bufsize
    char* tmp;
    int written;

    while(true) {
        if (bufsize <= LOCAL_BUFFER_SIZE) {
            bufsize = LOCAL_BUFFER_SIZE;
            tmp = buf;
        } else {
            if (!alloc)
                tmp = alloc = malloc(bufsize);
            else
                tmp = alloc = realloc(alloc, bufsize);
        }

        tmp[bufsize - 1] = '?';

        va_list l;
        va_start(l, f);
        written = vsnprintf(tmp, bufsize, f, l);
        va_end(l);

        if (written <= bufsize && tmp[written] == '\0')
            break;

        // increase buffer size and try again
        bufsize *= 2;
    }

    size_t start = 0;
    size_t i = 0;
    while(i < written) {
        if (tmp[i] == '\n') {
            cunk_print_bare(p, i - start, &tmp[start]);
            cunk_newline(p);
            start = i + 1;
        }
        i++;
    }

    if (start < i)
        cunk_print_bare(p, i - start, &tmp[start]);

    free(alloc);
    return p;
}

void cunk_print_size_suffix(Printer* p, size_t s, int extra) {
    const char* suffixes[] = { "B", "KiB", "MiB", "GiB", "PiB" };
    int max_suffix = sizeof(suffixes) / sizeof(const char*);

    int i = 0;
    while (s >= (1024 << (extra))) {
        s /= 1024;
        i++;
        if (i + 1 == max_suffix)
            break;
    }

    cunk_print(p, "%llu%s", s, suffixes[i]);
}

const char* cunk_printer_growy_unrwap(Printer* p) {
    assert(p->output == PoGrowy);
    return cunk_growy_deconstruct(p->growy);
}

const char* replace_string(const char* source, const char* match, const char* replace_with) {
    Growy* g = cunk_new_growy();
    size_t match_len = strlen(match);
    size_t replace_len = strlen(replace_with);
    const char* next_match = strstr(source, match);
    while (next_match != NULL) {
        size_t diff = next_match - source;
        cunk_growy_append_bytes(g, diff, (char*) source);
        cunk_growy_append_bytes(g, replace_len, (char*) replace_with);
        source = next_match + match_len;
        next_match = strstr(source, match);
    }
    cunk_growy_append_bytes(g, strlen(source), (char*) source);
    char zero = '\0';
    cunk_growy_append_bytes(g, 1, &zero);
    return cunk_growy_deconstruct(g);
}
