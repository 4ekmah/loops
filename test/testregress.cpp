/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "tests.hpp"
#include "testregress/test_basic.hpp"
#include "testregress/test_math.hpp"
#include "testregress/test_aarch64.hpp"
#include "testregress/test_intel64.hpp"
#include "testregress/test_riscV.hpp"
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    unzip_listings();

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "riscV.instruction_set_test:basic.a_plus_b:basic.min_max_scalar:basic.triangle_types:basic.ten_args_to_sum"; //DUBUG: return original line.
    int res = RUN_ALL_TESTS();
    refresh_zip_listings();

    return res;
}
