/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_BACKEND_HPP__
#define __LOOPS_BACKEND_HPP__

#include "loops/loops.hpp"
#include "common.hpp"
#include "composer.hpp"
#include <vector>

namespace loops
{
class backend_impl;
struct mnemotr //is for "mnemonic translation"
{
    struct argutr
    {
        enum {T_FIXED, T_FROMSOURCE, T_ERROROFUSAGE}; //TODO(ch): probably, it's needed some T_TRANSFORM(something like int map)
        argutr(const Arg& a_fixed);
        argutr(size_t a_src_arnum);
        int m_tag;
        Arg m_fixed;
        size_t m_src_arnum;
    };
    int m_tarop;
    std::vector<argutr> m_argslist;
    mnemotr(): m_tarop(-1) {} //TODO(ch): ensure, that -1 will always be an error.
    mnemotr(int a_tarop, std::initializer_list<argutr> a_args);
    syntop apply(const syntop& a_source, const backend_impl* a_bcknd = nullptr) const;//TODO(ch): backend*?  Will we have to ask somebody about target register numbers?
    size_t target_arg_num(size_t a_srcnum) const;
    enum {ARG_NOT_USED = -1};
};

typedef std::unordered_map<int, arg_indexed_array<mnemotr> > m2m_map;//m2m is for "mnemonic to mnemonic"

class backend_impl : public Backend
{
private:
    mutable size_t m_m2m_currentoffset; //TODO(ch): Do something with thread-safety.
protected:
    m2m_map m_2tararch;
    m2b_map m_2binary;
    backend_impl() : Backend(), refcount(0) {}
    size_t get_m2m_currentoffset() const {return m_m2m_currentoffset;}
    virtual bool handle_bytecodeop(const syntop& a_btop, syntfunc& a_formingtarget) const = 0;
public:
    size_t refcount;
    bool is_const_fit(const syntop& a_op, size_t argnum) const;
    virtual syntfunc bytecode2target(const syntfunc& a_bcfunc) const; //TODO(ch): most part of this function must be implemeted here. Or it must be there fully.
    
    virtual Arg translate_reg(IRegInternal tofind) const = 0; //Don't use it directly. it's for bytecode2target //TODO(ch): there must version of function, for register, that are already defined. In case, if they are not registered in translation map, program must fail.
    virtual std::unordered_map<int, std::string> get_op_strings() const = 0;
};

inline backend_impl* getImpl(Backend* wrapper)
{
    if (!wrapper)
        throw std::string("Null context pointer.");
    return static_cast<backend_impl*>(_getImpl(wrapper));
}

};
#endif // __LOOPS_BACKEND_HPP__
