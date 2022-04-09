/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "backend.hpp"
#include "printer.hpp"
#include <sstream>
#include <iomanip>

namespace loops
{

Printer::Printer(const std::vector<ColPrinter>& columns) : m_columns(columns), m_backend(nullptr) {}

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
            colprinter(str, toPrint.program[rownum], rownum, m_backend);
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

Printer::ColPrinter Printer::colNumPrinter(size_t firstRow)
{
    return [](::std::ostream& out, const Syntop& toPrint, size_t rowNum, BackendImpl*)
    {
        out << std::setw(6) << rowNum << " :";
    };
}

Printer::ColPrinter Printer::colDelimeterPrinter()
{
    return [](::std::ostream& out, const Syntop& toPrint, size_t rowNum, BackendImpl*)
    {
        out << ";";
    };
}
Printer::ColPrinter Printer::colOpnamePrinter(const std::unordered_map<int, std::string>& opstrings, const std::unordered_map<int, Printer::ColPrinter >& p_overrules)
{
    return [opstrings, p_overrules](::std::ostream& out, const Syntop& toPrint, size_t rowNum, BackendImpl* backend)
    {
        if(p_overrules.count(toPrint.opcode) == 0)
        {
            if (opstrings.count(toPrint.opcode) == 0)
                throw std::string("Printer: unprintable operation");
            out<<opstrings.at(toPrint.opcode);
        }
        else
            p_overrules.at(toPrint.opcode)(out, toPrint, rowNum, backend);
    };
}

Printer::ColPrinter Printer::colArgListPrinter(const std::unordered_map<int, Printer::ColPrinter>& p_overrules)
{
    return [p_overrules](::std::ostream& out, const Syntop& toPrint, size_t rowNum, BackendImpl* backend)
    {
        if(p_overrules.count(toPrint.opcode) == 0)
        {
            Printer::ArgPrinter argprinter = [](::std::ostream& out, const Syntop& toPrint, size_t rowNum, size_t argNum, const OpPrintInfo& pinfo)
                {
                    out<<toPrint[argNum];
                };
            if(backend)
                argprinter = backend->argPrinter();
            OpPrintInfo pinfo;
            if(backend)
                pinfo = backend->getPrintInfo(const_cast<Syntop&>(toPrint));
            size_t aamount = pinfo.size() ? pinfo.size() : toPrint.size();
            size_t anum = 0;
            for(size_t anum = 0; anum + 1 < aamount ; anum++)
            {
                argprinter(out, toPrint, rowNum, anum, pinfo);
                out<<", ";
            }
            if(aamount)
                argprinter(out, toPrint, rowNum, aamount - 1, pinfo);
        }
        else
            p_overrules.at(toPrint.opcode)(out, toPrint, rowNum, backend);
    };
}

void Printer::printHeader(std::ostream& out, const Syntfunc& toPrint) const
{
    out << toPrint.name << "(";
    for (size_t argnum = 0; argnum + 1 < toPrint.params.size(); argnum++)
        out << argIReg(toPrint.params[argnum])  << ", ";
    if (toPrint.params.size())
        out<<argIReg(toPrint.params.back());
    out << ")" << std::endl;
}

};
