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
//TODO(ch): BinTranslation collection is static data. This collection must be created at compile time.
//Currently, even if we will create this collection as global object, it will be initialized at runtime with unnecessary computational losts.
//
//There is similar comment for Sytnop translation: we need more flexible set of transformations up to custom functions or lambdas, which process input arguments to 
//bitset. Currently works some simple workaround: BTomm(T_OMIT) can switch off argument(mostly immediates) from argument audit in binary composition, but provide some flags
//on requests are coming from previous stages, simultaneously this argument is processed by usual code inside of BiT selector(e.g a64BTLookup for aarch64) and transformed
//to one or more constants, accordingly to encoding and provided to BiT as BTsta. So, both sides of argument interface works. There is simple idea to add some BTrpl, which
//will combine properties of BTsta and BTomm and will replace argument with it hardcoded in changed form version. The most flexible solution is kind of processing lambdas.

    struct BinTranslation
    {
        struct Token
        {
            //TODO(ch): Actually, it looks like, we need only adresses, statics, and common-use-arguments.
            enum {T_STATIC, T_REG, T_IMMEDIATE, T_SPILLED, T_OMIT};
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

        inline BinTranslation::Token BTomm(int srcArgnum, uint64_t flags = 0)
        {
            BinTranslation::Token res(BinTranslation::Token::T_OMIT, 0);
            res.srcArgnum = srcArgnum;
            res.fieldOflags = flags;
            return res; 
        }

        inline BinTranslation::Token BTreg(int srcArgnum, int width, uint64_t regflag = AF_INPUT | AF_OUTPUT)
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

        enum
        {
              In     = AF_INPUT, 
              Out    = AF_OUTPUT, 
              IO     = AF_INPUT | AF_OUTPUT,
              Lab    = AF_PRINTOFFSET,
              Addr8  = AF_ADDRESS8,
              Addr16 = AF_ADDRESS16,
              Addr32 = AF_ADDRESS32,
              Addr64 = AF_ADDRESS64,
              Eff64  = AF_EFFECTIVE64,
              Cond   = AF_CONDITION,
              Unsgn  = AF_UNSIGNED,
              LanInd = AF_LANEINDEX,
              VecRng = AF_VREGRANGE,
              NoTp   = AF_NOTYPE, 
              HlfLan = AF_HALFLANES,
              Rdc    = AF_REDUCED,
        }; //TODO(ch): Use IO in table construction.
    }
}

#endif //__LOOPS_COMPOSER_HPP__
