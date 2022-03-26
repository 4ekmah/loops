/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "composer.hpp"
#include <cstring>

namespace loops
{

#define MINIMAL_BUFFER_SIZE 512
bitwriter::bitwriter(): m_alloc(nonsys_alloc::get_instance())
    , m_buffer(m_alloc->allocate(512))
    , m_size(0)
    , m_maxsize(MINIMAL_BUFFER_SIZE)
    , m_bitpos(0)
    , m_startsize(NOTRANSACTION)
{}

void bitwriter::startinstruction()
{
    if (m_startsize != NOTRANSACTION)
        throw std::string("Bitstream: instruction is already started");
    m_startsize = m_size;
}

void bitwriter::writedetail(uint64_t a_detail, size_t a_fieldwidth)
{
    if (a_fieldwidth > 64)
        throw std::string("Bitstream: too big bitfield");
    if (m_startsize == NOTRANSACTION)
        throw std::string("Bitstream: writing detail out of instruction");
    if(m_maxsize < m_size + (a_fieldwidth >> 3) + 1)
    {
        m_maxsize <<= 1;
        m_buffer = m_alloc->expand(m_buffer, m_maxsize);
    }
    uint64_t body = m_bitpos ? ((uint64_t)(*(m_buffer + m_size))) << 56 : 0;
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
        m_buffer[m_size++] = bytearray[byten];
    if(bytes2write == 9)
        m_buffer[m_size++] = tail;
    m_bitpos = bits2write % 8;
    if (m_bitpos)
        --m_size;
}

void bitwriter::endinstruction()
{
    if (m_bitpos != 0)
        throw std::string("Bitstream: instructions with width, not multiple of 8 bits are not supported.");
    if (m_startsize == NOTRANSACTION)
        throw std::string("Bitstream: finishing instruction wasn't start.");

    //if(backend->is_monowidth() && monowidth == 4) //TODO(ch): Declare this functions in backend.
    {
        const size_t instrsize = 4;
        if (m_size - m_startsize != instrsize)
            throw std::string("Bitstream: non-standard instruction width.");
    }
//    if(backend->isLittleEndianInstructions()) //TODO(ch): Declare backend function.
    {
        if(m_size > m_startsize + 1)
        {
            size_t lesser = m_startsize;
            size_t bigger = m_size - 1;
            for(;lesser<bigger;++lesser,--bigger)
                std::swap(m_buffer[lesser], m_buffer[bigger]);
        }
    }
    m_startsize = NOTRANSACTION;
}

binatr::detail::detail(int tag, size_t fieldsize): m_tag(tag), m_fieldwidth(fieldsize)
{
    if(m_tag != D_REG && m_tag != D_CONST && m_tag != D_ADDRESS && m_tag != D_OFFSET && m_tag != D_STACKOFFSET)
        throw std::string("Wrong snippet constructor.");
}

binatr::detail::detail(int tag, uint64_t val, size_t fieldsize): m_tag(tag), m_fieldwidth(fieldsize), m_field(val)
{
    if(m_tag != D_STATIC)
        throw std::string("Wrong snippet constructor.");
}

binatr::binatr(std::initializer_list<detail> lst) : m_compound(lst), m_size(0)
{
    for (auto det:m_compound)
        m_size += det.m_fieldwidth;
    m_size = m_size/8 + ((m_size%8)?1:0);
}

void binatr::apply_n_append(const std::vector<Arg>& args, bitwriter* bits) const
{
    if (bits == nullptr)
       throw std::string("I_Snippet: null pointer to writer.");
    bits->startinstruction();
    size_t argnum = 0;
    for(const detail& det : m_compound)
        switch (det.m_tag)
        {
            case (detail::D_REG):
            case (detail::D_CONST):
            case (detail::D_ADDRESS):
            case (detail::D_OFFSET):
            case (detail::D_STACKOFFSET):
                argnum++;
                break;
            case (detail::D_STATIC): break;
            default:
                throw std::string("Unknown snippet detail type.");
        };
    if(argnum != args.size())
        throw std::string("Amounts of arguments and placeholder are not equal.");
    argnum = 0;
    for(const detail& det : m_compound)
    {
        uint64_t piece = 0;
        switch (det.m_tag)
        {
            case (detail::D_STATIC):
                piece = det.m_field;
                break;
            case (detail::D_REG):
                piece = args[argnum++].idx;
                break;
            case (detail::D_ADDRESS):
            {
//                canvas->m_addresses.push_back(bits->bitaddress()); //IMPORTANT: Place adressess somewhere! I think, in FuncImpl
            }
            case (detail::D_CONST):
            case (detail::D_OFFSET):
            case (detail::D_STACKOFFSET):
                piece = static_cast<uint64_t>(args[argnum++].value);
                break;
            default:
                throw std::string("Unknown snippet detail type.");
        };
        bits->writedetail(piece, det.m_fieldwidth);
    }
    bits->endinstruction();
}

bool binatr::is_const_fit(const syntop& a_op, size_t argnum) const
{
    if(argnum >= a_op.args.size())
        throw std::string("Binary translator: non-existent argument is requested.");
    if(a_op.args[argnum].tag != Arg::ICONST)
        throw std::string("Binary translator: register instead of const.");
    uint64_t val_to_be_fit = a_op.args[argnum].value;
    for(const detail& det : m_compound)
        if(det.m_tag != detail::D_STATIC)
        {
            if(argnum == 0)
            {
                if(det.m_tag == detail::D_REG)
                    throw std::string("Binary translator: register instead of const.");
                size_t bitwneeded = 0;
                for (;bitwneeded < 63; bitwneeded++) //TODO(ch): what a shame! Give normal implementation! AND!!! Use info about type(SIGNED/UNSIGNED offsets, adresses, etc.)
                    if(val_to_be_fit <= (((uint64_t)(1))<<bitwneeded))
                        break;
                return (bitwneeded <= det.m_fieldwidth);
            }
            argnum--;
        }
    throw std::string("Binary translator: non-existent argument is requested.");
};

void p_canvas::fromsynt(const m2b_map& a_iset, const std::vector<syntop> a_syntlist)
{
    bitwriter bitstream;
    for(const syntop& op: a_syntlist)
    {
        auto snippetrator = a_iset.find(op.opcode);
        if (snippetrator == a_iset.end())
            throw std::string("Synt2bin translation: unknown instruction");
        const binatr& bitexpansioner = snippetrator->second[op.args];
        bitexpansioner.apply_n_append(op.args, &bitstream);
    }
    m_buffer.resize(bitstream.size(), 0);
    std::copy(bitstream.buffer(), bitstream.buffer() + bitstream.size(), m_buffer.begin());
}
};
