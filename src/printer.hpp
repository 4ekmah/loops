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

namespace loops
{

struct syntfunc2print //TODO: In the end, this struct have to be reunited with Syntfunc
{
    Syntop* program;  //TODO: this ridiculous (pointer, length) pairs have to be replaced with C-written implementation of vector.
    int program_size;
    Arg* params;
    int params_size;
    char* name;
};

struct column_printer; 

typedef int (*print_column_t)(FILE* out, syntfunc2print* func, int row, column_printer* colprint);

struct column_printer //DUBUG: check, that everything is needed
{
    print_column_t func;
    char* buffer;
    int currentoffset;
};

struct printer_new
{
    column_printer* colprinters;
    int colprinters_size;
    char** cells;
};

int create_ir_printer(int columnflags, printer_new** res);
int create_assembly_printer(int columnflags, printer_new** res);
int free_printer(printer_new* tofree);
int print_syntfunc(FILE* out, syntfunc2print* func, printer_new* printer); //DUBUG: actually, it have to be member of printer_new, does C89 support member functions?

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
