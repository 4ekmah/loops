/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "composer.hpp"
#include "backend.hpp"
#include <cstring>

namespace loops
{

Bitwriter::Bitwriter(const Backend* a_backend) : m_backend(a_backend)
    , m_buffer(std::make_shared<std::vector<uint8_t> >((size_t)MINIMAL_BUFFER_SIZE, (uint8_t)0))
    , m_size(0)
    , m_bitpos(0)
    , m_startsize(NOTRANSACTION)
{}

void Bitwriter::startInstruction()
{
    if (m_startsize != NOTRANSACTION)
        throw std::runtime_error("Bitwriter: instruction is already started");
    m_startsize = m_size;
}

void Bitwriter::writeToken(uint64_t a_token, int a_fieldwidth)
{
    if (a_fieldwidth > 64)
        throw std::runtime_error("Bitwriter: too big bitfield");
    if (m_startsize == NOTRANSACTION)
        throw std::runtime_error("Bitwriter: writing token out of instruction");
    if((int)m_buffer->size() < m_size + ((m_bitpos + a_fieldwidth) >> 3) + 1)
        m_buffer->resize(m_buffer->size() << 1 );
    uint8_t* buffer = m_buffer->data();
    uint64_t body = m_bitpos ? ((uint64_t)(*(buffer + m_size))) << 56 : 0;
    a_token = (a_fieldwidth < 64)? a_token & ( ((uint64_t)(1) << a_fieldwidth) - 1): a_token;
    const size_t bits2write = m_bitpos + a_fieldwidth;
    const size_t bytes2write = (bits2write >> 3) + (bits2write%8 ? 1 : 0);
    uint8_t tail = 0;
    if (bits2write > 64)
    {
        size_t tailwidth = bits2write - 64;
        tail = (uint8_t)((a_token & ( ((uint64_t)(1) << tailwidth) - 1)) << (8 - tailwidth));
        a_token = a_token >> (tailwidth);
        body |= a_token;
    }
    else
    {
        a_token = a_token << (64 - bits2write);
        body |= a_token;
    }
    uint8_t* bytearray = reinterpret_cast<uint8_t*>(&body);
    int lowerbound = 8 - (int)bytes2write;
    lowerbound = lowerbound < 0 ? 0 : lowerbound;
    for(int byten = 7; byten >= lowerbound; --byten)
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
        const int instrsize = m_backend->instructionWidth();
        if (m_size - m_startsize != instrsize)
            throw std::runtime_error("Bitwriter: non-standard instruction width.");
    }
    if(m_backend->isLittleEndianInstructions())
    {
        if(m_size > m_startsize + 1)
        {
            int lesser = m_startsize;
            int bigger = m_size - 1;
            for(;lesser<bigger;++lesser,--bigger)
                std::swap(m_buffer->operator[](lesser), m_buffer->operator[](bigger));
        }
    }
    m_startsize = NOTRANSACTION;
}

uint64_t Bitwriter::revertToken(uint64_t a_tok, int dwidth)
{
    dwidth = (dwidth + 7) >> 3;
    int endpos = dwidth - 1;
    dwidth = dwidth >> 1;
    uint8_t* bytearr = reinterpret_cast<uint8_t*>(&a_tok);
    for (int p1 = 0; p1 < dwidth; p1++)
        std::swap(bytearr[p1], bytearr[endpos - p1]);
    return a_tok;
}

BinTranslation::Token::Token(int tag, int fieldsize): tag(tag)
   , width(fieldsize)
   , fieldOflags(0)
   , srcArgnum(UNDEFINED_ARGUMENT_NUMBER)
{
    if(tag != T_REG && tag != T_IMMEDIATE && tag != T_SPILLED && tag != T_OMIT)
        throw std::runtime_error("Binary translator: wrong token constructor.");
    if(tag == T_OMIT && fieldsize != 0) 
        throw std::runtime_error("Binary translator: omit immediate must not have field width.");
}

BinTranslation::Token::Token(int tag, uint64_t val, int fieldsize):tag(tag)
   , width(fieldsize)
   , fieldOflags(val)
   , srcArgnum(UNDEFINED_ARGUMENT_NUMBER)
{
    if(tag != T_STATIC)
        throw std::runtime_error("Binary translator: wrong token constructor.");
}

BinTranslation::BinTranslation(std::initializer_list<Token> lst) : m_compound(lst), m_bytewidth(0)
{
    for (const Token& det:m_compound)
        m_bytewidth += det.width;
    m_bytewidth = m_bytewidth/8 + ((m_bytewidth%8)?1:0);
}

BinTranslation::BinTranslation(const std::vector<Token>& lst) : m_compound(lst), m_bytewidth(0)
{
    for (const Token& det:m_compound)
        m_bytewidth += det.width;
    m_bytewidth = m_bytewidth/8 + ((m_bytewidth%8)?1:0);
}

void BinTranslation::applyNAppend(const Syntop& op, Bitwriter* bits) const
{
    if(m_compound.size() == 0) 
        return;
    if (bits == nullptr)
        throw std::runtime_error("Binary translator: null writer pointer.");
    bits->startInstruction();
    uint64_t argmask = (uint64_t(1) << op.size()) - 1;
    for (const Token& det : m_compound)
    {
        uint64_t pos = (det.srcArgnum != UNDEFINED_ARGUMENT_NUMBER) ? uint64_t(1) << det.srcArgnum : 0;

        switch (det.tag)
        {
        case (Token::T_REG):
            if (op.args[det.srcArgnum].tag != Arg::IREG && op.args[det.srcArgnum].tag != Arg::VREG)
                throw std::runtime_error("Binary translator: syntop bring const instead of register.");
            argmask = (argmask | pos) ^ pos;
            break;
        case (Token::T_SPILLED):
            if (op.args[det.srcArgnum].tag != Arg::ISPILLED)
                throw std::runtime_error("Binary translator: syntop bring active register or const instead of spilled.");
            argmask = (argmask | pos) ^ pos;
            break;
        case (Token::T_IMMEDIATE):
            if (op.args[det.srcArgnum].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Binary translator: syntop bring register instead of const.");
            argmask = (argmask | pos) ^ pos;
            break;
        case (Token::T_STATIC): break;
        case (Token::T_OMIT):
            argmask = (argmask | pos) ^ pos;
            break;
        default:
            throw std::runtime_error("Binary translator: unknown token type.");
        };
    }
    Assert(argmask == 0);
    for (const Token& det : m_compound)
    {
        if(det.tag == Token::T_OMIT)
            continue;
        uint64_t piece = 0;
        switch (det.tag)
        {
        case (Token::T_STATIC):
            piece = det.fieldOflags;
            break;
        case (Token::T_REG):
            piece = op.args[det.srcArgnum].idx;
            break;
        case (Token::T_IMMEDIATE):
        case (Token::T_SPILLED):
            piece = static_cast<uint64_t>(op.args[det.srcArgnum].value);
            if (det.tag == Token::T_SPILLED)
                piece *= 8; //TODO(ch): It's intel-specific(well, actually ISPILLED is also intel specific.) 
            if (bits->getBackend()->isLittleEndianOperands())
                piece = Bitwriter::revertToken(piece, det.width);
            break;
        default:
            throw std::runtime_error("Binary translator: unknown token type.");
        };
        bits->writeToken(piece, det.width);
    }
    bits->endInstruction();
    return;
}
}
