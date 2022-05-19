/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
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
inline ::std::ostream& operator<<(::std::ostream& str, const Arg& arg)
{
    switch (arg.tag)
    {
        case Arg::IREG: if(arg.idx == Syntfunc::RETREG) {str << "iR";} else {str << "i" << arg.idx;} break;
        case Arg::ISPILLED: {str << "s" << arg.value; } break; //TODO(ch): Can we avoid spilled registers in bytecode?
        case Arg::ICONST: str << arg.value; break;
        default:
            throw std::runtime_error("Undefined argument type.");
    };
    return str;
}

class Backend;
class Printer
{
    public:
        typedef std::function<void(::std::ostream&, const Syntop&, size_t, Backend* )> ColPrinter;
        typedef std::function<void(::std::ostream&, const Syntop&, size_t, size_t)> ArgPrinter;
        Printer(const std::vector<ColPrinter>& columns);
        void setBackend(Backend* a_backend) { m_backend = a_backend; }
        void print(std::ostream& out, const Syntfunc& toPrint, bool printheader = true, size_t firstop = 0, size_t lastop = -1) const;
        static ColPrinter colDelimeterPrinter();
        static ColPrinter colNumPrinter(size_t firstRow = 0);
        static ColPrinter colOpnamePrinter(const std::unordered_map<int, std::string>& opstrings, const std::unordered_map<int, Printer::ColPrinter >& p_overrules = std::unordered_map<int, Printer::ColPrinter >());
        static ColPrinter colArgListPrinter(const Syntfunc& suppfunc, const std::unordered_map<int, Printer::ColPrinter >& p_overrules = std::unordered_map<int, Printer::ColPrinter >());
    private:
        void printHeader(std::ostream& out, const Syntfunc& toPrint) const;
        std::vector<ColPrinter> m_columns;
        Backend* m_backend;
};

};
#endif//__LOOPS_PRINTER_HPP__
