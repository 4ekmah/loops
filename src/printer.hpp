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
class program_printer;
typedef std::shared_ptr<program_printer> program_printer_ptr;
typedef int (*print_column_t)(program_printer* printer, struct column_printer* colprinter, const loops::Syntfunc& func, int row);
typedef void (*free_column_printer_t)(struct column_printer* colprinter);
typedef int (*table_opname_getter)(int opcode, loops_cstring* found_name);

class column_printer
{
public:
    column_printer(){}
    virtual ~column_printer() {}
    column_printer(print_column_t a_func): func(a_func) {}
    print_column_t func;
};

class col_opname_table_printer : public column_printer
{
public:
    col_opname_table_printer(table_opname_getter a_name_getter): column_printer(&col_opname_table_printer::print), name_getter(a_name_getter) {}
    virtual ~col_opname_table_printer() override {}
private:
    table_opname_getter name_getter;
    static int print(struct program_printer* printer, struct column_printer* colprinter, const loops::Syntfunc& func, int row);
};
typedef std::shared_ptr<column_printer> column_printer_ptr;

class program_printer
{
public:
    static program_printer_ptr create_ir_printer(int columnflags);
    static program_printer_ptr create_assembly_printer(int columnflags, loops::Backend* backend);
    std::vector<column_printer_ptr> colprinters;
    int columnflags;
    typedef struct cell  
    {
        char* ptr;
        int size;
    } cell;
    std::vector<cell> cells;
    int current_offset;
    loops::Backend* backend;
    int close_printer_cell();
    void augment_buffer(int buffer_size = 0);
private:
    friend int loops_printf(program_printer* printer, const char *__restrict __format, ...);
    std::list<std::vector<char>> buffers;
};

int loops_printf(program_printer* printer, const char *__restrict __format, ...);
int print_address(program_printer* printer, int64_t addr);

int fprint_syntfunc(program_printer_ptr printer, FILE* out, const loops::Syntfunc& func);
/*
* Allocate with new enough data for out string. Allocated out have to be deleted by user.
*/
int sprint_syntfunc(program_printer_ptr printer, std::string& out, const loops::Syntfunc& func);

std::string IR_instruction2string(const loops::Syntop& op);
std::string assembly_instruction2string(const loops::Syntop& op, const loops::Backend& backend);
#endif//__LOOPS_PRINTER_HPP__
