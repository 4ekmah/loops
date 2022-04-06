/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "printer.hpp"
#include <sstream>
#include <iomanip>

namespace loops
{

Printer::Printer(const std::vector<ColPrinter>& columns) : m_columns(columns) {}

void Printer::print(std::ostream& out, const Syntfunc& toPrint, bool printheader, size_t firstop, size_t lastop) const
{
    lastop = (lastop == -1) ? toPrint.program.size() : lastop;
    if(lastop > toPrint.program.size())
        throw std::string("Printer error: too far last operation.");
    if (printheader)
        printHeader(out, toPrint);
    std::vector<std::string> pdetails;
    std::vector<size_t> max_widthes(m_columns.size(), 0);
    pdetails.reserve(m_columns.size() * (lastop-firstop));
    for(size_t rownum = firstop; rownum < lastop; rownum++)
        for (size_t colnum = 0; colnum < m_columns.size(); colnum++)
        {
            ColPrinter& colprinter = const_cast<ColPrinter&>(m_columns[colnum]);
            std::string col;
            ::std::ostringstream str(col, ::std::ios::out);
            colprinter(str, toPrint.program[rownum], rownum);
            str.flush();
            col = str.str();
            max_widthes[colnum] = std::max(max_widthes[colnum], col.size());
            pdetails.push_back(col);
        }
    for(size_t rownum = firstop; rownum < lastop; rownum++)
    {
        for (size_t colnum = 0; colnum < m_columns.size(); colnum++)
            out << std::setw(max_widthes[colnum] + 1)<< std::left << pdetails[m_columns.size()*rownum + colnum];
        out << std::endl;
    }
}

Printer::ColPrinter Printer::rowNumPrinter(size_t firstRow)
{
    return [](::std::ostream& out, const Syntop& toPrint, size_t rowNum)
    {
        out << std::setw(6) << rowNum << " :";
    };
}

Printer::ColPrinter Printer::rowSynPrinter(const std::unordered_map<int, std::string>& opstrings, const std::unordered_map<int, Printer::ColPrinter>& p_overrules)
{
    return [opstrings, p_overrules](::std::ostream& out, const Syntop& toPrint, size_t rowNum)
    {
        if(p_overrules.count(toPrint.opcode) == 0)
        {
            if (opstrings.count(toPrint.opcode) == 0)
                throw std::string("Printer: unprintable operation");
            out<<opstrings.at(toPrint.opcode)<<" ";
            for (size_t argnum = 0 ; argnum + 1 < toPrint.size(); argnum++)
                out << toPrint.args[argnum]<<", ";
            if(toPrint.size())
                out << toPrint.back();
        }
        else
            p_overrules.at(toPrint.opcode)(out, toPrint, rowNum);
    };
}

void Printer::printHeader(std::ostream& out, const Syntfunc& toPrint) const
{
    out << toPrint.name << "(";
    for (size_t argnum = 0; argnum + 1 < toPrint.params.size(); argnum++)
        out << "i" << toPrint.params[argnum] << ", ";
    if (toPrint.params.size())
        out<<"i"<<toPrint.params.back();
    out << ")" << std::endl;
}

};
