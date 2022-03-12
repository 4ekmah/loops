#include "loops/loops.hpp"
#include <vector>
#include <iostream>


typedef int (*minmaxfunc_t)(const int* ptr, int64_t n,
                            int* minpos, int* maxpos);

namespace loops
{
//minmaxfunc_t gencode(Context& ctx)
std::string gencode(Context& ctx)
{
    IReg ptr, n, minpos_addr, maxpos_addr;
    ctx.startfunc("foo", {&ptr, &n, &minpos_addr, &maxpos_addr});
    IReg i = ctx.const_(0), minpos = ctx.const_(0), maxpos = ctx.const_(0), minval = load_<int>(ptr), maxval = minval;
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
        i += ctx.const_((int)sizeof(int));
    ctx.while_(i < n);
    store_<int>(minpos_addr, minpos);
    store_<int>(maxpos_addr, maxpos);
    ctx.return_(ctx.const_(0));
    ctx.endfunc();
    //return (minmaxfunc_t)ctx.getfunc("foo").ptr();
    return *((std::string*)(ctx.getfunc("foo").ptr()));
}
}

int main(int argc, char** argv)
{
    loops::Context ctx(loops::Compiler::make_virtual_dump());
    std::string f = gencode(ctx);
    std::cout << f << std::endl;
    //minmaxfunc_t f = gencode(ctx);
    //std::vector<int> v = { 1, 2, -5, 7, 8 };
    //int minpos = -1, maxpos = -1;
    //int retval = f(&v[0], (int64_t)v.size(), &minpos, &maxpos);
    //printf("retval=%d, minpos=%d, maxpos=%d\n", retval, (int)minpos, (int)maxpos);

    return 0;
}
