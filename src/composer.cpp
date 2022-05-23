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
        throw std::runtime_error("Bitwriter: instruction is already started");
    m_startsize = m_size;
}

void Bitwriter::writeDetail(uint64_t a_detail, size_t a_fieldwidth)
{
    if (a_fieldwidth > 64)
        throw std::runtime_error("Bitwriter: too big bitfield");
    if (m_startsize == NOTRANSACTION)
        throw std::runtime_error("Bitwriter: writing detail out of instruction");
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
        throw std::runtime_error("Bitwriter: instructions with width, not multiple of 8 bits are not supported.");
    if (m_startsize == NOTRANSACTION)
        throw std::runtime_error("Bitwriter: finishing instruction wasn't start.");

    if(m_backend->isMonowidthInstruction())
    {
        const size_t instrsize = m_backend->instructionWidth();
        if (m_size - m_startsize != instrsize)
            throw std::runtime_error("Bitwriter: non-standard instruction width.");
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

Binatr::Detail::Detail(int tag, size_t fieldsize): tag(tag), arVecNum(-1)
   ,width(fieldsize)
   ,fieldOflags(0)
{
    if(tag != D_REG && tag != D_IMMEDIATE && tag != D_ADDRESS && tag != D_OFFSET && tag != D_STACKOFFSET && tag != D_SPILLED)
        throw std::runtime_error("Binary translator: wrong detail constructor.");
}

Binatr::Detail::Detail(int tag, uint64_t val, size_t fieldsize):tag(tag)
   ,width(fieldsize)
   ,fieldOflags(val)
   ,arVecNum(-1)
{
    if(tag != D_STATIC)
        throw std::runtime_error("Binary translator: wrong detail constructor.");
}

Binatr::Binatr(std::initializer_list<Detail> lst) : m_compound(lst), m_bytewidth(0)
{
    for (const Detail& det:m_compound)
        m_bytewidth += det.width;
    m_bytewidth = m_bytewidth/8 + ((m_bytewidth%8)?1:0);
}

void Binatr::applyNAppend(const Syntop& op, Bitwriter* bits) const
{
    if (bits == nullptr)
        throw std::runtime_error("Binary translator: null writer pointer.");
            bits->startInstruction();
            uint64_t argmask = (uint64_t(1) << op.size()) - 1;
    for (const Detail& det : m_compound)
    {
        uint64_t pos = (det.arVecNum != -1) ? uint64_t(1) << det.arVecNum : 0;

        switch (det.tag)
        {
        case (Detail::D_REG):
            if (op.args[det.arVecNum].tag != Arg::IREG)
                throw std::runtime_error("Binary translator: syntop bring const instead of register.");
            argmask = (argmask | pos) ^ pos;
            break;
        case (Detail::D_SPILLED):
            if (op.args[det.arVecNum].tag != Arg::ISPILLED)
                throw std::runtime_error("Binary translator: syntop bring active register or const instead of spilled.");
            argmask = (argmask | pos) ^ pos;
            break;
        case (Detail::D_IMMEDIATE):
        case (Detail::D_ADDRESS):
        case (Detail::D_OFFSET):
        case (Detail::D_STACKOFFSET):
            if (op.args[det.arVecNum].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Binary translator: syntop bring register instead of const.");
            argmask = (argmask | pos) ^ pos;
            break;
        case (Detail::D_STATIC): break;
        default:
            throw std::runtime_error("Binary translator: unknown detail type.");
        };
    }
    Assert(argmask == 0);
    for (const Detail& det : m_compound)
    {
        uint64_t piece = 0;
        switch (det.tag)
        {
        case (Detail::D_STATIC):
            piece = det.fieldOflags;
            break;
        case (Detail::D_REG):
            piece = op.args[det.arVecNum].idx;
            break;
        case (Detail::D_ADDRESS):
        {
            //                canvas->m_addresses.push_back(bits->bitaddress()); //TODO(ch): Place adresses postions somewhere! I think, into FuncImpl.
        }
        case (Detail::D_IMMEDIATE):
        case (Detail::D_OFFSET):
        case (Detail::D_STACKOFFSET):
        case (Detail::D_SPILLED):
            piece = static_cast<uint64_t>(op.args[det.arVecNum].value);
            if(det.fieldOflags & Detail::D_INVERT_IMM)
                piece = ~piece;
            if (det.tag == Detail::D_SPILLED)
                piece *= 8; //TODO(ch): It's intel-specific(well, actually ISPILLED is also intel specific.) 
            if (bits->getBackend()->isLittleEndianOperands())
                piece = Bitwriter::revertDetail(piece, det.width);
            break;
        default:
            throw std::runtime_error("Binary translator: unknown detail type.");
        };
        bits->writeDetail(piece, det.width);
    }
    bits->endInstruction();
    return;
}
};
