/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "loops/loops.hpp"
#include "loopslayers/generators/maxpool.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#if __has_include(<valgrind/callgrind.h>)
#include <valgrind/callgrind.h>
#define MAXPOOL_CALLGRIND_START() CALLGRIND_START_INSTRUMENTATION
#define MAXPOOL_CALLGRIND_STOP() CALLGRIND_STOP_INSTRUMENTATION
#else
#define MAXPOOL_CALLGRIND_START() ((void)0)
#define MAXPOOL_CALLGRIND_STOP() ((void)0)
#endif

namespace
{
using Clock = std::chrono::steady_clock;

struct CompileResult
{
    maxpool_f32_t kernel = nullptr;
    int64_t nanoseconds = 0;
};

class LoopsFunction
{
public:
    LoopsFunction()
    {
        func = loops::MaxpoolGenerator<float>(ctx).generateFunc(
            13, 13, 6, 6, 6, 6, 1, 1, ACT_NONE, 1.0f);
    }

    CompileResult compile(bool profile)
    {
        CompileResult result;
        const auto start = Clock::now();
        if (profile)
            MAXPOOL_CALLGRIND_START();
        void* entry = func.ptr();
        if (profile)
            MAXPOOL_CALLGRIND_STOP();
        const auto stop = Clock::now();
        if (!entry)
            throw std::runtime_error("Loops compilation failed");
        result.kernel = reinterpret_cast<maxpool_f32_t>(entry);
        result.nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
        return result;
    }

private:
    loops::Context ctx;
    loops::Func func;
};

struct MaxpoolFixture
{
    static constexpr int kh = 13;
    static constexpr int kw = 13;
    static constexpr int nc = 5;
    static constexpr int h = 11;
    static constexpr int w = 11;
    static constexpr int h0 = 11;
    static constexpr int w0 = 11;
    static constexpr int padding = 6;
    static constexpr int input_size = nc * h * w;
    static constexpr int output_size = nc * h0 * w0;
    static constexpr float sentinel = 338001.0f;

    std::vector<float> input;
    std::vector<float> reference;
    dwc_algs_limits limits;

    MaxpoolFixture() : input(input_size), reference(output_size)
    {
        for (int i = 0; i < input_size; ++i)
            input[i] = static_cast<float>((i * 2654435761u + 1013904223u) % 10000u) / 5000.0f - 1.0f;

        constexpr int wp = w + 2 * padding;
        constexpr int hp = h + 2 * padding;
        std::vector<float> padded(nc * hp * wp, 0.0f);
        for (int c = 0; c < nc; ++c)
            for (int y = 0; y < h; ++y)
                for (int x = 0; x < w; ++x)
                    padded[(c * hp + y + padding) * wp + x + padding] = input[(c * h + y) * w + x];

        for (int c = 0; c < nc; ++c)
            for (int y = 0; y < h0; ++y)
                for (int x = 0; x < w0; ++x)
                {
                    float value = padded[(c * hp + y) * wp + x];
                    for (int ky = 0; ky < kh; ++ky)
                        for (int kx = 0; kx < kw; ++kx)
                            value = std::max(value, padded[(c * hp + y + ky) * wp + x + kx]);
                    reference[(c * h0 + y) * w0 + x] = value;
                }

        loops::Context ctx;
        limits = loops::MaxpoolGenerator<float>(ctx).calc_maxpool_algs_limits(
            nc, h, w, kh, kw, h0, w0, padding, padding, padding, padding, 1, 1);
    }

    bool verify(maxpool_f32_t kernel)
    {
        std::vector<float> output(3 * output_size, sentinel);
        float* result = output.data() + output_size;
        const int64_t status = kernel(input.data(), h, w, nc, result, h0, w0, &limits);
        if (status != 0)
        {
            std::cerr << "loops: nonzero return value " << status << '\n';
            return false;
        }
        for (int i = 0; i < 3 * output_size; ++i)
        {
            const float expected = (i >= output_size && i < 2 * output_size)
                ? reference[i - output_size] : sentinel;
            if (output[i] != expected)
            {
                std::cerr << "loops: mismatch at " << i
                    << ", got " << output[i] << ", expected " << expected << '\n';
                return false;
            }
        }
        std::cout << "loops: output matches reference (" << output_size << " values plus guards)\n";
        return true;
    }
};

int64_t compile_once(bool profile)
{
    LoopsFunction function;
    return function.compile(profile).nanoseconds;
}

bool verify()
{
    MaxpoolFixture fixture;
    LoopsFunction function;
    return fixture.verify(function.compile(false).kernel);
}

int64_t median(std::vector<int64_t> values)
{
    std::sort(values.begin(), values.end());
    const size_t middle = values.size() / 2;
    if (values.size() % 2)
        return values[middle];
    return values[middle - 1] + (values[middle] - values[middle - 1]) / 2;
}

void measure(int iterations)
{
    for (int i = 0; i < 3; ++i)
        compile_once(false);

    std::vector<int64_t> samples;
    samples.reserve(iterations);
    for (int iteration = 0; iteration < iterations; ++iteration)
        samples.push_back(compile_once(true));

    const auto minmax = std::minmax_element(samples.begin(), samples.end());
    std::cout << "loops: iterations=" << iterations
        << ", median_ns=" << median(samples)
        << ", min_ns=" << *minmax.first
        << ", max_ns=" << *minmax.second << '\n';
}
}

int main(int argc, char** argv)
{
    try
    {
        int iterations = 100;
        if (argc == 2)
            iterations = std::stoi(argv[1]);
        else if (argc != 1)
        {
            std::cerr << "Usage: maxpool_compile [ITERATIONS]\n";
            return 2;
        }
        if (iterations <= 0)
            throw std::runtime_error("Iteration count must be positive");

        std::cout << "maxpool13: FP32, kernel=13x13, NC=5, H=W=H0=W0=11, "
            "padding=6, stride=1x1\n";
        if (!verify())
            return 1;
        measure(iterations);
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }
}
