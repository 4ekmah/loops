/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "loops/loops.hpp"
#include "loopslayers/generators/maxpool.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace
{
using Clock = std::chrono::steady_clock;

int64_t compileOnce()
{
    loops::Context ctx;
    loops::Func func = loops::MaxpoolGenerator<float>(ctx).generateFunc(
        13, 13, 6, 6, 6, 6, 1, 1, ACT_NONE, 1.0f);
    const auto start = Clock::now();
    void* entry = func.ptr();
    const auto stop = Clock::now();
    if (!entry)
        throw std::runtime_error("Loops compilation failed");
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
}
}

int main()
{
    try
    {
        for (int warmup = 0; warmup < 3; ++warmup)
            compileOnce();

        constexpr size_t iterations = 200;
        std::array<int64_t, iterations> samples;
        for (int64_t& sample : samples)
            sample = compileOnce();
        std::sort(samples.begin(), samples.end());
        const int64_t median = samples[iterations / 2 - 1]
            + (samples[iterations / 2] - samples[iterations / 2 - 1]) / 2;

        std::cout << "maxpool13: median of 200 compilations\n";
        std::cout << "median_ns=" << median << '\n';
        std::cout << std::fixed << std::setprecision(6)
            << "median_ms=" << static_cast<double>(median) / 1000000.0 << '\n';
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }
}
