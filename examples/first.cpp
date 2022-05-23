#include "loops/loops.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

typedef int (*justcompiled_t)(const int* ptr, int64_t n,
                            int* minpos, int* maxpos);
loops::Func gencode(loops::Context& ctx)
{
    using namespace loops;
    IReg ptr, n, minpos_addr, maxpos_addr;
    ctx.startFunc("foo", {&ptr, &n, &minpos_addr, &maxpos_addr});
    IReg i = ctx.const_(0);
    IReg minpos = ctx.const_(0);
    IReg maxpos = ctx.const_(0);
    IReg minval = load_<int>(ptr);
    IReg maxval = minval;
    n *= sizeof(int);
    ctx.do_();
        IReg x = load_<int>(ptr, i);
        ctx.if_(x < minval);
            minval = x;
            minpos = i;
        ctx.endif_();
        ctx.if_(x > maxval);
            maxval = x;
            maxpos = i;
        ctx.endif_();
        i += sizeof(int);
    ctx.while_(i < n);
    IReg elemsize = ctx.const_(sizeof(int));
    minpos /= elemsize;
    maxpos /= elemsize;
    store_<int>(minpos_addr, minpos);
    store_<int>(maxpos_addr, maxpos);
    ctx.return_(0);
    ctx.endFunc();
    return ctx.getFunc("foo");
}

int main(int argc, char** argv)
{
    {//Function usage
        loops::Context ctx;
        loops::Func lfunc = gencode(ctx);
        std::cout << "======--BYTECODE-LISTING--=======" << std::endl;
        lfunc.printBytecode(std::cout);
        std::string platform = ctx.getPlatformName();
        std::transform(platform.begin(), platform.end(), platform.begin(), ::toupper);
        std::cout << "======--"<< platform <<"--LISTING--====== = " << std::endl;
        lfunc.printAssembly(std::cout);
        std::cout << "======---FUNCTION-USAGE---=======" << std::endl;
        justcompiled_t f = reinterpret_cast<justcompiled_t>(lfunc.ptr());

        //minmax array
        std::vector<int> v = { 8, 2, -5, 7, 6 };
        int minpos = -1, maxpos = -1;
        int retval = f(&v[0], v.size(), &minpos, &maxpos);
        printf("retval=%d, minpos=%d, maxpos=%d\n", retval, (int)minpos, (int)maxpos);
    }
    
    return 0;
}
