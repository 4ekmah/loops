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

//DUBUGGG: Don't forget to consider ability of peek compilation(e.g., for printAssembly[which need Hex]) in pipeline design process
//Main idea for now is to copy Pipeline.
//In  debug mode FuncImpl must keep intermediate stages(like initial stage, after syntop stages and assembly stage)
class FuncImpl : public Func
{
public:
    FuncImpl(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params);
    static Func makeWrapper(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params);

    void call(std::initializer_list<int64_t> args) const;
    void* ptr();
    //directTranslation == true avoids most part of stages, like register allocation or controlBlocks2Jumps.
    //It's assumed, that code is already written in manner of being projected to target architecture. It's used for tests only(even for listing-only tests).
    inline void setDirectTranslation(bool directTranslation) { m_directTranslation = directTranslation; } //DUBUGGG: Implement somehow.

    void overrideRegisterSet(int basketNum, const std::vector<size_t>&  a_parameterRegisters,
        const std::vector<size_t>&  a_returnRegisters,
        const std::vector<size_t>&  a_callerSavedRegisters,
        const std::vector<size_t>&  a_calleeSavedRegisters);

    void setCompiledPtr(void* ptr) {m_compiled = ptr;}  //TODO(ch): I don't like this scheme. it's better to separate "compile" stage to "compile2buf" "writeBuf2exe"
                                                        //Generally, we have to implement FuncImpl as a pipeline, where each creating output ofeach stage deletes input(doesn't keep it).
    void printBytecode(std::ostream& out);
    void printAssembly(std::ostream& out, int columns);
    std::string name() const { return m_pipeline->get_data().name; }

    size_t m_refcount; //TODO: I must check if refcounting and impl logic is threadsafe.
    inline Context GetContext() { return m_context->getOwner(); }
    static FuncImpl* verifyArgs(std::initializer_list<Arg> args);
    const Syntfunc& getData() const { return m_pipeline->get_data(); }
    const FuncBodyBuf get_hex_body();
    inline CodeCollecting* get_code_collecting()
    {
        AssertMsg(m_pipeline.get(), "Attempt to add instruction to already finished function.");
        return m_pipeline->get_code_collecting();
    }
    void endFunc();

private:
    std::shared_ptr<Pipeline> m_pipeline;
    ContextImpl* m_context;
    bool m_directTranslation;
    
    enum {RT_NOTDEFINED, RT_REGISTER, RT_VOID};
    int m_returnType;

    void* m_compiled;
};

inline FuncImpl* getImpl(Func* wrapper)
{
    if (!wrapper)
        throw std::runtime_error("Null context pointer.");
    return static_cast<FuncImpl*>(_getImpl(wrapper));
}
};
#endif // __LOOPS_FUNC_IMPL_HPP__
