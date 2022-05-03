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
class Backend;
struct Mnemotr //is for "mnemonic translation"
{
    struct Argutr
    {
        enum {T_FIXED, T_FROMSOURCE, T_TRANSFORMTOSPILL, T_ERROROFUSAGE}; //TODO(ch): probably, it's needed some T_TRANSFORM(something like int map)
        Argutr(const Arg& a_fixed);
        Argutr(size_t a_src_arnum, uint64_t flags = 0);
        int tag;
        Arg fixed;
        size_t srcArgnum;
        uint64_t transitFlags;
    };
    int m_tarop;
    std::vector<Argutr> m_argsList;
    Mnemotr(): m_tarop(-1) {} //TODO(ch): ensure, that -1 will always be an error.
    Mnemotr(int a_tarop, std::initializer_list<Argutr> a_args);
    Syntop apply(const Syntop& a_source, const Backend* a_bcknd = nullptr) const;
    size_t targetArgNum(size_t a_srcnum) const;
    enum {ARG_NOT_USED = -1}; //TODO(ch) : Probably, it's better to replace it with NOIDX?
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
    inline Mnemotr::Argutr MAcop(size_t argnum, uint64_t flags = 0) { return Mnemotr::Argutr(argnum, flags); }
    //MAcop is for Mnemotr::Argutr to be copied and transformed to spill.
    inline Mnemotr::Argutr MAcopspl(size_t argnum)
    {
        Mnemotr::Argutr res(argnum);
        res.tag = Mnemotr::Argutr::T_TRANSFORMTOSPILL;
        return res; 
    }
};

class Backend
{
public:
    bool isConstFit(const Syntop& a_op, size_t argnum) const;
    std::set<size_t> filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const;
    virtual size_t reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const;
    virtual size_t spillSpaceNeeded(const Syntop& a_op) const;

    //About getUsedRegistersIdxs and getUsedRegisters: registers will return if it corresponds to ALL conditions given through flag mask,
    //if one condtion is true, and other is false, it will not return register.
    //Next three functions return NUMBERS OF ARGUMENT, not an register numbers.
    virtual std::set<size_t> getUsedRegistersIdxs(const Syntop& a_op, uint64_t flagmask = Binatr::Detail::D_INPUT | Binatr::Detail::D_OUTPUT) const;
    std::set<size_t> getOutRegistersIdxs(const Syntop& a_op) const;
    std::set<size_t> getInRegistersIdxs(const Syntop& a_op) const;

    //Next three functions return register numbers.
    std::set<IRegInternal> getUsedRegisters(const Syntop& a_op, uint64_t flagmask = Binatr::Detail::D_INPUT | Binatr::Detail::D_OUTPUT) const;
    std::set<IRegInternal> getOutRegisters(const Syntop& a_op) const;
    std::set<IRegInternal> getInRegisters(const Syntop& a_op) const;

    virtual Syntfunc bytecode2Target(const Syntfunc& a_bcfunc) const; //TODO(ch): most part of this function must be implemeted here. Or it must be there fully.
    const FuncBodyBuf target2Hex(const Syntfunc& a_bcfunc) const;
    void* compile(Context* a_ctx, Func* a_func) const;
    virtual void writePrologue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regSpilled, const std::set<IRegInternal>& a_calleeSaved, const std::vector<IRegInternal>& a_paramsInStack) const = 0;
    virtual void writeEpilogue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regSpilled, const std::set<IRegInternal>& a_calleeSaved) const = 0;

    virtual std::unordered_map<int, std::string> getOpStrings() const = 0;
    virtual Printer::ColPrinter colHexPrinter(const Syntfunc& toP) const = 0; //TODO(ch): I want to believe, that at some moment this function will become indpendent of toP. It's okay for current backend, but there is no confidence for intel or even vector expansions.
    virtual Printer::ArgPrinter argPrinter(const Syntfunc& toP) const = 0;
    OpPrintInfo getPrintInfo(const Syntop& op);

    Allocator* getAllocator() { return m_exeAlloc; }
    inline bool isLittleEndianInstructions() const { return m_isLittleEndianInstructions; }
    inline bool isLittleEndianOperands() const { return m_isLittleEndianOperands; }
    inline bool isMonowidthInstruction() const { return m_isMonowidthInstruction; }
    inline size_t instructionWidth() const { return m_instructionWidth; };
    size_t registersAmount() const;
    virtual const std::vector<IRegInternal>& parameterRegisters() const { return m_parameterRegisters; }
    virtual const std::vector<IRegInternal>& returnRegisters() const { return m_returnRegisters; }
    virtual const std::vector<IRegInternal>& callerSavedRegisters() const { return m_callerSavedRegisters; }
    virtual const std::vector<IRegInternal>& calleeSavedRegisters() const { return m_calleeSavedRegisters; }
    inline std::string name() const { return m_name; };

    const std::vector<CompilerStagePtr>& getAfterRegAllocStages() const { return m_afterRegAllocStages; }
private:
    mutable size_t m_m2mCurrentOffset; //TODO(ch): Do something with thread-safety.
protected:
    Backend();
    size_t getM2mCurrentOffset() const {return m_m2mCurrentOffset;}
    virtual bool handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const = 0;
    M2mMap m_2tararch;
    M2bMap m_2binary;
    Allocator* m_exeAlloc;
    bool m_isLittleEndianInstructions;
    bool m_isLittleEndianOperands;
    bool m_isMonowidthInstruction;
    size_t m_instructionWidth;
    size_t m_registersAmount;
    std::vector<IRegInternal> m_parameterRegisters;
    std::vector<IRegInternal> m_returnRegisters;
    std::vector<IRegInternal> m_callerSavedRegisters;
    std::vector<IRegInternal> m_calleeSavedRegisters;
    std::vector<CompilerStagePtr> m_afterRegAllocStages;
    std::string m_name;
};
};
#endif // __LOOPS_BACKEND_HPP__
