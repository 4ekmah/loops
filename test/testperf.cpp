/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#include <iostream>
#include "tests.hpp"
#if __LOOPS_ARCH ==  __LOOPS_AARCH64
#include "testperf/dwctest.hpp"
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#if __LOOPS_ARCH ==  __LOOPS_AARCH64 || __LOOPS_ARCH ==  __LOOPS_INTEL64
#include "testperf/mptest.hpp"
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64 || __LOOPS_ARCH ==  __LOOPS_INTEL64

//TODO(ch): create normal class for performance testing and integrate it in general testing system, please.
int main(int /*argc*/, char** /*argv*/)
{
#if __LOOPS_ARCH ==  __LOOPS_AARCH64
    loops::DepthwiseconvTest dwc(&std::cout); //TODO(ch): use common context.
    dwc.run();
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#if __LOOPS_ARCH ==  __LOOPS_AARCH64 || __LOOPS_ARCH ==  __LOOPS_INTEL64
    loops::MaxpoolTest mp(&std::cout);
    mp.run();
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64 || __LOOPS_ARCH ==  __LOOPS_INTEL64
    return 0;
}
