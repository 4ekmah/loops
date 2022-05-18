/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_COMPOSER_HPP__
#define __LOOPS_COMPOSER_HPP__

#include "loops/loops.hpp"
#include "common.hpp"
#include "allocator.hpp"
#include "printer.hpp"
#include <vector>
#include <unordered_map>

namespace loops
{
    class Backend;
    typedef std::shared_ptr<std::vector<uint8_t> > FuncBodyBuf;
    class Bitwriter
    {
    public:
        Bitwriter(const Backend* a_backend);
        void startInstruction();
        void writeDetail(uint64_t a_detail, size_t a_fieldwidth);
        void endInstruction();
        const FuncBodyBuf buffer() const
        {
            return std::make_shared<std::vector<uint8_t> >(m_buffer->data(), m_buffer->data() + m_size); //TODO(ch): eliminate unneccessary copy.
        }
        inline size_t bitAddress() { return (m_size << 3) + m_bitpos; }
        const Backend* getBackend() const { return m_backend; }
        static uint64_t revertDetail(uint64_t a_det, size_t dwidth);
    private:
        const Backend* m_backend;
        FuncBodyBuf m_buffer;
        size_t m_size;
        size_t m_bitpos;
        size_t m_startsize;
        enum {NOTRANSACTION = -1, MINIMAL_BUFFER_SIZE = 512};
    };
    
//TODO(ch):
// Our printer:    ldr   x5, [sp], #0x05 ; e5 17 40 f9
// Normal printer: ldr   x5, [sp, #0x28]
// This #0x05 must fixed. For such a thing we need more complex out modifiers, than a flags. In this case it's
// D_OUTMULTIPLIER(8).
// Same for current D_32Dep, which must be dependend on number of argument.
// Same for [base,offset] pairs.
//TODO(ch): Binatr collection is static data. This collection must be created at compile time. Currently, even if we will create this collection as global object, it will be initialized at runtime with unnecessary computational losts.
    struct Binatr //is for "binary translation"
    {
        struct Detail
        {
            //TODO(ch): Actually, it looks like, we need only adresses, statics, and common-use-arguments.
            enum {D_STATIC, D_REG, D_CONST, D_ADDRESS, D_OFFSET, D_STACKOFFSET, D_SPILLED};
            enum {D_INPUT = 1, D_OUTPUT = 2};
            Detail(int tag, size_t fieldsize);
            Detail(int tag, uint64_t val, size_t fieldsize);
            int tag;
            size_t width; //in bits //TODO(ch): unsigned char?
            uint64_t fieldOflags;
            inline uint64_t flags() { return fieldOflags;}
            size_t arVecNum;
        };
        std::vector<Detail> m_compound;
        size_t m_bytewidth;
        Binatr(): m_bytewidth(0) {}
        Binatr(std::initializer_list<Detail> lst);
        size_t size() const { return m_bytewidth; }
        void applyNAppend(const Syntop& op, Bitwriter* bits) const;
    };

    namespace BinatrTableConstructor
    {
        inline Binatr BiT(std::initializer_list<Binatr::Detail> details)
        {
            return Binatr(details);
        }

        inline Binatr::Detail BDsta(uint64_t field, size_t width) {return Binatr::Detail(Binatr::Detail::D_STATIC, field, width); }

        inline Binatr::Detail BDreg(size_t arVecNum, size_t width, uint64_t regflag = Binatr::Detail::D_INPUT | Binatr::Detail::D_OUTPUT)
        {
            Binatr::Detail res(Binatr::Detail::D_REG, width);
            res.fieldOflags = regflag;
            res.arVecNum = arVecNum;
            return res;
        }

        inline Binatr::Detail BDspl(size_t arVecNum, size_t width)
        {
            Binatr::Detail res(Binatr::Detail::D_SPILLED, width);
            res.arVecNum = arVecNum;
            return res;
        }

        inline Binatr::Detail BDcon(size_t arVecNum, size_t width, uint64_t flags = 0)//TODO(ch): Rename const -> immediate, BDcon->BDimm
        {
            Binatr::Detail res(Binatr::Detail::D_CONST, width);
            res.arVecNum = arVecNum;
            res.fieldOflags = flags;
            return res;
        }

        inline Binatr::Detail BDoff(size_t arVecNum, size_t width)
        {
            Binatr::Detail res(Binatr::Detail::D_OFFSET, width);
            res.arVecNum = arVecNum;
            return res;
        }

        enum {In = Binatr::Detail::D_INPUT, Out = Binatr::Detail::D_OUTPUT, IO = Binatr::Detail::D_OUTPUT};
    };
};

#endif //__LOOPS_COMPOSER_HPP__
