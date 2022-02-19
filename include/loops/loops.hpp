/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_LOOPS_HPP__
#define __LOOPS_LOOPS_HPP__

#include <inttypes.h>
#include <string>

namespace loops
{

class Context;

struct IReg
{
    IReg();
    IReg(int64_t val);
    explicit IReg(const Context& ctx);

    IReg(const IReg& r);
    IReg& operator = (const IReg& r);

    int idx; // <0 for constant-initialized registers
    int64_t value; // for constant-initialized registers
    Context* ctx;
};

struct Label
{
    Label();
    explicit Label(const Context& ctx);

    Label(const Label& lbl);
    Label& operator = (const Label& lbl);

    int idx;
    Context* ctx;
};

template<typename _Tp> struct VReg
{
    typedef _Tp elemtype;

    VReg();
    explicit VReg(const Context& ctx);
    explicit VReg(_Tp val);

    VReg(const VReg<_Tp>& r);
    VReg<_Tp>& operator = (const VReg<_Tp>& r);
    VReg<_Tp>& operator = (_Tp val);

    int idx; // <0 for constant-initialized registers;
            // idx uses independent enumeration scheme from IReg::idx
    elemtype value; // for constant-initialized registers
    Context* ctx;
};

class Func
{
public:
    Func();
    Func(const std::string& name, const Context& ctx,
         std::initializer_list<IReg> params);
    Func(const Func& f);
    Func& operator = (const Func& f);
    virtual ~Func();
    
    std::string name() const;
    void call(std::initializer_list<int64_t> args) const;
    void* ptr(); // returns function pointer
protected:
    struct Impl;
    Impl* p;
};

class Context
{
public:
    Context();
    Context(const Context& ctx);
    virtual ~Context();
    Context& operator = (const Context& ctx);
    IReg param();

    void startfunc(const std::string& name, std::initializer_list<IReg> params);
    void endfunc(const IReg& retval);
    void getfuncs(std::vector<Func>& funcs);
    Func getfunc(const std::string& name);

    // control flow
    void do_();
    void do_if_(const IReg& r); // start loop if r == true
    void while_(const IReg& r); // continue loop if r == true
    void break_();
    void continue_();
    void if_(const IReg& r);
    void elif_(const IReg& r);
    void else_();
    void endif_();
    // direct call
    IReg call_(const IReg& addr, std::initializer_list<IReg> args);
    // indirect call
    IReg call_(const IReg& addr, const IReg& offset, std::initializer_list<IReg> args);

protected:
    struct Impl;
    Impl* p;
};

///////////////////////////// integer operations ///////////////////////

IReg load(const IReg& base);
IReg load(const IReg& base, const IReg& offset);
// load with zero/sign extension
template<typename _Tp> IReg load_(const IReg& base);
template<typename _Tp> IReg load_(const IReg& base, const IReg& offset);

void store(const IReg& base, const IReg& r);
void store(const IReg& base, const IReg& offset, const IReg& r);
// store part of register
template<typename _Tp> void store_(const IReg& base, const IReg& r);
template<typename _Tp> void store_(const IReg& base, const IReg& offset, const IReg& r);

IReg operator + (const IReg& a, const IReg& b);
IReg operator - (const IReg& a, const IReg& b);
IReg operator * (const IReg& a, const IReg& b);
IReg operator / (const IReg& a, const IReg& b);
IReg operator % (const IReg& a, const IReg& b);
IReg operator >> (const IReg& a, const IReg& b);
IReg operator << (const IReg& a, const IReg& b);
IReg operator & (const IReg& a, const IReg& b);
IReg operator | (const IReg& a, const IReg& b);
IReg operator ^ (const IReg& a, const IReg& b);
IReg operator ~ (const IReg& a);
IReg operator - (const IReg& a);
IReg operator == (const IReg& a, const IReg& b);
IReg operator != (const IReg& a, const IReg& b);
IReg operator <= (const IReg& a, const IReg& b);
IReg operator >= (const IReg& a, const IReg& b);
IReg operator > (const IReg& a, const IReg& b);
IReg operator < (const IReg& a, const IReg& b);
IReg select(const IReg& flag, const IReg& iftrue, const IReg& iffalse);
IReg max(const IReg& a, const IReg& b);
IReg min(const IReg& a, const IReg& b);
IReg abs(const IReg& a);
IReg sign(const IReg& a);

}

#endif
