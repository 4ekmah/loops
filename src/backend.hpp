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
    //TODO: Looks like we need here some general transformation rule format, providing ability to change tag, value, elemtype or other. Probably, ability to set transformation function pointer.
    struct ArgTranslation
    {
        enum {T_FIXED, T_FROMSOURCE, T_TRANSFORMTOSPILL, T_COPYSHIFTRIGHT, T_SETELEMTYPE, T_ERROROFUSAGE};
        ArgTranslation(const Arg& a_fixed);
        ArgTranslation(int a_srcArgnum, uint64_t flags = 0);
        ArgTranslation(int a_srcArgnum, int a_elemtype, uint64_t flags);
        int tag;
        Arg fixed;
        int srcArgnum;
        int elemtype;
        uint64_t transitFlags;
    };
    int m_tarop;
    std::vector<ArgTranslation> m_argsList;
    SyntopTranslation(): m_tarop(-1) {} //TODO(ch): ensure, that -1 will always be an error.
    SyntopTranslation(int a_tarop, std::initializer_list<ArgTranslation> a_args);
    Syntop apply(const Syntop& a_source, const Backend* a_backend = nullptr) const;
    int targetArgNum(int a_srcnum) const;
    enum { ARG_NOT_USED = UNDEFINED_ARGUMENT_NUMBER };
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
    //SAreg is for SyntopTranslation::ArgTranslation fixed reigster
    inline SyntopTranslation::ArgTranslation SAvreg(RegIdx idx, int elemtype, uint64_t flags = 0)
    {
        Arg resArg = argReg(RB_VEC, idx);
        resArg.elemtype = elemtype;
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
    inline SyntopTranslation::ArgTranslation SAcop(int argnum, uint64_t flags = 0) { return SyntopTranslation::ArgTranslation(argnum, flags); }
    //SAcopspl is for SyntopTranslation::ArgTranslation to be copied and transformed to spill.
    inline SyntopTranslation::ArgTranslation SAcopspl(int argnum)
    {
        SyntopTranslation::ArgTranslation res(argnum);
        res.tag = SyntopTranslation::ArgTranslation::T_TRANSFORMTOSPILL;
        return res; 
    }
    //SAcopshr is for copy original immediate argument and divide it by 2 <shft> times.
    //Used for immediate offsets values on Arm.
    inline SyntopTranslation::ArgTranslation SAcopsar(int argnum, int64_t shft, uint64_t flags = 0)
    {
        SyntopTranslation::ArgTranslation res(argnum);
        res.fixed.value = shft;
        res.tag = SyntopTranslation::ArgTranslation::T_COPYSHIFTRIGHT;
        res.transitFlags = flags;
        return res;
    }
    //SAcopelt is for SyntopTranslation::ArgTranslation to be copied from source with changing elemtype
    inline SyntopTranslation::ArgTranslation SAcopelt(int argnum, int elemtype, uint64_t flags = 0) { return SyntopTranslation::ArgTranslation(argnum, elemtype, flags); }
}

class Backend
{
public:
    virtual ~Backend();
    bool isImmediateFit(const Syntop& a_op, int argnum) const;
    virtual std::set<int> filterStackPlaceable(const Syntop& a_op, const std::set<int>& toFilter) const;
    virtual int reusingPreferences(const Syntop& a_op, const std::set<int>& undefinedArgNums) const;
    virtual int spillSpaceNeeded(const Syntop& a_op, int basketNum) const;

