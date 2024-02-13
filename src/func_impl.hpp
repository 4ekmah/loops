/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_FUNC_IMPL_HPP__
#define __LOOPS_FUNC_IMPL_HPP__

#include "loops/loops.hpp"
#include "common.hpp"
#include "code_collecting.hpp"
#include "pipeline.hpp"
#include <deque>
#include <map>
#include <set>

namespace loops {

class ContextImpl;

class FuncImpl : public Func
{
public:
    FuncImpl(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params);
    static Func makeWrapper(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params);

    std::string name() const { return m_pipeline->get_data().name; }
    int signature() const { return (int)m_pipeline->get_data().params.size(); }
    void* ptr();
    void endFunc();
    void printBytecode(std::ostream& out, const std::string& uptoPASS = "CP_COLLECTING");
    void printAssembly(std::ostream& out, int columns);

    inline ContextImpl* getContext() { return m_context; }

    const Syntfunc& get_data() const;
    const FuncBodyBuf get_hex_body();
    inline CodeCollecting* get_code_collecting()
    {
        AssertMsg(m_pipeline.get(), "Attempt to add instruction to already finished function.");
        return m_pipeline->get_code_collecting();
    }
    void set_compiled_ptr(void* ptr) {m_compiled = ptr;}  //TODO(ch): I don't like this scheme. it's better to separate "compile" pass to "compile2buf" "writeBuf2exe"
    //directTranslation == true avoids most part of passes, like register allocation or controlBlocks2Jumps.
    //It's assumed, that code is already written in manner of being projected to target architecture. It's used for tests only(even for listing-only tests).
    inline void directTranslationOn() { m_directTranslation = true; }
    void overrideRegisterSet(int basketNum, const std::vector<int>&  a_parameterRegisters,
        const std::vector<int>&  a_returnRegisters,
        const std::vector<int>&  a_callerSavedRegisters,
        const std::vector<int>&  a_calleeSavedRegisters);

    size_t m_refcount; //TODO: I must check if refcounting and impl logic is threadsafe.
private:
    ContextImpl* m_context;

    std::shared_ptr<Pipeline> m_pipeline;
    std::shared_ptr<Pipeline> m_debug_pipeline;
    void* m_compiled;

    bool m_directTranslation;
};

inline FuncImpl* getImpl(Func* wrapper)
{
    if (!wrapper)
        throw std::runtime_error("Null context pointer.");
    return static_cast<FuncImpl*>(_getImpl(wrapper));
}
}
#endif // __LOOPS_FUNC_IMPL_HPP__
