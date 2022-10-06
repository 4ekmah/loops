/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "tests.hpp"
#include "testregress/test_basic.hpp"
#include "testregress/test_math.hpp"
#include "testregress/test_aarch64.hpp"
#include "testregress/test_intel64.hpp"

int main(int argc, char** argv)
{
    loops::TestSuite* suite = loops::TestSuite::getInstance();
    suite->run(false);
    return 0;
}
