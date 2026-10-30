/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "loops/loops.hpp"
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>

//Polynomial c0 + c1*x + c2*x^2 + c3*x^3, evaluated by Horner's scheme: y = c[k] + y * x.
//In each step result of fma coincides with multiplier, not with accumulator. On Intel, where
//fma is inplace by accumulator, it's non-commutative case, demanding auxiliary register.
static const float coeffs[4] = { 0.5f, -1.25f, 2.0f, 0.75f };

typedef int64_t (*horner_t)(float* dest, const float* src, int64_t n);
loops::Func genhorner(loops::Context& CTX)
{
    using namespace loops;
    IReg dest, src, n;
    USE_CONTEXT_(CTX);
    STARTFUNC_("fma_horner", &dest, &src, &n)
    {
        VReg<float> c0 = VCONST_(float, coeffs[0]);
        VReg<float> c1 = VCONST_(float, coeffs[1]);
        VReg<float> c2 = VCONST_(float, coeffs[2]);
        VReg<float> c3 = VCONST_(float, coeffs[3]);
        IReg offset = CONST_(0);
        WHILE_(n > 0)
        {
            VReg<float> x = loadvec<float>(src, offset);
            VReg<float> y = fma(c2, c3, x);
            y = fma(c1, y, x);
            y = fma(c0, y, x);
            storevec(dest, offset, y);
            offset += CTX.vbytes();
            n -= CTX.vlanes<float>();
        }
        RETURN_(0);
    }
    return CTX.getFunc("fma_horner");
}

int main(int /*argc*/, char** /*argv*/)
{
    loops::Context CTX;
    loops::Func hfunc = genhorner(CTX);

    std::cout << "--------FMAHORNEREXAMPLE---------" << std::endl;
    std::cout << "=========--IR-LISTING--==========" << std::endl;
    hfunc.printIR(std::cout);
    std::string platform = CTX.getPlatformName();
    std::transform(platform.begin(), platform.end(), platform.begin(), [](char t) {return (char)::toupper(t); });
    std::cout << "======--" << platform << "--LISTING--====== = " << std::endl;
    hfunc.printAssembly(std::cout);
    std::cout << "======--FUNCTION-OUTPUT---=======" << std::endl;
    horner_t f = reinterpret_cast<horner_t>(hfunc.ptr());
    const int vlanes = CTX.vlanes<float>();
    std::vector<float> src(vlanes * 4);
    std::vector<float> dest(src.size(), 0.f);
    for (int i = 0; i < (int)src.size(); i++)
        src[i] = 0.125f * (float)i - 1.f;
    f(&dest[0], &src[0], (int64_t)src.size());
    bool correct = true;
    for (int i = 0; i < (int)src.size(); i++)
    {
        float x = src[i];
        float expected = coeffs[0] + x * (coeffs[1] + x * (coeffs[2] + x * coeffs[3]));
        if (std::abs(dest[i] - expected) > 1.e-5f * std::max(1.f, std::abs(expected)))
        {
            std::cout << "Mismatch at " << i << ": x=" << x << ", result=" << dest[i] << ", expected=" << expected << std::endl;
            correct = false;
        }
    }
    std::cout << (correct ? "All results are correct." : "Results are WRONG.") << std::endl;
    std::cout << std::endl << std::endl << std::endl;

    return correct ? 0 : 1;
}
