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

struct column_printer;
class program_printer;
typedef std::shared_ptr<program_printer> program_printer_ptr;
//DUBUG: review all this strange int returning functions. Most of them don't need to throw anything.
class column_printer
{
public:
    typedef int (*print_t)(program_printer* printer, struct column_printer* colprinter, const loops::Syntfunc& func, int row);
    column_printer(){}
    virtual ~column_printer() {}
    column_printer(print_t a_func): func(a_func) {}
    print_t func;
};

class col_opname_table_printer : public column_printer
{
public:
    typedef loops_cstring (*table_opname_getter)(int opcode);
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
    loops::Backend* backend;
    void close_printer_cell();
    int fprint_syntfunc(FILE* out, const loops::Syntfunc& func);
    int sprint_syntfunc(std::string& out, const loops::Syntfunc& func);
private:
    program_printer(){};
    typedef struct cell
    {
        char* ptr;
        int size;
    } cell;
    std::vector<cell> cells;
    int current_offset;
    void augment_buffer(int buffer_size);
    friend void loops_printf(program_printer* printer, const char *__restrict __format, ...);
    int print_syntfunc(FILE* fout, std::string& sout, int outtype, const loops::Syntfunc& func);
    std::list<std::vector<char>> buffers;
};

void loops_printf(program_printer* printer, const char *__restrict __format, ...);
int print_address(program_printer* printer, int64_t addr); //DUBUG: Now: continue making functions void...

std::string IR_instruction2string(const loops::Syntop& op);
std::string assembly_instruction2string(const loops::Syntop& op, const loops::Backend& backend);
#endif//__LOOPS_PRINTER_HPP__