    //About getUsedRegistersIdxs and getUsedRegisters: registers will return if it corresponds to ALL conditions given through flag mask,
    //if one condtion is true, and other is false, it will not return register.
    //Next three functions return NUMBERS OF ARGUMENT, not an register numbers.
    virtual std::set<int> getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask = AF_INPUT | AF_OUTPUT) const;
    std::set<int> getOutRegistersIdxs(const Syntop& a_op, int basketNum) const;
    std::set<int> getInRegistersIdxs(const Syntop& a_op, int basketNum) const;

    //Next three functions return register numbers.
    std::set<RegIdx> getUsedRegisters(const Syntop& a_op, int basketNum, uint64_t flagmask = AF_INPUT | AF_OUTPUT) const;
    std::set<RegIdx> getOutRegisters(const Syntop& a_op, int basketNum) const;
    std::set<RegIdx> getInRegisters(const Syntop& a_op, int basketNum) const;

    //It's assumed here, that Syntop is native, not IR. Return argument flags.
    //Result array have to be allocated before and have to be of (Syntop::SYNTOP_ARGS_MAX) size.
    void fill_native_operand_flags(const loops::Syntop* a_op, uint64_t* result) const;

    //Prologue and epilogue support
    /*
    getStackParameterLayout
    Return offset-of-SP map of stack-passed parameters: parLayout[basketNum][idx used in a_func.params] = offset
    It's assumed offset from SP just after function call(without prologue).
    Offset measured not in bytes, each unit = 8 bytes. 
    */
    virtual void getStackParameterLayout(const Syntfunc& a_func, const std::vector<int> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, int> (&parLayout)[RB_AMOUNT]) const = 0;
    virtual int stackGrowthAlignment(int stackGrowth) const = 0;
    virtual void writeCallerPrologue(Syntfunc& prog, int stackGrowth) const = 0;
    virtual void writeCallerEpilogue(Syntfunc& prog, int stackGrowth) const = 0;

    virtual Arg getSParg() const = 0;

    virtual column_printer get_opname_printer() const = 0;
    virtual column_printer get_opargs_printer() const = 0;
    virtual column_printer get_hex_printer() const = 0;
    
    Allocator* getAllocator() { return &m_exeAlloc; }
    inline std::vector<int> getStackBasketOrder() const { return {RB_VEC, RB_INT};}
    inline int getVectorRegisterBits() const { return m_vectorRegisterBits; }
    template<typename _Tp> inline int vlanes() const { return (m_vectorRegisterBits >> 3)  / sizeof(_Tp); }
    inline int vlanes(int elemtype) const { return (m_vectorRegisterBits >> 3) / elem_size(elemtype); }
    inline bool isLittleEndianInstructions() const { return m_isLittleEndianInstructions; }
    inline bool isLittleEndianOperands() const { return m_isLittleEndianOperands; }
    inline bool isMonowidthInstruction() const { return m_isMonowidthInstruction; }
    inline int instructionWidth() const { return m_instructionWidth; }
    inline int callerStackIncrement() const { return m_callerStackIncrement; }
    //In some architectures jumps are measured from start of jump instruction(Arm), on other
    //from first byte after end of instruction.
    inline bool postInstructionOffset() const { return m_postInstructionOffset; }
    virtual std::vector<int> parameterRegisters(int basketNum) const { return m_parameterRegisters[basketNum]; }
    virtual std::vector<int> returnRegisters(int basketNum) const { return m_returnRegisters[basketNum]; }
    virtual std::vector<int> callerSavedRegisters(int basketNum) const { return m_callerSavedRegisters[basketNum]; }
    virtual std::vector<int> calleeSavedRegisters(int basketNum) const { return m_calleeSavedRegisters[basketNum]; }
    inline std::string name() const { return m_name; };

    const std::vector<CompilerPassPtr>& getAfterRegAllocPasses() const { return m_afterRegAllocPasses; }
    const std::vector<CompilerPassPtr>& getBeforeRegAllocPasses() const { return m_beforeRegAllocPasses; }

    inline BinTranslation lookS2b(const Syntop& index) const;
    inline SyntopTranslation lookS2s(const Syntop& index) const;
protected:
    Backend();

    BinTranslation(*m_s2blookup)(const Syntop&, bool&);
    SyntopTranslation(*m_s2slookup)(const Backend*, const Syntop&, bool&);

    Allocator m_exeAlloc;
    int m_vectorRegisterBits;
    bool m_isLittleEndianInstructions;
    bool m_isLittleEndianOperands;
    bool m_isMonowidthInstruction;
    bool m_postInstructionOffset;
    int m_instructionWidth;
    int m_callerStackIncrement;
    size_t m_registersAmount;
    std::vector<int> m_parameterRegisters[RB_AMOUNT];
    std::vector<int> m_returnRegisters[RB_AMOUNT];
    std::vector<int> m_callerSavedRegisters[RB_AMOUNT];
    std::vector<int> m_calleeSavedRegisters[RB_AMOUNT];
    std::vector<CompilerPassPtr> m_afterRegAllocPasses;
    std::vector<CompilerPassPtr> m_beforeRegAllocPasses;
    std::string m_name;
};

BinTranslation Backend::lookS2b(const Syntop& index) const
{
    //TODO(ch): Unfortunately, Intel64 printer calls lookS2b, so, because of recursion we cannot print instruction here. Fix it.
    bool NOTSUPPORTED;
    BinTranslation ret = m_s2blookup(index, NOTSUPPORTED);
    Assert(NOTSUPPORTED);
    return ret;
}
SyntopTranslation Backend::lookS2s(const Syntop& index) const
{
    bool success;
    SyntopTranslation ret = m_s2slookup(this, index, success);
    if(!success) 
        throw std::runtime_error(std::string("Unsupported intermediate representation instruction: ") + IR_instruction2string(index));
    return ret;
}

}
#endif // __LOOPS_BACKEND_HPP__
