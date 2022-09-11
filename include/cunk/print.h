#ifndef CUNK_PRINT_H
#define CUNK_PRINT_H

typedef struct Printer_ Printer;

Printer* cunk_open_file_as_printer(void* FILE);

void cunk_print(Printer*, const char*, ...);
void cunk_newline(Printer* p);
void cunk_indent(Printer* p);
void cunk_deindent(Printer* p);
void cunk_flush(Printer*);

#endif
