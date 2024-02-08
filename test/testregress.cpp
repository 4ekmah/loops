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
#include <gtest/gtest.h>
#include <zip_file.hpp>

int main(int argc, char** argv)
{
    // Write example
    // miniz_cpp::zip_file file;
    // file.write("/home/qohen/development/linux/loops/test/refasm/aarch64/linux/a_plus_b.tst", "meow/a_plus_b.tst");
    // file.save("/home/qohen/development/linux/loops/test/refasm/aarch64_linux.zip");

    // Read example
    // miniz_cpp::zip_file file("/home/qohen/development/linux/loops/test/refasm/aarch64_linux.zip");
    // // auto meow = file.infolist();
    // // file.extract(meow[0], "/home/qohen/development/linux/loops/test/refasm");
    // // Subdirectory meow have to be precreated, miniz doesn't care about it and fails to create new file in given path.
    // file.extract("meow/a_plus_b.tst", "/home/qohen/development/linux/loops/test/refasm");
    // return 0;
        
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
