/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_MPTEST_HPP__
#define __LOOPS_MPTEST_HPP__
#include "loops/loops.hpp"
#include "loopslayers/loopslayers.h"
#if __LOOPS_ARCH ==  __LOOPS_AARCH64 || __LOOPS_ARCH ==  __LOOPS_INTEL64
namespace loops
{

class MaxpoolTest
{
public:
    MaxpoolTest(std::ostream* a_out);
    virtual ~MaxpoolTest();
    virtual void run();
protected:
    MaxpoolTest* impl;
    MaxpoolTest(): impl(nullptr) {}
};
bool compare_algs_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference, std::ostream* out);
void print_algs_limits(const dwc_algs_limits& tocheck, std::ostream* out);
}

#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64 || __LOOPS_ARCH ==  __LOOPS_INTEL64
#endif //__LOOPS_MPTEST_HPP__