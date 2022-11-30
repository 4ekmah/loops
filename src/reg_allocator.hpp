/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_REG_ALLOCATOR_HPP__
#define __LOOPS_REG_ALLOCATOR_HPP__

#include "loops/loops.hpp"
#include "backend.hpp"
#include "common.hpp"
#include <map>

namespace loops {
/*
TODO(ch): Implement with RISC-V RVV
For RVV support Register pool must be modified to support connected vectors(fractions and splices):
1.) There will be hierarchy of nested vectors: biggest vector can be separated into 1/2, 1/4, 1/8,
1/16, 1/32, 1/64. Connected vectors are allocated in one space and simulteneously: container one \
and nested one.
2.) Since vectors can have different sizes, allocation of nested vector blocks container allocation,
excepts connected vectors.
3.) And vice versa, container vector can be released only after releasing all components.
 */
    class RegisterPool
    {
    public:
        RegisterPool(Backend* m_owner);

        void initRegisterPool();
        size_t freeRegsAmount(int basketNum) const;
        inline bool havefreeRegs(int basketNum) const { return freeRegsAmount(basketNum) > 0; }
        RegIdx provideParamFromPool(int basketNum);  //Must be called first.
        // (There must be provided first registers from parameter vessel, further: return - callerSaved - calleeSaved).
        RegIdx provideRegFromPool(int basketNum, RegIdx a_hint = IReg::NOIDX);
        std::vector<RegIdx> provideConsecutiveRegs(int basketNum, int amount);
        RegIdx provideReturnFromPool(int basketNum); //Must be called last.
        void releaseReg(int basketNum, RegIdx freeReg);

        RegIdx provideSpillPlaceholder(int basketNum);
        void clearSpillPlaceholders(int basketNum);

        inline std::set<RegIdx> usedCallee(int basketNum) const { return m_usedCallee[basketNum]; }
        void overrideRegisterSet(int basketNum, const std::vector<size_t>&  a_parameterRegisters,
                                                const std::vector<size_t>&  a_returnRegisters,
                                                const std::vector<size_t>&  a_callerSavedRegisters,
                                                const std::vector<size_t>&  a_calleeSavedRegisters);
        
        void getOverridenParams(std::vector<size_t> (&regParsOverride)[RB_AMOUNT]) const;
    private:
        Backend* m_backend;
        // Sometimes register can exist in more than one vessel(like parameter and return), so we have to trace
        // register to be erased from all of them.
        void removeFromAllVessels(int basketNum, size_t reg);

        enum { PARAMS_VESS = 0, RETURN_VESS = 1, CALLER_VESS = 2, CALLEE_VESS = 3, VESS_AMOUNT = 4, REG_MAX = 64, REG_UNDEF = 255 };
        enum { NOREGISTER = -1, MAXIMUM_SPILLS = 3}; //TODO(ch):need more detailed spill scheme, than just 3 spills.
        // Register pool have internal ordering of registers for supporting correct providing sequence.
        uint8_t m_reorderArch2Inner[RB_AMOUNT][VESS_AMOUNT][REG_MAX];
        uint8_t m_reorderInner2Arch[RB_AMOUNT][VESS_AMOUNT][REG_MAX];
        uint64_t m_vessel[RB_AMOUNT][VESS_AMOUNT];
        uint64_t m_pool[RB_AMOUNT]; // Pool is used for easy registers amount calculation and spill placeholders management.
        uint64_t m_spillPlaceholders[RB_AMOUNT]; //There used outer register ordering
        uint64_t m_spillPlaceholdersAvailable[RB_AMOUNT];
        std::set<RegIdx> m_usedCallee[RB_AMOUNT];
        std::vector<size_t> m_registersO[RB_AMOUNT][VESS_AMOUNT];
    };

/*
TODO(ch): Implement with RISC-V RVV
There can be useful next approach:
Do not choose upper-level multiplier(M1, M2, ..M8) until calculation amount of needed
vector registers(which can be done only on register allocation). When amount is known,
it's simple to find appropriate multiplier, defining vector register set able to fit
this amount with maximal length of vector.
E.g:
M8 provide 4 highest-level registers of hierarchy depth = 6.
M4 provide 8 highest-level registers of hierarchy depth = 5.
M1 provide 32 highest-level registers of hierarchy depth = 3.
MF8 provide 256 highest-level registers of hierarchy depth = 0.
*/
class FuncImpl;
class RegisterAllocator //TODO(ch): Can you make it derivative from CompilerStage?
{
public:
    RegisterAllocator(ContextImpl* a_owner);
    void process(FuncImpl* a_func, Syntfunc& a_processed);
    inline size_t epilogueSize() const { return m_epilogueSize; }
    RegisterPool& getRegisterPool() { return m_pool; }
private:
    ContextImpl* m_owner;
    RegisterPool m_pool;
    size_t m_epilogueSize;
};
};
#endif // __LOOPS_REG_ALLOCATOR_HPP__
