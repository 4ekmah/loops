/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_COMPOSER_HPP__
#define __LOOPS_COMPOSER_HPP__

#include "loops/loops.hpp"
#include "allocator.hpp"
#include "common.hpp"
#include <vector>
#include <unordered_map>

namespace loops
{
    class BackendImpl;
    typedef std::shared_ptr<std::vector<uint8_t> > FuncBodyBuf;
    class Bitwriter
    {
    public:
        Bitwriter(const BackendImpl* a_backend);
        void startInstruction();
        void writeDetail(uint64_t a_detail, size_t a_fieldwidth);
        void endInstruction();
        const FuncBodyBuf buffer() const {return m_buffer;}
        inline size_t bitAddress() { return (m_size << 3) + m_bitpos; }
    private:
        const BackendImpl* m_backend;
        FuncBodyBuf m_buffer;
        size_t m_size;
        size_t m_bitpos;
        size_t m_startsize;
        enum {NOTRANSACTION = -1, MINIMAL_BUFFER_SIZE = 512};
    };
    
//TODO(ch): Binatr collection is static data. This collection must be created at compile time. Currently, even if we will create this collection as global object, it will be initialized at runtime with unnecessary computational losts.
    struct Binatr //is for "binary translation"
    {
        struct Detail
        {
            //TODO(ch): Actually, it looks like, we need only adresses, statics, and common-use-arguments.
            enum {D_STATIC, D_REG, D_CONST, D_ADDRESS, D_OFFSET, D_STACKOFFSET};
            Detail(int tag, size_t fieldsize);
            Detail(int tag, uint64_t val, size_t fieldsize);
            int tag;
            size_t width; //in bits //TODO(ch): unsigned char?
            uint64_t field;
        };
        std::vector<Detail> m_compound;
        size_t m_size;
        Binatr(){}
        Binatr(std::initializer_list<Detail> lst);
        inline size_t size() const { return m_size; }
        void applyNAppend(const Syntop& op, Bitwriter* bits) const;
    };

    typedef std::unordered_map<int, ArgIndexedArray<Binatr> > M2bMap;//m2b is for "mnemonic to binary"
};

#endif //__LOOPS_COMPOSER_HPP__
