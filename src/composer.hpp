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
    class bitwriter
    {
    private:
//            backend* m_backend; //TODO(ch): to ask about little-endian.
        allocator* m_alloc;
        uint8_t* m_buffer; //TODO(ch): Who will free this buffer?
        size_t m_size;
        size_t m_maxsize;
        size_t m_bitpos;
        size_t m_startsize;
        enum {NOTRANSACTION = -1};
    public:
        bitwriter();
        void startinstruction();
        void writedetail(uint64_t a_detail, size_t a_fieldwidth);
        void endinstruction();
        const uint8_t* buffer() const {return m_buffer;}
        size_t size() const {return m_size;}
        inline size_t bitaddress() { return (m_size << 3) + m_bitpos; }
    };
    
//TODO(ch): Binatr collection is static data. This collection must be created at compile time. Currently, even if we will create this collection as global object, it will be initialized at runtime with unnecessary computational losts.
    class binatr //is for "binary translation"
    {
    public:
        struct detail
        {
            //TODO(ch): Actually, it looks like, we need only adresses, statics, and common-use-arguments.
            enum {D_STATIC, D_REG, D_CONST, D_ADDRESS, D_OFFSET, D_STACKOFFSET};
            detail(int tag, size_t fieldsize);
            detail(int tag, uint64_t val, size_t fieldsize);
            int m_tag;
            size_t m_fieldwidth; //in bits //TODO(ch): unsigned char?
            uint64_t m_field;
        };
        binatr(){}
        binatr(std::initializer_list<detail> lst);
        inline size_t size() const { return m_size; }
        void apply_n_append(const std::vector<Arg>& args, bitwriter* bits) const;
        bool is_const_fit(const syntop& a_op, size_t argnum) const;
    private:
        std::vector<detail> m_compound;
        size_t m_size;
    };

    typedef std::unordered_map<int, arg_indexed_array<binatr> > m2b_map;//m2b is for "mnemonic to binary"
    class p_canvas //TODO(ch): p_canvas isn't needed anymore.
    {
        public:
        std::vector<uint8_t> m_buffer;
        std::vector<size_t> m_addresses;
        void fromsynt(const m2b_map& a_iset, const std::vector<syntop> a_syntlist);
    };
};

#endif //__LOOPS_COMPOSER_HPP__
