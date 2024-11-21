/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "tests.hpp"
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    unzip_listings();

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "riscV.instruction_set_test:basic.a_plus_b:basic.min_max_scalar:basic.triangle_types:basic.ten_args_to_sum:basic.nonnegative_odd:basic.all_loads_all_stores:math/fixed_power.fixed_power/0:math/fixed_power.fixed_power/1:math/fixed_power.fixed_power/2"; //DUBUG: return original line.
    
    int res = RUN_ALL_TESTS();
    refresh_zip_listings();

    return res;
}
