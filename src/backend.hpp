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
#include <map>

namespace loops
{
class Backend;
struct SyntopTranslation
{
    struct ArgTranslation
    {
        enum {T_FIXED, T_FROMSOURCE, T_TRANSFORMTOSPILL, T_ERROROFUSAGE}; //TODO(ch): probably, it's needed some T_TRANSFORM(something like int map)
        ArgTranslation(const Arg& a_fixed);
        ArgTranslation(size_t a_src_arnum, uint64_t flags = 0);
        int tag;
        Arg fixed;
        size_t srcArgnum;
        uint64_t transitFlags;
    };
    int m_tarop;
    std::vector<ArgTranslation> m_argsList;
    SyntopTranslation(): m_tarop(-1) {} //TODO(ch): ensure, that -1 will always be an error.
    SyntopTranslation(int a_tarop, std::initializer_list<ArgTranslation> a_args);
    Syntop apply(const Syntop& a_source, const Backend* a_backend = nullptr) const;
    size_t targetArgNum(size_t a_srcnum) const;
    enum {ARG_NOT_USED = -1}; //TODO(ch) : Probably, it's better to replace it with NOIDX?
};

namespace SyntopTranslationConstruction
{
    inline SyntopTranslation SyT(int taropcode, std::initializer_list<SyntopTranslation::ArgTranslation> translations)
    {
        return SyntopTranslation(taropcode, translations);
    }

    //SAreg is for SyntopTranslation::ArgTranslation fixed reigster
    inline SyntopTranslation::ArgTranslation SAreg(IRegInternal idx, uint64_t flags = 0)
    {
        Arg resArg = argIReg(idx);
        resArg.flags = flags;
        return SyntopTranslation::ArgTranslation(resArg);
    }
    //SAimm is for SyntopTranslation::ArgTranslation fixed value
    inline SyntopTranslation::ArgTranslation SAimm(int64_t val, uint64_t flags = 0)
{
        Arg resArg = argIImm(val);
        resArg.flags = flags;
        return SyntopTranslation::ArgTranslation(resArg);
    }
    //SAcop is for SyntopTranslation::ArgTranslation to be copied from source
    inline SyntopTranslation::ArgTranslation SAcop(size_t argnum, uint64_t flags = 0) { return SyntopTranslation::ArgTranslation(argnum, flags); }
    //SAcopspl is for SyntopTranslation::ArgTranslation to be copied and transformed to spill.
    inline SyntopTranslation::ArgTranslation SAcopspl(size_t argnum)
    {
        SyntopTranslation::ArgTranslation res(argnum);
        res.tag = SyntopTranslation::ArgTranslation::T_TRANSFORMTOSPILL;
        return res; 
    }
};

class Backend
{
public:
    bool isImmediateFit(const Syntop& a_op, size_t argnum) const;
    virtual std::set<size_t> filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const;
    virtual size_t reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const;
    virtual size_t spillSpaceNeeded(const Syntop& a_op) const;

    //About getUsedRegistersIdxs and getUsedRegisters: registers will return if it corresponds to ALL conditions given through flag mask,
    //if one condtion is true, and other is false, it will not return register.
    //Next three functions return NUMBERS OF ARGUMENT, not an register numbers.
    virtual std::set<size_t> getUsedRegistersIdxs(const Syntop& a_op, uint64_t flagmask = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) const;
    std::set<size_t> getOutRegistersIdxs(const Syntop& a_op) const;
    std::set<size_t> getInRegistersIdxs(const Syntop& a_op) const;

    //Next three functions return register numbers.
    std::set<IRegInternal> getUsedRegisters(const Syntop& a_op, uint64_t flagmask = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) const;
    std::set<IRegInternal> getOutRegisters(const Syntop& a_op) const;
    std::set<IRegInternal> getInRegisters(const Syntop& a_op) const;

    virtual Syntfunc bytecode2Target(const Syntfunc& a_bcfunc) const; //TODO(ch): most part of this function must be implemeted here. Or it must be there fully.
    const FuncBodyBuf target2Hex(const Syntfunc& a_bcfunc) const;
    void* compile(Context* a_ctx, Func* a_func) const;
    
    //Prologue and epilogue support
    virtual size_t stackGrowthAlignment(size_t stackGrowth) const = 0;
    virtual size_t stackParamOffset(size_t a_nettoSpills, size_t a_snippetCausedSpills) const = 0;
    virtual Arg getSParg(Func* funcimpl) const = 0;

    virtual std::unordered_map<int, std::string> getOpStrings() const = 0;
    virtual Printer::ColPrinter colHexPrinter(const Syntfunc& toP) const = 0; //TODO(ch): I want to believe, that at some moment this function will become indpendent of toP. It's okay for current backend, but there is no confidence for intel or even vector expansions.
    virtual Printer::ArgPrinter argPrinter(const Syntfunc& toP) const = 0;

    Allocator* getAllocator() { return m_exeAlloc; }
    inline bool isLittleEndianInstructions() const { return m_isLittleEndianInstructions; }
    inline bool isLittleEndianOperands() const { return m_isLittleEndianOperands; }
    inline bool isMonowidthInstruction() const { return m_isMonowidthInstruction; }
    inline size_t instructionWidth() const { return m_instructionWidth; };
    virtual uint64_t parameterRegisters() const { return m_parameterRegisters; }
    virtual uint64_t returnRegisters() const { return m_returnRegisters; }
    virtual uint64_t callerSavedRegisters() const { return m_callerSavedRegisters; }
    virtual uint64_t calleeSavedRegisters() const { return m_calleeSavedRegisters; }
    inline std::string name() const { return m_name; };
    virtual void switchOnSpillStressMode() = 0;

    const std::vector<CompilerStagePtr>& getAfterRegAllocStages() const { return m_afterRegAllocStages; }
private:
    mutable size_t m_s2sCurrentOffset; //TODO(ch): Do something with thread-safety.
protected:
    Backend();
    size_t getS2sCurrentOffset() const {return m_s2sCurrentOffset;}
    virtual bool handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const = 0;

    BinTranslation(*m_s2blookup)(const Syntop&, bool&);
    SyntopTranslation(*m_s2slookup)(const Syntop&, bool&);
    inline BinTranslation lookS2b(const Syntop& index) const
    {
        bool NOTSUPPORTED;
        BinTranslation ret = m_s2blookup(index, NOTSUPPORTED);
        Assert(NOTSUPPORTED);
        return ret;
    }
    inline SyntopTranslation lookS2s(const Syntop& index) const
    {
        bool NOTSUPPORTED;
        SyntopTranslation ret = m_s2slookup(index, NOTSUPPORTED);
        Assert(NOTSUPPORTED);
        return ret;
    }

    Allocator* m_exeAlloc;
    bool m_isLittleEndianInstructions;
    bool m_isLittleEndianOperands;
    bool m_isMonowidthInstruction;
    size_t m_instructionWidth;
    size_t m_registersAmount;
    uint64_t m_parameterRegisters;
    uint64_t m_returnRegisters;
    uint64_t m_callerSavedRegisters;
    uint64_t m_calleeSavedRegisters;
    std::vector<CompilerStagePtr> m_afterRegAllocStages;
    std::string m_name;
};
};
#endif // __LOOPS_BACKEND_HPP__
