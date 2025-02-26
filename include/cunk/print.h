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

const char* cunk_printer_growy_unwrap(Printer* p);
Growy* cunk_new_growy();
#define format_string(f, ...) cunk_printer_growy_unwrap(cunk_print(cunk_open_growy_as_printer(cunk_new_growy()), (f), __VA_ARGS__))

const char* replace_string(const char* source, const char* match, const char* replace_with);

#endif
