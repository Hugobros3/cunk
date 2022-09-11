#include "cunk/print.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

struct Printer_ {
    FILE* f;
    int indent;
};

Printer* cunk_open_file_as_printer(void* f) {
    Printer* p = calloc(1, sizeof(Printer));
    p->f = (FILE*) f;
    return p;
}

void cunk_newline(Printer* p) {
    fprintf(p->f, "\n");
    for (int i = 0; i < p->indent; i++)
        fprintf(p->f, "    ");
}

void cunk_indent(Printer* p) {
    p->indent++;
}

void cunk_deindent(Printer* p) {
    p->indent--;
}

void cunk_print(Printer* p, const char* f, ...) {
    size_t len = strlen(f) + 1;
    if (len == 1)
        return;

    char* buf[32];
    char* tmp = len < 32 ? buf : malloc(len);
    memcpy(tmp, f, len);

    if (tmp[0] == '\n')
        cunk_newline(p);

    va_list l;
    va_start(l, f);
    char* t = strtok(tmp, "\n");
    while (t) {
        vfprintf(p->f, t, l);
        t = strtok(NULL, "\n");
        if (t)
            cunk_newline(p);
    }

    if (len > 2 && f[len - 2] == '\n')
        cunk_newline(p);

    va_end(l);

    if (len >= 32)
        free(tmp);
}

void cunk_flush(Printer* p) {
    fflush(p->f);
}
