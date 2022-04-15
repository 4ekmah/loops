/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "composer.hpp"
#include "backend.hpp"
#include <cstring>

namespace loops
{

Bitwriter::Bitwriter(const BackendImpl* a_backend): m_buffer(std::make_shared<std::vector<uint8_t> >(MINIMAL_BUFFER_SIZE, 0))
    , m_size(0)
    , m_bitpos(0)
    , m_startsize(NOTRANSACTION)
    , m_backend(a_backend)
{}

void Bitwriter::startInstruction()
{
    if (m_startsize != NOTRANSACTION)
        throw std::string("Bitwriter: instruction is already started");
    m_startsize = m_size;
}

void Bitwriter::writeDetail(uint64_t a_detail, size_t a_fieldwidth)
{
    if (a_fieldwidth > 64)
        throw std::string("Bitwriter: too big bitfield");
    if (m_startsize == NOTRANSACTION)
        throw std::string("Bitwriter: writing detail out of instruction");
    if(m_buffer->size() < m_size + (a_fieldwidth >> 3) + 1)
        m_buffer->resize(m_buffer->size() << 1 );
    uint8_t* buffer = &m_buffer->operator[](0);
    uint64_t body = m_bitpos ? ((uint64_t)(*(buffer + m_size))) << 56 : 0;
    a_detail = (a_fieldwidth < 64)? a_detail & ( (1 << a_fieldwidth) - 1): a_detail;
    const size_t bits2write = m_bitpos + a_fieldwidth;
    const size_t bytes2write = (bits2write >> 3) + (bits2write%8 ? 1 : 0);
    uint8_t tail;
    size_t tailwidth;
    if (bits2write > 64)
    {
        size_t tailwidth = bits2write - 64;
        tail = (uint8_t)((a_detail & ( (1 << tailwidth) - 1)) << (8 - tailwidth));
        a_detail = a_detail >> (tailwidth);
        body |= a_detail;
    }
    else
    {
        a_detail = a_detail << (64 - bits2write);
        body |= a_detail;
    }
    size_t bytesfromfield = bytes2write % 8;
    uint8_t* bytearray = reinterpret_cast<uint8_t*>(&body);
    for(int byten = 7; byten >= 8 - bytesfromfield; --byten)
        buffer[m_size++] = bytearray[byten];
    if(bytes2write == 9)
        buffer[m_size++] = tail;
    m_bitpos = bits2write % 8;
    if (m_bitpos)
        --m_size;
}

void Bitwriter::endInstruction()
{
    if (m_bitpos != 0)
        throw std::string("Bitwriter: instructions with width, not multiple of 8 bits are not supported.");
    if (m_startsize == NOTRANSACTION)
        throw std::string("Bitwriter: finishing instruction wasn't start.");

    if(m_backend->isMonowidthInstruction())
    {
        const size_t instrsize = m_backend->instructionWidth();
        if (m_size - m_startsize != instrsize)
            throw std::string("Bitwriter: non-standard instruction width.");
    }
    if(m_backend->isLittleEndianInstructions())
    {
        if(m_size > m_startsize + 1)
        {
            size_t lesser = m_startsize;
            size_t bigger = m_size - 1;
            for(;lesser<bigger;++lesser,--bigger)
                std::swap(m_buffer->operator[](lesser), m_buffer->operator[](bigger));
        }
    }
    m_startsize = NOTRANSACTION;
}

Binatr::Detail::Detail(int tag, size_t fieldsize, uint64_t regflag): tag(tag), width(fieldsize), fieldOflags(regflag)
{
    if(tag != D_REG && tag != D_CONST && tag != D_ADDRESS && tag != D_OFFSET && tag != D_STACKOFFSET)
        throw std::string("Binary translator: wrong detail constructor.");
    if(tag != D_REG && regflag != 0)
        throw std::string("Binary translator: registerflag is given for non-register instruction detail.");
        
}

Binatr::Detail::Detail(int tag, uint64_t val, size_t fieldsize): tag(tag), width(fieldsize), fieldOflags(val)
{
    if(tag != D_STATIC)
        throw std::string("Binary translator: wrong detail constructor.");
}

Binatr::Binatr(std::initializer_list<Detail> lst) : m_compound(lst), m_size(0)
{
    for (auto det:m_compound)
        m_size += det.width;
    m_size = m_size/8 + ((m_size%8)?1:0);
}

void Binatr::applyNAppend(const Syntop& op, Bitwriter* bits) const
{
    if (bits == nullptr)
       throw std::string("Binary translator: null writer pointer.");
    bits->startInstruction();
    size_t argnum = 0;
    for(const Detail& det : m_compound)
        switch (det.tag)
        {
            case (Detail::D_REG):
                if(op.args[argnum].tag != Arg::IREG)
                    throw std::string("Binary translator: syntop bring const instead of register.");
                ++argnum;
                break;
            case (Detail::D_CONST):
            case (Detail::D_ADDRESS):
            case (Detail::D_OFFSET):
            case (Detail::D_STACKOFFSET):
                if(op.args[argnum].tag != Arg::ICONST)
                    throw std::string("Binary translator: syntop bring register instead of const.");
                ++argnum;
                break;
            case (Detail::D_STATIC): break;
            default:
                throw std::string("Binary translator: unknown detail type.");
        };
    if(argnum != op.size())
        throw std::string("Amounts of arguments and placeholder are not equal.");
    argnum = 0;
    for(const Detail& det : m_compound)
    {
        uint64_t piece = 0;
        switch (det.tag)
        {
            case (Detail::D_STATIC):
                piece = det.fieldOflags;
                break;
            case (Detail::D_REG):
                piece = op[argnum++].idx;
                break;
            case (Detail::D_ADDRESS):
            {
//                canvas->m_addresses.push_back(bits->bitaddress()); //TODO(ch): Place adresses postions somewhere! I think, into FuncImpl.
            }
            case (Detail::D_CONST):
            case (Detail::D_OFFSET):
            case (Detail::D_STACKOFFSET):
                piece = static_cast<uint64_t>(op[argnum++].value);
                break;
            default:
                throw std::string("Binary translator: unknown detail type.");
        };
        bits->writeDetail(piece, det.width);
    }
    bits->endInstruction();
}

OpPrintInfo Binatr::getPrintInfo(const Syntop& op) const
{
    OpPrintInfo printInfo;
    size_t argNum = 0;
    for(const Detail& det : m_compound)
        if(det.tag != Detail::D_STATIC)
        {
            if(det.printNum != OpPrintInfo::PI_NOTASSIGNED)
            {

                OpPrintInfo::operand oprnd;
                oprnd.argnum = argNum;
                oprnd.flags |= (det.tag == Detail::D_OFFSET) ? OpPrintInfo::PI_OFFSET : 0;
                oprnd.flags |= ((det.fieldOflags&Detail::D_PRINTADDRESS) != 0) ? OpPrintInfo::PI_ADDRESS : 0;
                oprnd.flags |= ((((det.fieldOflags&Detail::D_32Dep) != 0) && (op.args[0].value == 0)) ? OpPrintInfo::PI_REG32 : 0); // TODO(ch):op.args[0].value == 0 is unsafe
                printInfo[det.printNum] = oprnd;
            }
            argNum++;
        }
    for(const OpPrintInfo::operand& oprnd: printInfo)
        if(oprnd.argnum == OpPrintInfo::PI_NOTASSIGNED)
            throw std::string("Binary translator: argument printing numbers are given with gap.");
    return printInfo;
}
};
