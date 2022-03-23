/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_AARCH64_HPP__
#define __LOOPS_AARCH64_HPP__

#include "composer.hpp"
#include "common.hpp"
#include <vector>
#include <map>

namespace loops
{
enum {
    A64_LDRSW_R = 0,
    A64_LDRSW_I,
    A64_STR32_I,
    A64_MOV_I,
    A64_MOV_R,
    A64_ADD_R,
    A64_MUL,
    A64_SDIV,
    A64_CMP_R,
    A64_B_LT,
    A64_B_LE,
    A64_B_GT,
    A64_B_GE,
    A64_RET
};

std::vector<syntop> bytecode2arm64(const std::vector<syntop>& bytecode, const std::vector<IRegInternal>& parameters, IRegInternal a_retreg); //TODO(ch): use funcimpl as function parameter.
const instruction_set& aarch64instructionset();

class AArch64Compiler : public RCCompiler
{
public:
    AArch64Compiler() {};
    virtual void* compile(Context* a_ctx, Func* a_func) const override final;
};

};
#endif //__LOOPS_AARCH64_HPP__
