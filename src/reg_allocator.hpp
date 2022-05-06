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
class FuncImpl;
class RegisterAllocator //TODO(ch): Can you make it derivative from CompilerStage?
{
public:
    RegisterAllocator(ContextImpl* a_owner);
    void process(FuncImpl* a_func, Syntfunc& a_processed, size_t a_knownRegsAmount);
    inline size_t epilogueSize() const { return m_epilogueSize; }
    inline std::set<IRegInternal> usedCallee() const { return m_usedCallee; }
    
    void overrideFuncsRegisterSet(const std::vector<size_t>& a_parameterRegisters,// TODO(ch): this function is temporary solution for tests. Must be hidden in more developed version of library.
                                  const std::vector<size_t>& a_returnRegisters,
                                  const std::vector<size_t>& a_callerSavedRegisters,
                                  const std::vector<size_t>& a_calleeSavedRegisters);

private:
    ContextImpl* m_owner;
    void initRegisterPool();
    size_t overallRegisterAmount() const;
    size_t freeRegsAmount() const;
    bool havefreeRegs() const;
    IRegInternal provideParamFromPool(); //Must be called first.
    IRegInternal provideRegFromPool(IRegInternal a_hint = IReg::NOIDX);
    IRegInternal provideSpillPlaceholder();
    IRegInternal provideReturnFromPool();//Must be called last.
    void clearSpillPlaceholders();
    void removeFromAllBaskets(IRegInternal reg);
    void releaseRegister(IRegInternal freeReg);
    std::map<IRegInternal, std::pair<size_t, size_t> > livenessAnalysis(Syntfunc& a_processed);
    std::vector<IRegInternal> m_parameterRegisters;
    std::vector<IRegInternal> m_returnRegisters;
    std::vector<IRegInternal> m_callerSavedRegisters;
    std::vector<IRegInternal> m_calleeSavedRegisters;
    std::unordered_map<IRegInternal, int> m_registersDistr;
    std::set<IRegInternal> m_usedCallee;
    size_t m_spillPlaceholdersTop;
    size_t m_spillPlaceholdersAvailable;
    size_t m_epilogueSize;
    size_t m_knownRegsAmount;
    size_t m_snippetCausedSpills;

    std::vector<IRegInternal> m_parameterRegistersO;
    std::vector<IRegInternal> m_returnRegistersO;
    std::vector<IRegInternal> m_callerSavedRegistersO;
    std::vector<IRegInternal> m_calleeSavedRegistersO;

};
};
#endif // __LOOPS_REG_ALLOCATOR_HPP__
