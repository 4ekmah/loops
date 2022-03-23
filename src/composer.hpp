/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_COMPOSER_HPP__
#define __LOOPS_COMPOSER_HPP__

#include "loops/loops.hpp"
#include "common.hpp"
#include <vector>
#include <unordered_map>

namespace loops
{
    class p_canvas;

//TODO(ch): It's obvious, there can be simple stage of just substitution all variables with CONSTs or REGs accordingly to given arguments. All D_SWITHCHERs will be replaced with D_OPTYPE, all D_VARIABLES will evolve to D_REG or D_CONST.
//TODO(ch): Snippets collection is static data. This collection must be created at compile time. Currently, even if we will create this collection as global object, it will be initialized at runtime with unnecessary computational losts.
    class i_snippet
    {
    public:
        struct detail
        {
            //TODO(ch): Actually, it looks like, we need only adresses, statics, switchers varibales, and common-use-arguments.
            enum {D_STATIC, D_SWITCHER, D_REG, D_CONST, D_ADDRESS, D_OFFSET, D_STACKOFFSET, D_VARIABLE };
            detail(int tag, size_t fieldsize);
            detail(int tag, uint64_t val, size_t fieldsize);
            detail(int tag, uint64_t val, uint64_t alterval, size_t corres_pos, size_t fieldsize);
            int m_tag;
            size_t m_fieldsize; //in bits //TODO(ch): unsigned char?
            uint64_t m_val, m_alterval; //TODO(ch): Rename it to "field". There never will be a values.
            // m_val means:
            // D_STATIC - static part of instruction(basically, identify instruction itself)
            // D_SWITCHER - bitfield, designating, that D_VARIABLE must be register. m_alterval is a bitfield, designating const.
            // D_VARIABLE - number of corresponding D_SWITCHER.
            // D_CONST, D_ADDRESS, D_OFFSET, D_STACKOFFSET, D_REG - must not be initialized.
        };
        i_snippet(){}
        i_snippet(std::initializer_list<detail> lst);
        inline size_t size() const { return m_size; }
        void apply_n_append(const std::vector<Arg>& args, p_canvas* canvas) const;
    private:
        std::vector<detail> m_compound;
        size_t m_size;
    };

    typedef std::unordered_map<int, i_snippet> instruction_set;
    class p_canvas
    {
        public:
        std::vector<uint8_t> m_buffer;
        std::vector<size_t> m_addresses;
        void fromsynt(const instruction_set& a_iset, const std::vector<syntop> a_syntlist);
    };
};

#endif //__LOOPS_COMPOSER_HPP__
