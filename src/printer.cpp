/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "backend.hpp"
#include "printer.hpp"
#include <sstream>
#include <iomanip>

namespace loops
{

void print_address(::std::ostream& str, int64_t addr)
{
    static char hexsymb[] = "0123456789ABCDEF";
    char* bytes = (char*)(&addr);
    str<<"0x";
    for(int i = 0; i < 8 ; i++)
        str<<hexsymb[(bytes[7-i]&0xF0)>>4]<<hexsymb[bytes[7-i]&0x0F];
}

Printer::Printer(const std::vector<ColPrinter>& columns) : m_columns(columns), m_backend(nullptr) {}

void Printer::print(std::ostream& out, const Syntfunc& toPrint, bool printheader, int firstop, int lastop) const
{
    lastop = (lastop == -1) ? (int)toPrint.program.size() : lastop;
    if(lastop > (int)toPrint.program.size())
        throw std::runtime_error("Printer error: too far last operation.");
    if (printheader)
        printHeader(out, toPrint);
    std::vector<std::string> pdetails;
    std::vector<size_t> max_widthes(m_columns.size(), 0);
    pdetails.reserve(m_columns.size() * (lastop-firstop));
    for(int rownum = firstop; rownum < lastop; rownum++)
        for (int colnum = 0; colnum < (int)m_columns.size(); colnum++)
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
    for(int rownum = firstop; rownum < lastop; rownum++)
    {
        for (int colnum = 0; colnum < (int)m_columns.size(); colnum++)
            out << std::setw(max_widthes[colnum] + 1)<< std::left << pdetails[m_columns.size()*rownum + colnum];
        out << std::endl;
    }
}

Printer::ColPrinter Printer::colNumPrinter(int /*firstRow*/)
{
    return [](::std::ostream& out, const Syntop& /*toPrint*/, int rowNum, Backend*)
    {
        out << std::setw(6) << rowNum << " :";
    };
}

Printer::ColPrinter Printer::colDelimeterPrinter()
{
    return [](::std::ostream& out, const Syntop& /*toPrint*/, int /*rowNum*/, Backend* /*backend*/)
    {
        out << ";";
    };
}

Printer::ColPrinter Printer::colOpnamePrinter(const std::unordered_map<int, std::string>& opstrings, const std::unordered_map<int, Printer::ColPrinter >& p_overrules)
{
    return [opstrings, p_overrules](::std::ostream& out, const Syntop& toPrint, int rowNum, Backend* backend)
    {
        if(p_overrules.count(toPrint.opcode) == 0)
        {
            if (opstrings.count(toPrint.opcode) == 0)
                throw std::runtime_error("Printer: unprintable operation");
            out<<opstrings.at(toPrint.opcode);
        }
        else
            p_overrules.at(toPrint.opcode)(out, toPrint, rowNum, backend);
    };
}

Printer::ColPrinter Printer::colArgListPrinter(const Syntfunc& suppfunc, const std::unordered_map<int, Printer::ColPrinter>& p_overrules)
{
    return [suppfunc, p_overrules](::std::ostream& out, const Syntop& toPrint, int rowNum, Backend* backend)
    {
        if(p_overrules.count(toPrint.opcode) == 0)
        {
            Printer::ArgPrinter argprinter = [](::std::ostream& out, const Syntop& toPrint, int /*rowNum*/, int argNum)
                {
                    out<<toPrint[argNum];
                };
            if(backend)
                argprinter = backend->argPrinter(suppfunc); //TODO(ch): We shouldn't request printer at any row. It must be called once in start.
            int aamount = toPrint.size();
            for(int anum = 0; anum + 1 < aamount ; anum++)
            {
                if(toPrint[anum].flags & AF_NOPRINT)
                    continue;
                argprinter(out, toPrint, rowNum, anum);
                out<<", ";
            }
            if(aamount)
                argprinter(out, toPrint, rowNum, aamount - 1);
        }
        else
            p_overrules.at(toPrint.opcode)(out, toPrint, rowNum, backend);
    };
}

void Printer::printHeader(std::ostream& out, const Syntfunc& toPrint) const
{
    out << toPrint.name << "(";
    for (int argnum = 0; argnum + 1 < (int)toPrint.params.size(); argnum++)
        out << toPrint.params[argnum]  << ", ";
    if (toPrint.params.size())
        out<< toPrint.params.back();
    out << ")" << std::endl;
}

}
