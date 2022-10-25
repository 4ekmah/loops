/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
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
        enum {T_FIXED, T_FROMSOURCE, T_TRANSFORMTOSPILL, T_COPYSHIFTRIGHT, T_ERROROFUSAGE};
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
    inline SyntopTranslation::ArgTranslation SAreg(RegIdx idx, uint64_t flags = 0)
    {
        Arg resArg = argReg(RB_INT, idx);
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
    //SAcopshr is for copy original immediate argument and divide it by 2 <shft> times.
    //Used for immediate offsets values on Arm.
    inline SyntopTranslation::ArgTranslation SAcopsar(size_t argnum, int64_t shft, uint64_t flags = 0)
    {
        SyntopTranslation::ArgTranslation res(argnum);
        res.fixed.value = shft;
        res.tag = SyntopTranslation::ArgTranslation::T_COPYSHIFTRIGHT;
        res.transitFlags = flags;
        return res;
    }
};


class Backend
{
public:
    bool isImmediateFit(const Syntop& a_op, size_t argnum) const;
    virtual std::set<size_t> filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const;
    virtual size_t reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const;
    virtual size_t spillSpaceNeeded(const Syntop& a_op, int basketNum) const;

    //About getUsedRegistersIdxs and getUsedRegisters: registers will return if it corresponds to ALL conditions given through flag mask,
    //if one condtion is true, and other is false, it will not return register.
    //Next three functions return NUMBERS OF ARGUMENT, not an register numbers.
    virtual std::set<size_t> getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) const;
    std::set<size_t> getOutRegistersIdxs(const Syntop& a_op, int basketNum) const;
    std::set<size_t> getInRegistersIdxs(const Syntop& a_op, int basketNum) const;

    //Next three functions return register numbers.
    std::set<RegIdx> getUsedRegisters(const Syntop& a_op, int basketNum, uint64_t flagmask = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) const;
    std::set<RegIdx> getOutRegisters(const Syntop& a_op, int basketNum) const;
    std::set<RegIdx> getInRegisters(const Syntop& a_op, int basketNum) const;

    virtual Syntfunc bytecode2Target(const Syntfunc& a_bcfunc) const; //TODO(ch): most part of this function must be implemeted here. Or it must be there fully.
    const FuncBodyBuf target2Hex(const Syntfunc& a_bcfunc) const;
    void* compile(Context* a_ctx, Func* a_func);
    
    //Prologue and epilogue support
    /*
    getStackParameterLayout
    Return offset-of-SP map of stack-passed parameters: parLayout[basketNum][idx used in a_func.params] = offset
    It's assumed offset from SP just after function call(without prologue).
    Offset measured not in bytes, each unit = 8 bytes. 
    */
    virtual void getStackParameterLayout(const Syntfunc& a_func, const std::vector<size_t> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, size_t> (&parLayout)[RB_AMOUNT]) const = 0;
    virtual size_t stackGrowthAlignment(size_t stackGrowth) const = 0;
    virtual Arg getSParg(Func* funcimpl) const = 0;

    virtual std::unordered_map<int, std::string> getOpStrings() const = 0;
    virtual Printer::ColPrinter colHexPrinter(const Syntfunc& toP) const = 0; //TODO(ch): I want to believe, that at some moment this function will become indpendent of toP. It's okay for current backend, but there is no confidence for intel or even vector expansions.
    virtual Printer::ArgPrinter argPrinter(const Syntfunc& toP) const = 0;
    
    Allocator* getAllocator() { return &m_exeAlloc; }
    inline std::vector<int> getStackBasketOrder() const { return {RB_VEC, RB_INT};}
    inline int getVectorRegisterBits() const { return m_vectorRegisterBits; }
    template<typename _Tp> inline size_t vlanes() const { return (m_vectorRegisterBits >> 3)  / sizeof(_Tp); }
    inline size_t vlanes(int elemtype) const { return (m_vectorRegisterBits >> 3)  / elemSize(elemtype); }
    inline bool isLittleEndianInstructions() const { return m_isLittleEndianInstructions; }
    inline bool isLittleEndianOperands() const { return m_isLittleEndianOperands; }
    inline bool isMonowidthInstruction() const { return m_isMonowidthInstruction; }
    inline size_t instructionWidth() const { return m_instructionWidth; };
    virtual std::vector<size_t> parameterRegisters(int basketNum) const { return m_parameterRegisters[basketNum]; }
    virtual std::vector<size_t> returnRegisters(int basketNum) const { return m_returnRegisters[basketNum]; }
    virtual std::vector<size_t> callerSavedRegisters(int basketNum) const { return m_callerSavedRegisters[basketNum]; }
    virtual std::vector<size_t> calleeSavedRegisters(int basketNum) const { return m_calleeSavedRegisters[basketNum]; }
    inline std::string name() const { return m_name; };
    virtual void switchOnSpillStressMode() = 0;

    const std::vector<CompilerStagePtr>& getAfterRegAllocStages() const { return m_afterRegAllocStages; }
    const std::vector<CompilerStagePtr>& getBeforeRegAllocStages() const { return m_beforeRegAllocStages; }
private:
    mutable size_t m_s2sCurrentOffset; //TODO(ch): Do something with thread-safety.
protected:
    Backend();
    size_t getS2sCurrentOffset() const {return m_s2sCurrentOffset;}
    virtual bool handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const = 0;

    BinTranslation(*m_s2blookup)(const Syntop&, bool&);
    SyntopTranslation(*m_s2slookup)(const Backend*, const Syntop&, bool&);
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
        SyntopTranslation ret = m_s2slookup(this, index, NOTSUPPORTED);
        Assert(NOTSUPPORTED);
        return ret;
    }

    Allocator m_exeAlloc;
    int m_vectorRegisterBits;
    bool m_isLittleEndianInstructions;
    bool m_isLittleEndianOperands;
    bool m_isMonowidthInstruction;
    size_t m_instructionWidth;
    size_t m_registersAmount;
    std::vector<size_t> m_parameterRegisters[RB_AMOUNT];
    std::vector<size_t> m_returnRegisters[RB_AMOUNT];
    std::vector<size_t> m_callerSavedRegisters[RB_AMOUNT];
    std::vector<size_t> m_calleeSavedRegisters[RB_AMOUNT];
    std::vector<CompilerStagePtr> m_afterRegAllocStages;
    std::vector<CompilerStagePtr> m_beforeRegAllocStages;
    std::string m_name;
};
};
#endif // __LOOPS_BACKEND_HPP__
