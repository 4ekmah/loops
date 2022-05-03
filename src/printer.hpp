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

struct OpPrintInfo //TODO(ch): (Also see Syntop) Looks like we need static_vector<size>.
{
    enum {PI_MAXARGS = 10, PI_NOTASSIGNED = size_t(-1)};
    enum {PI_OFFSET = 1, PI_REGADD = 2, PI_REG32 = 4, PI_ADDRESS = 8};
    struct operand
    {
        size_t argnum;
        uint64_t flags;
        operand(): argnum(PI_NOTASSIGNED), flags(0) {}
    };
    operand operands[PI_MAXARGS];
    size_t _size;
    size_t size() const {return _size;}
    OpPrintInfo(): _size(0) {}
    inline operand& operator[](size_t nnum)
    {
        if(nnum >= OpPrintInfo::PI_MAXARGS)
            throw std::string("Printer: too big argument printing number.");
        _size = std::max(_size, nnum + 1);
        return operands[nnum];
    }
    inline const operand& operator[](size_t nnum) const
    {
        if(nnum >= _size)
            throw std::string("Printer: too big argument printing number.");
        return operands[nnum];
    }
    inline const operand* begin() const { return operands; }
    inline const operand* end() const
    {
        if(_size > PI_MAXARGS)
            throw std::string("Printer: too much args!");
        return operands + _size;
    }
    inline operand* begin() { return operands; }
    inline operand* end()
    {
        if(_size > PI_MAXARGS)
            throw std::string("Printer: too much args!");
        return operands + _size;
    }
    inline operand& back()
    {
        if(_size == 0)
            throw std::string("Printer: don't have such arg!");
        return operands[size() - 1];
    }
    inline const operand& back() const
    {
        if(_size == 0)
            throw std::string("Printer: don't have such arg!");
        return operands[size() - 1];
    }
};

inline ::std::ostream& operator<<(::std::ostream& str, const Arg& arg)
{
    switch (arg.tag)
    {
        case Arg::IREG: if(arg.idx == Syntfunc::RETREG) {str << "iR";} else {str << "i" << arg.idx;} break;
        case Arg::ISPILLED: {str << "s" << arg.value; } break; //TODO(ch): Can we avoid spilled registers in bytecode?
        case Arg::ICONST: str << arg.value; break;
        default:
            throw std::string("Undefined argument type.");
    };
    return str;
}

class Backend;
class Printer
{
    public:
        typedef std::function<void(::std::ostream&, const Syntop&, size_t, Backend* )> ColPrinter;
        typedef std::function<void(::std::ostream&, const Syntop&, size_t, size_t, const OpPrintInfo&)> ArgPrinter;
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
