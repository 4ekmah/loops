/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
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
        void writeToken(uint64_t a_token, int a_fieldwidth);
        void endInstruction();
        const FuncBodyBuf buffer() const
        {
            return std::make_shared<std::vector<uint8_t> >(m_buffer->data(), m_buffer->data() + m_size); //TODO(ch): eliminate unneccessary copy.
        }
        inline int bitAddress() { return (m_size << 3) + m_bitpos; }
        const Backend* getBackend() const { return m_backend; }
        static uint64_t revertToken(uint64_t a_tok, int dwidth);
    private:
        const Backend* m_backend;
        FuncBodyBuf m_buffer;
        int m_size;
        int m_bitpos;
        int m_startsize;
        enum {NOTRANSACTION = -1, MINIMAL_BUFFER_SIZE = 512};
    };
    
//TODO(ch):
// Our printer:    ldr   x5, [sp], #0x05 ; e5 17 40 f9
// Normal printer: ldr   x5, [sp, #0x28]
// This #0x05 must fixed. For such a thing we need more complex out modifiers, than a flags. In this case it's
// T_OUTMULTIPLIER(8).
// Same for [base,offset] pairs.
//TODO(ch): BinTranslation collection is static data. This collection must be created at compile time. Currently, even if we will create this collection as global object, it will be initialized at runtime with unnecessary computational losts.
    struct BinTranslation
    {
        struct Token
        {
            //TODO(ch): Actually, it looks like, we need only adresses, statics, and common-use-arguments.
            enum {T_STATIC, T_REG, T_IMMEDIATE, T_ADDRESS, T_OFFSET, T_STACKOFFSET, T_SPILLED, T_OMIT};
            enum {T_INPUT = 1, T_OUTPUT = 2, T_INVERT_IMM = 4};
            Token(int tag, int fieldsize);
            Token(int tag, uint64_t val, int fieldsize);
            int tag;
            int width; //in bits //TODO(ch): unsigned char?
            uint64_t fieldOflags;
            inline uint64_t flags() { return fieldOflags;}
            int srcArgnum;
        };
        std::vector<Token> m_compound;
        int m_bytewidth;
        BinTranslation(): m_bytewidth(0) {}
        BinTranslation(std::initializer_list<Token> lst);
        int size() const { return m_bytewidth; }
        void applyNAppend(const Syntop& op, Bitwriter* bits) const;
    };

    namespace BinTranslationConstruction
    {
        inline BinTranslation BiT(std::initializer_list<BinTranslation::Token> tokens)
        {
            return BinTranslation(tokens);
        }

        inline BinTranslation::Token BTsta(uint64_t field, int width) {return BinTranslation::Token(BinTranslation::Token::T_STATIC, field, width); }

        inline BinTranslation::Token BTomm(int srcArgnum)
        {
            BinTranslation::Token res(BinTranslation::Token::T_OMIT, 0);
            res.srcArgnum = srcArgnum;
            return res; 
        }

        inline BinTranslation::Token BTreg(int srcArgnum, int width, uint64_t regflag = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT)
        {
            BinTranslation::Token res(BinTranslation::Token::T_REG, width);
            res.fieldOflags = regflag;
            res.srcArgnum = srcArgnum;
            return res;
        }

        inline BinTranslation::Token BTspl(int srcArgnum, int width)
        {
            BinTranslation::Token res(BinTranslation::Token::T_SPILLED, width);
            res.srcArgnum = srcArgnum;
            return res;
        }

        inline BinTranslation::Token BTimm(int srcArgnum, int width, uint64_t flags = 0)
        {
            BinTranslation::Token res(BinTranslation::Token::T_IMMEDIATE, width);
            res.srcArgnum = srcArgnum;
            res.fieldOflags = flags;
            return res;
        }

        inline BinTranslation::Token BToff(int srcArgnum, int width)
        {
            BinTranslation::Token res(BinTranslation::Token::T_OFFSET, width);
            res.srcArgnum = srcArgnum;
            return res;
        }

        enum {In = BinTranslation::Token::T_INPUT, Out = BinTranslation::Token::T_OUTPUT, IO = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT, InvIm = BinTranslation::Token::T_INVERT_IMM}; //TODO(ch): Use IO in table construction.
    }
}

#endif //__LOOPS_COMPOSER_HPP__
