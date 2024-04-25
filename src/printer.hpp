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
#include <unordered_map>
#include <stdlib.h>
#include <string.h>

void printer_h_initialize();
void printer_h_deinitialize();

typedef struct buffer_list
{
    char* buffer;
    int buffer_size;
    struct buffer_list* next;
} buffer_list;

namespace loops
{

struct syntfunc2print //TODO: In the end, this struct have to be reunited with Syntfunc
{
    Syntop* program;  //DUBUG: this ridiculous (pointer, length) pairs have to be replaced with C-written implementation of vector.
    int program_size;
    Arg* params;
    int params_size;
    char* name;
};

struct column_printer;
struct printer_new;
typedef int (*print_column_t)(struct printer_new* printer, struct column_printer* colprinter, syntfunc2print* func, int row);

typedef struct column_printer
{
    print_column_t func;
    char* buffer;
    int auxdata;
} column_printer;

typedef struct printer_new
{
    column_printer* colprinters;
    int colprinters_size;
    int columnflags;
    buffer_list* buffers_head;
    buffer_list* buffers_tail;
    char** cells;
    int* cell_sizes;
    int current_cell;
    int current_offset;
} printer_new;

int loops_printf(printer_new* printer, const char *__restrict __format, ...); //DUBUG: remove these new_ suffixes.
int new_print_address(printer_new* printer, int64_t addr);
void close_printer_cell(printer_new* printer);

int create_ir_printer(int columnflags, printer_new** res);
// int create_assembly_printer(int columnflags, printer_new** res); //DUBUG
void free_printer(printer_new* tofree);
int fprint_syntfunc(printer_new* printer, FILE* out, syntfunc2print* func);
/*
* Allocate with malloc enough data for out string. Allocated out have to be free by user.
*/
int sprint_syntfunc(printer_new* printer, char** out, syntfunc2print* func);

void print_address(::std::ostream& str, int64_t addr);

static inline ::std::ostream& operator<<(::std::ostream& str, const Arg& arg)
{
    switch (arg.tag)
    {
        case Arg::IREG: if(arg.idx == Syntfunc::RETREG) {str << "iR";} else {str << "i" << arg.idx;} break;
        case Arg::ISPILLED: {str << "s" << arg.value; } break; //TODO(ch): Can we avoid spilled registers in IR?
        case Arg::IIMMEDIATE: str << arg.value; break;
        case Arg::VREG: {str << "v" << arg.idx; } break; //TODO(ch): Can we avoid spilled registers in IR?
        default:
            throw std::runtime_error("Undefined argument type.");
    };
    return str;
}

class Backend;
class Printer
{
    public:
        typedef std::function<void(::std::ostream&, const Syntop&, int, Backend* )> ColPrinter;
        typedef std::function<void(::std::ostream&, const Syntop&, int, int)> ArgPrinter;
        Printer(const std::vector<ColPrinter>& columns);
        void setBackend(Backend* a_backend) { m_backend = a_backend; }
        void print(std::ostream& out, const Syntfunc& toPrint, bool printheader = true, int firstop = 0, int lastop = -1) const;
        static ColPrinter colDelimeterPrinter();
        static ColPrinter colNumPrinter(int firstRow = 0);
        static ColPrinter colOpnamePrinter(const std::unordered_map<int, std::string>& opstrings, const std::unordered_map<int, Printer::ColPrinter >& p_overrules = std::unordered_map<int, Printer::ColPrinter >());
        static ColPrinter colArgListPrinter(const Syntfunc& suppfunc, const std::unordered_map<int, Printer::ColPrinter >& p_overrules = std::unordered_map<int, Printer::ColPrinter >());
    private:
        void printHeader(std::ostream& out, const Syntfunc& toPrint) const;
        std::vector<ColPrinter> m_columns;
        Backend* m_backend;
};

}
#endif//__LOOPS_PRINTER_HPP__
