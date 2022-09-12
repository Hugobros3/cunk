#ifndef CUNK_PRINT_H
#define CUNK_PRINT_H

#include <stddef.h>

typedef struct Printer_ Printer;
typedef struct Growy_ Growy;

Printer* cunk_open_file_as_printer(void* FILE);
Printer* cunk_open_growy_as_printer(Growy*);

Printer* cunk_print(Printer*, const char*, ...);
void cunk_newline(Printer* p);
void cunk_indent(Printer* p);
void cunk_deindent(Printer* p);
void cunk_flush(Printer*);

/// Prints a size using appropriate KiB/MiB/GiB suffixes
void cunk_print_size_suffix(Printer*, size_t, int extra);

const char* cunk_printer_growy_unrwap(Printer* p);
#define format_string(f, ...) cunk_printer_growy_unrwap(cunk_print(cunk_open_growy_as_printer(cunk_new_growy()), __VA_ARGS__))

#endif
