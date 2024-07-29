/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_PRINTER_HPP__
#define __LOOPS_PRINTER_HPP__

#include "loops/loops.hpp"
#include "collections.hpp"
#include "common.hpp"
#include <functional>
#include <ostream>
#include <vector>
#include <unordered_map>
#include <stdlib.h>
#include <string.h>

int printer_h_initialize();
void printer_h_deinitialize();

struct syntfunc2print //TODO: In the end, this struct have to be reunited with Syntfunc
{
    loops::Syntop* program;  //DUBUG: this ridiculous (pointer, length) pairs have to be replaced with C-written implementation of vector.
    int program_size;
    loops::Arg* params;
    int params_size;
    char* name;
};

struct column_printer;
struct printer_new;
typedef int (*print_column_t)(struct printer_new* printer, struct column_printer* colprinter, syntfunc2print* func, int row);
typedef void (*free_column_printer_t)(struct column_printer* colprinter);

typedef struct column_printer
{
    print_column_t func;
    void* auxdata;
    free_column_printer_t free_func;
} column_printer;

typedef struct printer_buffer
{
    char* buffer;
    int buffer_size;
} printer_buffer;

LOOPS_LIST_DECLARE(printer_buffer);

typedef struct printer_new
{
    column_printer* colprinters;
    int colprinters_size;
    int columnflags;
    LOOPS_LIST(printer_buffer) buffers;
    char** cells;
    int* cell_sizes;
    int current_cell;
    int current_offset;
    loops::Backend* backend;
} printer_new;

int loops_printf(printer_new* printer, const char *__restrict __format, ...); //DUBUG: remove these new_ suffixes.
int new_print_address(printer_new* printer, int64_t addr);
int close_printer_cell(printer_new* printer);

int col_opname_table_printer(printer_new* printer, column_printer* colprinter, syntfunc2print* func, int row);

int create_ir_printer(int columnflags, printer_new** res);
int create_assembly_printer(int columnflags, loops::Backend* backend, printer_new** res);
void free_printer(printer_new* tofree);
int fprint_syntfunc(printer_new* printer, FILE* out, syntfunc2print* func);
/*
* Allocate with malloc enough data for out string. Allocated out have to be free by user.
*/
int sprint_syntfunc(printer_new* printer, char** out, syntfunc2print* func);
#endif//__LOOPS_PRINTER_HPP__
