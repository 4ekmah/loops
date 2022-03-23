/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "composer.hpp"

namespace loops
{

i_snippet::detail::detail(int tag, size_t fieldsize): m_tag(tag), m_fieldsize(fieldsize)
{
    if(m_tag != D_REG && m_tag != D_CONST && m_tag != D_ADDRESS && m_tag != D_OFFSET && m_tag != D_STACKOFFSET)
        throw std::string("Wrong snippet constructor.");
}

i_snippet::detail::detail(int tag, uint64_t val, size_t fieldsize): m_tag(tag), m_fieldsize(fieldsize), m_val(val)
{
    if(m_tag != D_STATIC && m_tag != D_VARIABLE)
        throw std::string("Wrong snippet constructor.");
}

i_snippet::detail::detail(int tag, uint64_t val, uint64_t alterval, size_t corres_pos, size_t fieldsize): m_tag(tag), m_fieldsize(fieldsize), m_val(val), m_alterval(alterval)
{
    if(m_tag != D_SWITCHER)
        throw std::string("Wrong snippet constructor.");
}

i_snippet::i_snippet(std::initializer_list<detail> lst) : m_compound(lst), m_size(0)
{
    for (auto det:m_compound)
        m_size += det.m_fieldsize;
    m_size = m_size/8 + ((m_size%8)?1:0);
}

union bytificator {
    uint8_t bytes[8];
    uint64_t field;
};

inline void invertbytes(bytificator& bytes)
{
    bytificator intermediate = bytes;
    bytes.bytes[0] = intermediate.bytes[7];
    bytes.bytes[1] = intermediate.bytes[6];
    bytes.bytes[2] = intermediate.bytes[5];
    bytes.bytes[3] = intermediate.bytes[4];
    bytes.bytes[4] = intermediate.bytes[3];
    bytes.bytes[5] = intermediate.bytes[2];
    bytes.bytes[6] = intermediate.bytes[1];
    bytes.bytes[7] = intermediate.bytes[0];
}

void i_snippet::apply_n_append(const std::vector<Arg>& args, p_canvas* canvas) const
{
    canvas->m_buffer.resize(canvas->m_buffer.size() + size(),0);
    uint8_t* pos = &(*(canvas->m_buffer.end())) - size();
    uint8_t bitpos = 0;
    size_t argnum = 0;
    int switchercounter = 0;
    std::vector<detail> compound = m_compound;
    for(const detail& det : compound)
        if(det.m_tag == detail::D_SWITCHER)
            switchercounter++; //TODO(ch): calculate this in constructor;
    for(detail& det : compound)
        switch (det.m_tag)
        {
            case (detail::D_STATIC):
            case (detail::D_SWITCHER):break;
            case (detail::D_REG):
            case (detail::D_CONST):
            case (detail::D_ADDRESS):
            case (detail::D_OFFSET):
            case (detail::D_STACKOFFSET):
                argnum++;
                break;
            case (detail::D_VARIABLE):
            {
                const size_t sw_pos = det.m_val;
                if(sw_pos >= compound.size() ||
                   compound[sw_pos].m_tag!= detail::D_SWITCHER)
                    throw std::string("Wrong snippet switcher.");
                if (argnum >= args.size())
                    throw std::string("Inconsistent amount of arguments for snippet.");
                int64_t newstatic;
                int newargtype = detail::D_REG;
                switch(args[argnum].tag)
                {
                    case (Arg::IREG) :
                        newargtype = detail::D_REG;
                        newstatic = compound[sw_pos].m_val; break;
                    case (Arg::ICONST) :
                        newargtype = detail::D_CONST;
                        newstatic = compound[sw_pos].m_alterval; break;
                    default:
                        throw std::string("Unexpected argument type.");
                };
                det = detail(newargtype, det.m_fieldsize);
                compound[sw_pos] = detail(detail::D_STATIC, newstatic, compound[sw_pos].m_fieldsize);
                argnum++;
                switchercounter--;
            } break;
            default:
                throw std::string("Unknown snippet detail type.");
        };
    if(switchercounter != 0)
        throw std::string("Amounts of switcher and variable are not equal.");
    if(argnum != args.size())
        throw std::string("Amounts of arguments and placeholder are not equal.");
    argnum = 0;
    //TODO(ch): PRIORITY! rewrite this really bad and inaccurate bitstream.
    //E.g, if we have m_fieldsize == 64, and bitpos == 1..7, it will fail, obviously(because we are not writing tail)
    for(detail& det : compound)
    {
        uint64_t piece = 0;
        switch (det.m_tag)
        {
            case (detail::D_STATIC):
                piece = det.m_val;
                break;
            case (detail::D_REG):
                piece = args[argnum++].idx;
                break;
            case (detail::D_ADDRESS):
            {
                size_t addrpos = (size_t)(pos - (&(*canvas->m_buffer.begin()))) * 8;
                addrpos += bitpos;
                canvas->m_addresses.push_back(addrpos);
            }
            case (detail::D_CONST):
            case (detail::D_OFFSET):
            case (detail::D_STACKOFFSET):
                piece = static_cast<uint64_t>(args[argnum++].value);
                break;
            default:
                throw std::string("Unknown snippet detail type.");
        };
        bytificator microarray; //TODO(ch): too little usage for new type.
        microarray.field = 0;
        microarray.bytes[7] = *pos;
        piece = piece & ( (1 << det.m_fieldsize) - 1); //zeroing everything left to fieldsize(needed for negative numbers)
        piece = piece << (64 - bitpos - det.m_fieldsize);
        microarray.field |= piece;
        size_t bits2write = bitpos + det.m_fieldsize;
        size_t bytes2write = bits2write/8 + ((bits2write%8)?1:0);
        invertbytes(microarray); //TODO(ch): I'm sure that we will use better solution for bitfields, but, still, ensure on all the platforms, that they are using little-endian.
        memcpy(pos,microarray.bytes,bytes2write);
        pos += bits2write / 8;
        bitpos = bits2write % 8;
    }
    //TODO(ch): PRIORITY! Unbelievable workaround. We cannot be sure, that every instruction will be 4-byte-wide and that all instructions will be written as little-endian, but for now... it will work.
    bytificator afterreverter;
    afterreverter.field = *(reinterpret_cast<uint32_t*>(pos - 4));
    invertbytes(afterreverter);
    uint32_t reverted4 = *(reinterpret_cast<uint32_t*>(&(afterreverter.field))+1);
    *(reinterpret_cast<uint32_t*>(pos - 4)) = reverted4;
}

void p_canvas::fromsynt(const instruction_set& a_iset, const std::vector<syntop> a_syntlist)
{
    for(const syntop& op: a_syntlist)
    {
        auto snippetrator = a_iset.find(op.opcode);
        if (snippetrator == a_iset.end())
            throw std::string("Synt2bin translation: unknown instruction");
        snippetrator->second.apply_n_append(op.args, this);
    }
}
};
