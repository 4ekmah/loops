/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "loops/loops.hpp"
#include <iostream>
#include "tests.hpp"
#if __LOOPS_ARCH ==  __LOOPS_AARCH64
#include "testperf/depthwiseconv.hpp"
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64

//TODO(ch): create normal class for performance testing and integrate it in general testing system, please.
int main(int argc, char** argv)
{
    loops::Context CTX;
#if __LOOPS_ARCH ==  __LOOPS_AARCH64
    loops::DepthwiseconvTest dwc(CTX, &std::cout);
    dwc.run();
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
    return 0;
}
