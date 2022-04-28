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

Bitwriter::Bitwriter(const Backend* a_backend): m_buffer(std::make_shared<std::vector<uint8_t> >(MINIMAL_BUFFER_SIZE, 0))
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
    a_detail = (a_fieldwidth < 64)? a_detail & ( ((uint64_t)(1) << a_fieldwidth) - 1): a_detail;
    const size_t bits2write = m_bitpos + a_fieldwidth;
    const size_t bytes2write = (bits2write >> 3) + (bits2write%8 ? 1 : 0);
    uint8_t tail = 0;
    if (bits2write > 64)
    {
        size_t tailwidth = bits2write - 64;
        tail = (uint8_t)((a_detail & ( ((uint64_t)(1) << tailwidth) - 1)) << (8 - tailwidth));
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

uint64_t Bitwriter::revertDetail(uint64_t a_det, size_t dwidth)
{
    dwidth = (dwidth + 7) >> 3;
    size_t endpos = dwidth - 1;
    dwidth = dwidth >> 1;
    uint8_t* bytearr = reinterpret_cast<uint8_t*>(&a_det);
    for (size_t p1 = 0; p1 < dwidth; p1++)
        std::swap(bytearr[p1], bytearr[endpos - p1]);
    return a_det;
}

Binatr::Detail::Detail(int tag, size_t fieldsize): tag(tag)
   ,width(fieldsize)
   ,fieldOflags(0)
   ,printNum(-1)
{
    if(tag != D_REG && tag != D_CONST && tag != D_ADDRESS && tag != D_OFFSET && tag != D_STACKOFFSET && tag != D_SPILLED)
        throw std::string("Binary translator: wrong detail constructor.");
}

Binatr::Detail::Detail(int tag, uint64_t val, size_t fieldsize):tag(tag)
   ,width(fieldsize)
   ,fieldOflags(val)
   ,printNum(-1)
{
    if(tag != D_STATIC)
        throw std::string("Binary translator: wrong detail constructor.");
}

Binatr::Binatr(std::initializer_list<Detail> lst) : m_compound(lst), m_bytewidth(0)
{
    for (const Detail& det:m_compound)
        m_bytewidth += det.width;
    m_bytewidth = m_bytewidth/8 + ((m_bytewidth%8)?1:0);
}

size_t Binatr::size(const Syntop& op) const 
{
    size_t res = m_bytewidth;
#if defined(_WIN32)                                      //TODO(ch): IMPORTANT. Destroy it. Devastate. Annihilate. 
    switch (m_compound[0].fieldOflags)
    {
    case(0x224):
    {
        size_t statn = (op[1].idx > 7) ? 1 : 0;
        statn |= (op[0].idx > 7) ? 2 : 0;
        static uint64_t sadd[4] = { 0, 1, 1, 1 };
        res += sadd[statn];
        break;
    }
    default:
        break;
    }
#endif
    return res; 
}

void Binatr::applyNAppend(const Syntop& op, Bitwriter* bits) const
{
    if (bits == nullptr)
       throw std::string("Binary translator: null writer pointer.");
    bits->startInstruction();
    size_t argnum = 0;
    std::vector<size_t> reordering;
    if (!m_reordering.size())
    {
        reordering.reserve(op.size());
        for (size_t rodr = 0; rodr < op.size(); rodr++) reordering.push_back(rodr);
    }
    else
        reordering = m_reordering;

#if defined(_WIN32)                                      //TODO(ch): IMPORTANT. Destroy it. Devastate. Annihilate. 
    std::vector<Detail> intelRegisterTrick = m_compound;
    std::vector<Detail> m_compound = intelRegisterTrick;
    switch (m_compound[0].fieldOflags)
    {
    case(0x918F8):
        if (op[1].idx > 7)
            m_compound[0].fieldOflags = 0x938F8;
        break;
    case(0x1218C):
    {
        if (op.size() == 2)
        {
            size_t statn = (op[0].idx > 7) ? 1 : 0;
            statn |= (op[1].idx > 7) ? 2 : 0;
            static uint64_t stats[4] = { 0x1218C , 0x1318C, 0x1258C, 0x1358C };
            m_compound[0].fieldOflags = stats[statn];
        } else if (op.size() == 3)
        {
            size_t statn = (op[0].idx > 7) ? 1 : 0;
            statn |= (op[2].idx > 7) ? 2 : 0;
            statn |= (op[1].idx > 7) ? 4 : 0;
            static uint64_t stats[8] = { 0x1218C, 0x1318C, 0x1258C, 0x1358C, 0x1298C, 0x1398C, 0x12D8C, 0x13D8C };
            m_compound[0].fieldOflags = stats[statn];
        }
        break;
    }
    case(0x120E7):
    {
        size_t statn = (op[1].idx > 7) ? 1 : 0;
        statn |= (op[0].idx > 7) ? 2 : 0;
        static uint64_t stats[4] = { 0x120E7, 0x124E7, 0x130E7, 0x134E7 };
        m_compound[0].fieldOflags = stats[statn];
        break;
    }
    case(0x120E5):
    {
        if (op[1].idx > 7)
            m_compound[0].fieldOflags = 0x130E5;
        break;
    }
    case(0x120ED):
    {
        if (op[0].idx > 7)
            m_compound[0].fieldOflags = 0x130ED;
        break;
    }
    case(0x12227):
    {
        size_t statn = (op[1].idx > 7) ? 1 : 0;
        statn |= (op[0].idx > 7) ? 2 : 0;
        static uint64_t stats[4] = { 0x12227, 0x12627, 0x13227, 0x13627 };
        m_compound[0].fieldOflags = stats[statn];
        break;
    }
    case(0x12007):
    {
        size_t statn = (op[1].idx > 7) ? 1 : 0;
        statn |= (op[0].idx > 7) ? 2 : 0;
        static uint64_t stats[4] = { 0x12007, 0x13007, 0x12407, 0x13407 };
        m_compound[0].fieldOflags = stats[statn];
        break;
    }
    case(0x224):
    {
        size_t statn = (op[1].idx > 7) ? 1 : 0;
        statn |= (op[0].idx > 7) ? 2 : 0;
        static uint64_t stats[4] = { 0x224, 0x10624, 0x11224, 0x11624 };
        static uint64_t statw[4] = { 10, 18, 18, 18 };
        m_compound[0].fieldOflags = stats[statn];
        m_compound[0].width = statw[statn];
        break;
    }
    case(0x12225):
    {
        if (op[0].idx > 7)
            m_compound[0].fieldOflags = 0x13225;
        break;
    }
    case(0x1222D):
    {
        if (op[1].idx > 7)
            m_compound[0].fieldOflags = 0x1322D;
        break;
    }
    default:
        break;
    }
#endif

    for(const Detail& det : m_compound)
        switch (det.tag)
        {
            case (Detail::D_REG):
                if(op.args[reordering[argnum]].tag != Arg::IREG)
                    throw std::string("Binary translator: syntop bring const instead of register.");
                ++argnum;
                break;
            case (Detail::D_SPILLED):
                if (op.args[reordering[argnum]].tag != Arg::ISPILLED)
                    throw std::string("Binary translator: syntop bring active register instead of spilled.");
                ++argnum;
                break;
            case (Detail::D_CONST):
            case (Detail::D_ADDRESS):
            case (Detail::D_OFFSET):
            case (Detail::D_STACKOFFSET):
                if(op.args[reordering[argnum]].tag != Arg::ICONST)
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
                piece = op[reordering[argnum++]].idx;
                break;
            case (Detail::D_ADDRESS):
            {
//                canvas->m_addresses.push_back(bits->bitaddress()); //TODO(ch): Place adresses postions somewhere! I think, into FuncImpl.
            }
            case (Detail::D_CONST):
            case (Detail::D_OFFSET):
            case (Detail::D_STACKOFFSET):
            case (Detail::D_SPILLED):
                piece = static_cast<uint64_t>(op[reordering[argnum++]].value);
                if (bits->getBackend()->isLittleEndianOperands())
                    piece = Bitwriter::revertDetail(piece, det.width);
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
