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

    class RegisterPool
    {
    public:
        RegisterPool(ContextImpl* m_owner);

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

        ContextImpl* m_owner;
        std::set<IRegInternal> m_usedCallee;
        static const size_t m_maximumSpills;

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
        static const size_t NOREGISTER;
        // Register pool have internal ordering of registers for supporting correct providing sequence. 
        // (There must be provided first registers from parameter basket, further: return - callerSaved - calleeSaved).
        std::vector<size_t> m_regOrder;
        std::vector<size_t> m_invertOrderMapping;

        uint64_t m_parameterRegistersO;
        uint64_t m_returnRegistersO;
        uint64_t m_callerSavedRegistersO;
        uint64_t m_calleeSavedRegistersO;
    };

class FuncImpl;
class RegisterAllocator //TODO(ch): Can you make it derivative from CompilerStage?
{
public:
    RegisterAllocator(ContextImpl* a_owner);
    void process(FuncImpl* a_func, Syntfunc& a_processed, size_t a_virtualRegsAmount);
    inline size_t epilogueSize() const { return m_epilogueSize; }
    RegisterPool& getRegisterPool() { return m_pool; }
private:
    std::map<IRegInternal, std::pair<size_t, size_t> > livenessAnalysis(Syntfunc& a_processed);
    ContextImpl* m_owner;
    RegisterPool m_pool;
    size_t m_epilogueSize;
};
};
#endif // __LOOPS_REG_ALLOCATOR_HPP__
