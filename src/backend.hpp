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
class BackendImpl;
struct Mnemotr //is for "mnemonic translation"
{
    struct Argutr
    {
        enum {T_FIXED, T_FROMSOURCE, T_ERROROFUSAGE}; //TODO(ch): probably, it's needed some T_TRANSFORM(something like int map)
        Argutr(const Arg& a_fixed);
        Argutr(size_t a_src_arnum);
        int tag;
        Arg fixed;
        size_t srcArgnum;
    };
    int m_tarop;
    std::vector<Argutr> m_argsList;
    Mnemotr(): m_tarop(-1) {} //TODO(ch): ensure, that -1 will always be an error.
    Mnemotr(int a_tarop, std::initializer_list<Argutr> a_args);
    Syntop apply(const Syntop& a_source, const BackendImpl* a_bcknd = nullptr) const;
    size_t targetArgNum(size_t a_srcnum) const;
    enum {ARG_NOT_USED = -1};
};

typedef std::unordered_map<int, ArgIndexedArray<Mnemotr> > M2mMap;//m2m is for "mnemonic to mnemonic"

class BackendImpl : public Backend
{
public:
    size_t m_refcount;
    bool isConstFit(const Syntop& a_op, size_t argnum) const;
    virtual Syntfunc bytecode2Target(const Syntfunc& a_bcfunc) const; //TODO(ch): most part of this function must be implemeted here. Or it must be there fully.
    const FuncBodyBuf target2Hex(const Syntfunc& a_bcfunc) const;
    void* compile(Context* a_ctx, Func* a_func) const;
    virtual Arg translateReg(IRegInternal tofind) const = 0; //Don't use it directly. it's for bytecode2target //TODO(ch): there must version of function, for register, that are already defined. In case, if they are not registered in translation map, program must fail.
    virtual std::unordered_map<int, std::string> getOpStrings() const = 0;
    inline bool isLittleEndianInstructions() const {return m_isLittleEndianInstructions;}
    inline bool isMonowidthInstruction() const {return m_isMonowidthInstruction;}
    inline size_t instructionWidth () const {return m_instructionWidth;} ;
private:
    mutable size_t m_m2mCurrentOffset; //TODO(ch): Do something with thread-safety.
protected:
    BackendImpl() : Backend(), m_refcount(0) {}
    size_t getM2mCurrentOffset() const {return m_m2mCurrentOffset;}
    virtual bool handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const = 0;
    M2mMap m_2tararch;
    M2bMap m_2binary;
    Allocator* m_exeAlloc;
    bool m_isLittleEndianInstructions;
    bool m_isMonowidthInstruction;
    size_t m_instructionWidth;
};

inline BackendImpl* getImpl(Backend* wrapper)
{
    if (!wrapper)
        throw std::string("Null context pointer.");
    return static_cast<BackendImpl*>(_getImpl(wrapper));
}

};
#endif // __LOOPS_BACKEND_HPP__
