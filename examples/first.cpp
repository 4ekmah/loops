#include "loops/loops.hpp"
#include <vector>
#include <iostream>


typedef int (*justcompiled_t)(const int* ptr, int64_t n,
                            int* minpos, int* maxpos);
void* gencode(loops::Context& ctx)
{
    using namespace loops;
    IReg ptr, n, minpos_addr, maxpos_addr;
    ctx.startfunc("foo", {&ptr, &n, &minpos_addr, &maxpos_addr});
    IReg i = ctx.const_(0);
    IReg minpos = ctx.const_(0);
    IReg maxpos = ctx.const_(0);
    IReg minval = load_<int>(ptr);
    IReg maxval = minval;
    IReg elemsize = ctx.const_((int)sizeof(int));
    n *= elemsize;
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
        i += elemsize;
    ctx.while_(i < n);
    minpos /= elemsize;
    maxpos /= elemsize;
    store_<int>(minpos_addr, minpos);
    store_<int>(maxpos_addr, maxpos);
    ctx.return_(ctx.const_(0));
    ctx.endfunc();
    void* res = ctx.getfunc("foo").ptr();
    return res;
}

//typedef int (*justcompiled_t)(const int* ptr, int64_t n, int* out);
//void* gencode(loops::Context& ctx)
//{
//    using namespace loops;
//    IReg ptr, n, out;
//    ctx.startfunc("foo", {&ptr, &n, &out});
//    IReg i = ctx.const_(0);
//    IReg sum = ctx.const_(0);
//    IReg elemsize = ctx.const_((int)sizeof(int));
//    n *= elemsize;
//    ctx.do_();
//        IReg x = load_<int>(ptr, i);
//        sum += x;
//        i += elemsize;
//    ctx.while_(i < n);
//    ctx.return_(sum);
//    ctx.endfunc();
//    void* res = ctx.getfunc("foo").ptr();
//    return res;
//}

//typedef int (*justcompiled_t)(int64_t a, int64_t b);
//void* gencode(loops::Context& ctx)
//{
//    using namespace loops;
//    IReg a, b;
//    ctx.startfunc("foo", {&a, &b});
//    ctx.if_( a > b );
//    b = a;
//    ctx.endif_();
//    ctx.return_(b);
//    ctx.endfunc();
//    void* res = ctx.getfunc("foo").ptr();
//    return res;
//}

int main(int argc, char** argv)
{
    {//Print bytecode dumps.
        std::cout<<"======--BYTECODE-LISTING--======="<<std::endl;
        loops::Context ctx(loops::Compiler::make_virtual_dump());
        std::cout<<*((std::string*)gencode(ctx))<<std::endl;
    }
    
    {//Print aarch64 dump
        std::cout<<"======--AARCH64--LISTING--======="<<std::endl;
        loops::Context ctx(loops::Compiler::make_aarch64_dump());
        std::cout<<*((std::string*)gencode(ctx))<<std::endl;
    }
    
    {//Print aarch64 hexdump
        std::cout<<"====--AARCH64-HEX-LISTING--======"<<std::endl;
        loops::Context ctx(loops::Compiler::make_aarch64_bin_dump());
        std::cout<<*((std::string*)gencode(ctx))<<std::endl;
    }
    
    {//Function usage
        std::cout<<"======---FUNCTION-USAGE---======="<<std::endl;
        loops::Context ctx(loops::Compiler::make_aarch64_compiler());
        justcompiled_t f = reinterpret_cast<justcompiled_t>(gencode(ctx));
        
        //minmax array
        std::vector<int> v = { 8, 2, -5, 7, 6 };
        int minpos = -1, maxpos = -1;
        int retval = f(&v[0], v.size(), &minpos, &maxpos);
        printf("retval=%d, minpos=%d, maxpos=%d\n", retval, (int)minpos, (int)maxpos);

//        //Returning minimum through return .
//        std::vector<int> v = { 8, 2, -5, 7, 6 }; //sum = 18
//        std::vector<int> vihod = { 0, 0, 0, 0, 0 }; //sum = 18
//        int sum = f(&v[0], v.size(), &vihod[0]);
//        printf("sum=%d\n", sum);

        //max between two
//        int64_t a = 3;
//        int64_t b = 4;
//        int64_t c = f(b, a);
//        printf("%d + %d = %d \n", (int)a, (int)b, (int)c);
    }
    
    return 0;
}
