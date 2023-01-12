/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_PIPELINE_HPP__
#define __LOOPS_PIPELINE_HPP__

#include "loops/loops.hpp"
#include "common.hpp"
#include "code_collecting.hpp"
#include <deque>
#include <map>
#include <unordered_map>
#include <set>

namespace loops {

enum
{
    CS_NOSTAGE = 0,
    CS_COLLECTING,
    CS_LIVENESS_ANALYSIS,
    CS_REGISTER_ALLOCATION,
    CS_CONTROLFLOW_TO_JUMPS,
    CS_BYTECODE_TO_ASSEMBLY,
    CS_ASSEMBLY_TO_HEX,
    CS_ARCH_SPECIFIC,
};

class Cf2jumps: public CompilerStage
{
public:
    Cf2jumps(const Backend* a_backend, int a_epilogueSize, Func* a_func);
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return false; }
    virtual StageID stage_id() const override final { return CS_CONTROLFLOW_TO_JUMPS; }
    virtual ~Cf2jumps() {}
private:
    Func* m_func;
    int m_epilogueSize;
};

class Bytecode2Assembly : public CompilerStage
{
public:
    Bytecode2Assembly(const Backend* a_backend) : CompilerStage(a_backend) {}
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return false; }
    virtual StageID stage_id() const override final { return CS_BYTECODE_TO_ASSEMBLY; }
    virtual ~Bytecode2Assembly() override {}
protected:
    struct label_ref_info
    {
        size_t opnum;
        size_t argnum;
        size_t opoffset;
        label_ref_info() : opnum(0), argnum(0), opoffset(0) {}
        label_ref_info(size_t a_opnum, size_t a_argnum, size_t a_opoffset) : opnum(a_opnum), argnum(a_argnum), opoffset(a_opoffset) {}
    };
};

class Assembly2Hex : public CompilerStage
{
public:
    Assembly2Hex(const Backend* a_backend) : CompilerStage(a_backend), m_bitstream(a_backend) {}
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return true; }
    virtual StageID stage_id() const override final { return CS_ASSEMBLY_TO_HEX; }
    virtual ~Assembly2Hex() override {}
    const FuncBodyBuf result_buffer() const;
private:
    Bitwriter m_bitstream;
};

class Pipeline
{
public:
    Pipeline(Backend* a_backend, Func* a_func, const std::string& name, std::initializer_list<IReg*> params);
    inline void full_run() { run_until_including(CS_ASSEMBLY_TO_HEX); }
    void run_until(StageID a_stageID);
    void run_until_including(StageID a_stageID);
    inline const Syntfunc& get_data() const { return m_data; }
    const FuncBodyBuf result_buffer() const { return m_buffer; }
    CodeCollecting* get_code_collecting();
    void overrideRegisterSet(int basketNum, const std::vector<size_t>&  a_parameterRegisters,
                                            const std::vector<size_t>&  a_returnRegisters,
                                            const std::vector<size_t>&  a_callerSavedRegisters,
                                            const std::vector<size_t>&  a_calleeSavedRegisters);
private:
    void run();
    void run_stage(CompilerStage* a_stage);
    Syntfunc m_data;
    CodeCollecting m_codecol;
    Backend* m_backend;
    Func* m_func;
    FuncBodyBuf m_buffer;
    int m_current_stage;
    int m_target_stage;
    int m_mode;
    std::unordered_map<int, int> m_stage_ordering; //TODO(ch): make it static?
    enum {PM_FINDORDER, PM_REGULAR};
    std::vector<size_t> m_parameterRegistersO[RB_AMOUNT];
    std::vector<size_t> m_returnRegistersO[RB_AMOUNT];
    std::vector<size_t> m_callerSavedRegistersO[RB_AMOUNT];
    std::vector<size_t> m_calleeSavedRegistersO[RB_AMOUNT];
};
};
#endif // __LOOPS_PIPELINE_HPP__
