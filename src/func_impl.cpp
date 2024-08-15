/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "func_impl.hpp"
#include "backend.hpp"
#include "composer.hpp"
#include "printer.hpp"
#include "reg_allocator.hpp"
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <deque>
#include <iostream>

namespace loops
{
FuncImpl::FuncImpl(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params) : m_refcount(0) //TODO(ch): support vector parameters
    , m_context(ctx) 
    , m_pipeline(std::make_shared<Pipeline>(ctx->getBackend(), this, name, params))
    , m_compiled(nullptr)
    
    , m_directTranslation(false)
{
}

Func FuncImpl::makeWrapper(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params)
{
    return Func::make(new FuncImpl(name, ctx, params));
}

void* FuncImpl::ptr()
{
    if(m_compiled == nullptr)
    {
        Assert(m_pipeline.get());
        FuncBodyBuf body = get_hex_body();
        Backend* backend = m_context->getBackend();
        Allocator* alloc = backend->getAllocator();
        m_compiled = alloc->allocate(body->size());
        memcpy(m_compiled, (void*)&(body->operator[](0)), body->size()); //TODO(ch): You have to change used adressess before.
        alloc->protect2Execution((uint8_t*)m_compiled);
    }
    return m_compiled;
}

void FuncImpl::overrideRegisterSet(int basketNum, const std::vector<int>& a_parameterRegisters,
    const std::vector<int>& a_returnRegisters,
    const std::vector<int>& a_callerSavedRegisters,
    const std::vector<int>& a_calleeSavedRegisters)
{
    m_pipeline->overrideRegisterSet(basketNum, a_parameterRegisters, a_returnRegisters, a_callerSavedRegisters, a_calleeSavedRegisters);
}

void FuncImpl::printIR(std::ostream& out, int columns, const std::string& uptoPass_)
{
    Pipeline l_pipeline(*(m_context->debug_mode() ? m_debug_pipeline.get(): m_pipeline.get()));
    std::string uptoPass = uptoPass_;
    if (uptoPass == "")
    {
        std::vector<std::string>  allpasses = l_pipeline.get_all_passes();
        auto found = std::find(allpasses.begin(), allpasses.end(), "CP_IR_TO_ASSEMBLY");
        Assert(found != allpasses.end() && found != allpasses.begin());
        uptoPass = *(found - 1);
    }
    l_pipeline.run_until(uptoPass);
    program_printer* _printer;
    Assert(create_ir_printer(columns, &_printer) == 0);
    syntfunc2print s2p;
    s2p.name = (char*)(l_pipeline.get_data().name.c_str());
    int err = loops_span_construct(&(s2p.program), (Syntop*)l_pipeline.get_data().program.data(), (int)l_pipeline.get_data().program.size());
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    err = loops_span_construct(&(s2p.params), (Arg*)l_pipeline.get_data().params.data(), (int)l_pipeline.get_data().params.size());
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    char* printed_str;
    err = sprint_syntfunc(_printer, &printed_str, &s2p);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    free_printer(_printer);
    out << printed_str;
    free(printed_str);
}

void FuncImpl::printAssembly(std::ostream& out, int columns)
{
    Pipeline l_pipeline(*(m_context->debug_mode() ? m_debug_pipeline.get(): m_pipeline.get()));
    l_pipeline.run_until("CP_IR_TO_ASSEMBLY");
    program_printer* _printer;
    Assert(create_assembly_printer(columns, m_context->getBackend(), &_printer) == 0);
    syntfunc2print s2p;
    s2p.name = (char*)(l_pipeline.get_data().name.c_str());
    int err = loops_span_construct(&(s2p.program), (Syntop*)l_pipeline.get_data().program.data(), (int)l_pipeline.get_data().program.size());
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    err = loops_span_construct(&(s2p.params), (Arg*)l_pipeline.get_data().params.data(), (int)l_pipeline.get_data().params.size());
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    char* printed_str;
    err = sprint_syntfunc(_printer, &printed_str, &s2p);
    if(err != LOOPS_ERR_SUCCESS)
        throw std::runtime_error(get_errstring(err));
    free_printer(_printer);
    out << printed_str;
    free(printed_str);
}

const Syntfunc& FuncImpl::get_data() const
{
    if(m_pipeline.get())
        return m_pipeline->get_data(); 
    AssertMsg(m_context->debug_mode(), "Function is already compiled.");
    return m_debug_pipeline->get_data();
}

const FuncBodyBuf FuncImpl::get_hex_body()
{
    m_pipeline->full_run();
    const FuncBodyBuf res = m_pipeline->result_buffer();
    m_pipeline.reset();
    return res;
}

void FuncImpl::endFunc()
{
    m_pipeline->run_until("CP_COLLECTING");
    if (m_directTranslation)
        m_pipeline->skip_until("CP_IR_TO_ASSEMBLY");
    if (m_context->debug_mode())
        m_debug_pipeline = std::make_shared<Pipeline>(*(m_pipeline.get()));
}
}
