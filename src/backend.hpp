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
#include "printer.hpp"
#include <vector>
#include <set>

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

typedef SyntopIndexedArray<Mnemotr> M2mMap;//m2m is for "mnemonic to mnemonic"

namespace MnemotrTableConstructor
{
    using BackendTableConstructor::SFsiz;
    using BackendTableConstructor::SFtyp;
    using BackendTableConstructor::SFval;

    inline BackendTableConstructor::SyntopTreeTempBranch<Mnemotr> Sb(int cval, const typename SyntopIndexedArray<Mnemotr>::ArgIndA& val)
    {
        using namespace BackendTableConstructor;
        return _Sb<Mnemotr>(cval, val);
    }

    inline SyntopIndexedArray<Mnemotr>::ArgIndA Sl(int taropcode, std::initializer_list<Mnemotr::Argutr> translations)
    {
        return BackendTableConstructor::Sl(Mnemotr(taropcode, translations));
    }

    //MAreg is for Mnemotr::Argutr fixed reigster
    inline Mnemotr::Argutr MAreg(IRegInternal idx) { return Mnemotr::Argutr(argIReg(idx)); }
    //MAcon is for Mnemotr::Argutr fixed value
    inline Mnemotr::Argutr MAcon(int64_t val) { return Mnemotr::Argutr(argIConst(val)); }
    //MAcop is for Mnemotr::Argutr to be copied from source
    inline Mnemotr::Argutr MAcop(size_t argnum) { return Mnemotr::Argutr(argnum); }
};

class BackendImpl : public Backend
{
public:
    size_t m_refcount;
    bool isConstFit(const Syntop& a_op, size_t argnum) const;

    //About getUsedRegistersIdxs and getUsedRegisters: registers will return if it corresponds to ALL conditions given through flag mask,
    //if one condtion is true, and other is false, it will not return register.
    //Next three functions return NUMBERS OF ARGUMENT, not an register numbers.
    std::set<size_t> getUsedRegistersIdxs(const Syntop& a_op, uint64_t flagmask = Binatr::Detail::D_INPUT | Binatr::Detail::D_OUTPUT) const;
    std::set<size_t> getOutRegistersIdxs(const Syntop& a_op) const;
    std::set<size_t> getInRegistersIdxs(const Syntop& a_op) const;

    //Next three functions return register numbers.
    std::set<IRegInternal> getUsedRegisters(const Syntop& a_op, uint64_t flagmask = Binatr::Detail::D_INPUT | Binatr::Detail::D_OUTPUT) const;
    std::set<IRegInternal> getOutRegisters(const Syntop& a_op) const;
    std::set<IRegInternal> getInRegisters(const Syntop& a_op) const;

    virtual Syntfunc bytecode2Target(const Syntfunc& a_bcfunc) const; //TODO(ch): most part of this function must be implemeted here. Or it must be there fully.
    const FuncBodyBuf target2Hex(const Syntfunc& a_bcfunc) const;
    void* compile(Context* a_ctx, Func* a_func) const;
    virtual void writePrologue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regUsed, size_t a_regSpilled) const = 0;
    virtual void writeEpilogue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regUsed, size_t a_regSpilled) const = 0;

    virtual std::unordered_map<int, std::string> getOpStrings() const = 0;
    virtual Printer::ColPrinter colHexPrinter(const Syntfunc& toP) const = 0; //TODO(ch): I want to believe, that at some moment this function will become indpendent of toP. It's okay for current backend, but there is no confidence for intel or even vector expansions.
    virtual Printer::ArgPrinter argPrinter() const = 0;
    OpPrintInfo getPrintInfo(const Syntop& op);

    Allocator* getAllocator() {return m_exeAlloc;}
    inline bool isLittleEndianInstructions() const {return m_isLittleEndianInstructions;}
    inline bool isMonowidthInstruction() const {return m_isMonowidthInstruction;}
    inline size_t instructionWidth () const {return m_instructionWidth;} ;
    size_t registersAmount() const { return m_registersAmount; }
    inline std::string name() const {return m_name;} ;
    virtual Arg translateReg(IRegInternal tofind) const = 0; //Don't use it directly. it's for bytecode2target
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
    size_t m_registersAmount;
    std::string m_name;
};

inline BackendImpl* getImpl(Backend* wrapper)
{
    if (!wrapper)
        throw std::string("Null context pointer.");
    return static_cast<BackendImpl*>(_getImpl(wrapper));
}
inline const BackendImpl* getImpl(const Backend* wrapper)
{
    if(!wrapper)
        throw std::string("Null context pointer.");
    return static_cast<BackendImpl*>(_getImpl(const_cast<Backend*>(wrapper)));
}


};
#endif // __LOOPS_BACKEND_HPP__
