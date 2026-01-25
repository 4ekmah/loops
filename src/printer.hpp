/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_PRINTER_HPP__
#define __LOOPS_PRINTER_HPP__

#include "loops/loops.hpp"
#include "common.hpp"
#include <functional>
#include <ostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <stdlib.h>
#include <string.h>

typedef loops::Syntop loops_Syntop; //TODO[CPP2ANSIC]: Delete, Syntop will be out of loops namespace.
typedef loops::Arg loops_Arg;       //TODO[CPP2ANSIC]: Delete, Arg will be out of loops namespace.

struct column_printer;
struct program_printer;
typedef int (*print_column_t)(struct program_printer* printer, struct column_printer* colprinter, const loops::Syntfunc& func, int row);
typedef void (*free_column_printer_t)(struct column_printer* colprinter);
typedef int (*table_opname_getter)(int opcode, loops_cstring* found_name);

class column_printer
{
public:
    column_printer(){}
    column_printer(print_column_t a_func, void* a_auxdata = nullptr): func(a_func), auxdata(a_auxdata), free_func(nullptr) {}
    virtual ~column_printer() {}; 
    print_column_t func;
    void* auxdata;                   //DUBUG:delete!
    free_column_printer_t free_func; //DUBUG:delete!
};
typedef std::shared_ptr<column_printer> column_printer_ptr;

typedef struct program_printer
{
    std::vector<column_printer_ptr> colprinters;
    std::list<std::vector<char>> buffers;
    int columnflags;
    char** cells;     //TODO[CPP2ANSIC]: This 3 fields have to be loops_vector(struct{char* cell_data, int cell_size}), when this container will be created.
    int* cell_sizes;  //
    int current_cell; //
    int current_offset;
    loops::Backend* backend;
} program_printer;

int loops_printf(program_printer* printer, const char *__restrict __format, ...);
int new_print_address(program_printer* printer, int64_t addr);
int close_printer_cell(program_printer* printer);

int col_opname_table_printer(program_printer* printer, column_printer* colprinter, const loops::Syntfunc& func, int row);

int create_ir_printer(int columnflags, program_printer** res);
int create_assembly_printer(int columnflags, loops::Backend* backend, program_printer** res);
void free_printer(program_printer* tofree);
int fprint_syntfunc(program_printer* printer, FILE* out, const loops::Syntfunc& func);
/*
* Allocate with malloc enough data for out string. Allocated out have to be free by user.
*/
int sprint_syntfunc(program_printer* printer, char** out, const loops::Syntfunc& func);

std::string IR_instruction2string(const loops::Syntop& op);
std::string assembly_instruction2string(const loops::Syntop& op, const loops::Backend& backend);
#endif//__LOOPS_PRINTER_HPP__
