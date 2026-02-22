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
        LOOPS_ASSERT(m_pipeline.get());
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
        LOOPS_ASSERT(found != allpasses.end() && found != allpasses.begin());
        uptoPass = *(found - 1);
    }
    l_pipeline.run_until(uptoPass);
    program_printer_ptr _printer = program_printer::create_ir_printer(columns);
    std::string printed_str = _printer->sprint_syntfunc(l_pipeline.get_data());
    out << printed_str;
}

void FuncImpl::printAssembly(std::ostream& out, int columns)
{
    Pipeline l_pipeline(*(m_context->debug_mode() ? m_debug_pipeline.get(): m_pipeline.get()));
    l_pipeline.run_until("CP_IR_TO_ASSEMBLY");
    program_printer_ptr _printer = program_printer::create_assembly_printer(columns, m_context->getBackend());
    std::string printed_str = _printer->sprint_syntfunc(l_pipeline.get_data());
    out << printed_str;
}

const Syntfunc& FuncImpl::get_data() const
{
    if(m_pipeline.get())
        return m_pipeline->get_data(); 
    LOOPS_ASSERT_MSG(m_context->debug_mode(), "Function is already compiled.");
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
