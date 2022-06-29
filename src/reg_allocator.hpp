/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
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
        size_t freeRegsAmount() const;
        inline bool havefreeRegs() const { return freeRegsAmount() > 0; }
        IRegInternal provideParamFromPool();  //Must be called first.
        IRegInternal provideRegFromPool(IRegInternal a_hint = IReg::NOIDX);
        IRegInternal provideReturnFromPool(); //Must be called last.
        void releaseRegister(IRegInternal freeReg);

        IRegInternal provideSpillPlaceholder();
        void clearSpillPlaceholders();

        inline std::set<IRegInternal> usedCallee() const { return m_usedCallee; }
        void overrideRegisterSet(uint64_t a_parameterRegisters, uint64_t a_returnRegisters,
                                 uint64_t a_callerSavedRegisters, uint64_t a_calleeSavedRegisters);
    private:
        void removeFromAllBaskets(size_t reg);

        Backend* m_backend;
        std::set<IRegInternal> m_usedCallee;

        // Sometimes register can exist in more than one basket(like parameter and return), so we have to trace 
        // register to be erased from all of them. That's why we are using one pool for controling engagement 
        // and four static bit masks, which are just marking which register is belong to which basket.
        uint64_t m_pool;
        uint64_t m_parameterRegisters;
        uint64_t m_returnRegisters;
        uint64_t m_callerSavedRegisters;
        uint64_t m_calleeSavedRegisters;
        uint64_t m_spillPlaceholders;
        uint64_t m_spillPlaceholdersAvailable;
        enum { NOREGISTER = -1, MAXIMUM_SPILLS = 3}; //TODO(ch):need more detailed scheme, than just 3 spills.
        // Register pool have internal ordering of registers for supporting correct providing sequence. 
        // (There must be provided first registers from parameter basket, further: return - callerSaved - calleeSaved).
        std::vector<IRegInternal> m_regOrder;
        std::vector<size_t> m_invertOrderMapping;

        uint64_t m_parameterRegistersO;
        uint64_t m_returnRegistersO;
        uint64_t m_callerSavedRegistersO;
        uint64_t m_calleeSavedRegistersO;
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
    void process(FuncImpl* a_func, Syntfunc& a_processed, size_t a_virtualRegsAmount);
    inline size_t epilogueSize() const { return m_epilogueSize; }
    RegisterPool& getRegisterPool() { return m_pool; }
private:
    ContextImpl* m_owner;
    RegisterPool m_pool;
    size_t m_epilogueSize;
};
};
#endif // __LOOPS_REG_ALLOCATOR_HPP__
