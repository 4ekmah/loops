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

class ImmediateImplantation: public CompilerPass
{
public:
    ImmediateImplantation(const Backend* a_backend);
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return false; }
    virtual std::string pass_id() const override final { return "CP_IMMEDIATE_IMPLANTATION"; }
    virtual ~ImmediateImplantation() {}
};

class ElifElimination: public CompilerPass
{
public:
    ElifElimination(const Backend* a_backend);
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return false; }
    virtual std::string pass_id() const override final { return "CP_ELIF_ELIMINATION"; }
    virtual ~ElifElimination() {}
};

class Cf2jumps: public CompilerPass
{
public:
    Cf2jumps(const Backend* a_backend, int a_epilogueSize);
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return false; }
    virtual std::string pass_id() const override final { return "CP_CONTROLFLOW_TO_JUMPS"; }
    virtual ~Cf2jumps() {}
private:
    int m_epilogueSize;
};

class IR2Assembly : public CompilerPass
{
public:
    IR2Assembly(const Backend* a_backend) : CompilerPass(a_backend) {}
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return false; }
    virtual std::string pass_id() const override final { return "CP_IR_TO_ASSEMBLY"; }
    virtual ~IR2Assembly() override {}
protected:
    struct label_ref_info
    {
        int opnum;
        int argnum;
        int opoffset;
        label_ref_info() : opnum(0), argnum(0), opoffset(0) {}
        label_ref_info(int a_opnum, int a_argnum, int a_opoffset) : opnum(a_opnum), argnum(a_argnum), opoffset(a_opoffset) {}
    };
};

class Assembly2Hex : public CompilerPass
{
public:
    Assembly2Hex(const Backend* a_backend) : CompilerPass(a_backend), m_bitstream(a_backend) {}
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return true; }
    virtual std::string pass_id() const override final { return "CP_ASSEMBLY_TO_HEX"; }
    virtual ~Assembly2Hex() override {}
    const FuncBodyBuf result_buffer() const;
private:
    Bitwriter m_bitstream;
};

class Pipeline
{
public:
    Pipeline(Backend* a_backend, Func* a_func, const std::string& name, std::initializer_list<IReg*> params);
    inline void full_run() { run_until("CP_ASSEMBLY_TO_HEX"); }
    void run_until(const std::string& a_passID);
    void run_all_before(const std::string& a_passID);
    void skip_until(const std::string& a_passID);
    inline const Syntfunc& get_data() const { return m_data; }
    const FuncBodyBuf result_buffer() const { return m_buffer; }
    CodeCollecting* get_code_collecting();
    void overrideRegisterSet(int basketNum, const std::vector<int>&  a_parameterRegisters,
                                            const std::vector<int>&  a_returnRegisters,
                                            const std::vector<int>&  a_callerSavedRegisters,
                                            const std::vector<int>&  a_calleeSavedRegisters);
    std::vector<std::string> get_all_passes();
private:
    void run();
    void run_pass(CompilerPass* a_pass);
    Syntfunc m_data;
    CodeCollecting m_codecol;
    Backend* m_backend;
    FuncBodyBuf m_buffer;
    int m_current_pass;
    int m_target_pass;
    int m_mode;
    std::unordered_map<std::string, int> m_pass_ordering; //TODO(ch): make it static?
    int m_cp_collecting_pass_num;
    enum {PM_FINDORDER, PM_REGULAR};
    std::vector<int> m_parameterRegistersO[RB_AMOUNT];
    std::vector<int> m_returnRegistersO[RB_AMOUNT];
    std::vector<int> m_callerSavedRegistersO[RB_AMOUNT];
    std::vector<int> m_calleeSavedRegistersO[RB_AMOUNT];
};
}
#endif // __LOOPS_PIPELINE_HPP__
