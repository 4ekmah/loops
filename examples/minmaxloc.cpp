/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "loops/loops.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

typedef int (*minmaxpos_t)(const int* ptr, int64_t n, int* minpos, int* maxpos);
loops::Func genminmaxloc(loops::Context& CTX)
{
    using namespace loops;
    IReg ptr, n, minpos_addr, maxpos_addr;
    USE_CONTEXT_(CTX);
    STARTFUNC_("minmaxloc", &ptr, &n, &minpos_addr, &maxpos_addr)
    {
        IReg i = CONST_(0);
        IReg minpos = CONST_(0);
        IReg maxpos = CONST_(0);
        IReg minval = load_<int>(ptr);
        IReg maxval = minval;
        n *= sizeof(int);
        WHILE_(i < n)
        {
            IReg x = load_<int>(ptr, i);
            IF_(x < minval)
            {
                minval = x;
                minpos = i;
            }
            IF_(x > maxval)
            {
                maxval = x;
                maxpos = i;
            }
            i += sizeof(int);
        }
        IReg elemsize = CONST_(sizeof(int));
        minpos /= elemsize;
        maxpos /= elemsize;
        store_<int>(minpos_addr, minpos);
        store_<int>(maxpos_addr, maxpos);
        RETURN_(0);
    }
    return CTX.getFunc("minmaxloc");
}

int main(int /*argc*/, char** /*argv*/)
{
    loops::Context CTX;
    loops::Func mmlfunc = genminmaxloc(CTX);

    std::cout << "--------MINMAXLOCEXAMPLE---------" << std::endl;
    std::cout << "=========--IR-LISTING--==========" << std::endl;
    mmlfunc.printIR(std::cout);
    std::string platform = CTX.getPlatformName();
    std::transform(platform.begin(), platform.end(), platform.begin(), [](char t) {return (char)::toupper(t); });
    std::cout << "======--" << platform << "--LISTING--====== = " << std::endl;
    mmlfunc.printAssembly(std::cout);
    std::cout << "======--FUNCTION-OUTPUT---=======" << std::endl;
    minmaxpos_t f = reinterpret_cast<minmaxpos_t>(mmlfunc.ptr());
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    int minpos = -1, maxpos = -1;
    int retval = f(&v[0], v.size(), &minpos, &maxpos);
    std::cout << "retval=" << retval<< ", minpos="<< (int)minpos <<", maxpos="<< (int)maxpos << std::endl;
    std::cout << std::endl << std::endl << std::endl;

    return 0;
}
