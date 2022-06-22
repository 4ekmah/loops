/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_TEST_AARCH64_HPP__
#define __LOOPS_TEST_AARCH64_HPP__
#if defined(__APPLE__)
#include "loops/loops.hpp"
#include "tests.hpp"
#include <iostream>

namespace loops
{
    LTEST(ten_args_to_sum, { //There we are testing stack parameter passing.
        IReg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
        CTX.startFunc(TESTNAME, {&a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9});
        IReg res = a0 * 1;
        res += a1 * 2;
        res += a2 * 3;
        res += a3 * 4;
        res += a4 * 5;
        res += a5 * 6;
        res += a6 * 7;
        res += a7 * 8;
        res += a8 * 3;
        res += a9 * 2;
        CTX.return_(res);
        getImpl(&CTX)->getRegisterAllocator()->getRegisterPool().overrideRegisterSet(makeBitmask64({ 0, 1, 2, 3, 4, 5, 6, 7 }), makeBitmask64({ 0, 1, 2, 3, 4, 5, 6, 7 }), makeBitmask64({}), makeBitmask64({ 18, 19, 20, 21, 22 }));
        CTX.endFunc();
        getImpl(&CTX)->getRegisterAllocator()->getRegisterPool().overrideRegisterSet(0, 0, 0, 0);
        });
    LTESTexe(ten_args_to_sum, {
        typedef int64_t(*ten_args_to_sum_f)(int64_t a0, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9);
        ten_args_to_sum_f tested = reinterpret_cast<ten_args_to_sum_f>(EXEPTR);
        std::vector<int> v = { 1,1,1,1,1,1,1,1,3,5 };
        TEST_EQ(tested(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9]),(int64_t)(55));
        });

LTESTcomposer(instruction_set_test, {
    FuncImpl* _f = getImpl(getImpl(&CTX)->getCurrentFunc());
    IReg x0 = iregHid(0,_f);
    IReg w0 = iregHid(0,_f);
    IReg x15 = iregHid(15,_f);
    IReg w15 = iregHid(15,_f);

    store_<uint64_t>(x0, 256, x0);
    store_<int64_t>(x0, 256, x0);
    store_<uint64_t>(x0, x0, x0);
    store_<int64_t>(x0, x0, x0);

    store_<uint32_t>(x0, 256, x0);
    store_<int32_t>(x0, 256, x0);
    store_<uint32_t>(x0, x0, x0);
    store_<int32_t>(x0, x0, x0);

    store_<uint16_t>(x0, 256, x0);
    store_<int16_t>(x0, 256, x0);
    store_<uint16_t>(x0, x0, x0);
    store_<int16_t>(x0, x0, x0);

    store_<uint8_t>(x0, 256, x0);
    store_<int8_t>(x0, 256, x0);
    store_<uint8_t>(x0, x0, x0);
    store_<int8_t>(x0, x0, x0);

    load2<uint64_t>(x0, x0);
    load2<uint64_t>(x0 , x0 , 256);
    load2<uint64_t>(x15, x0 , 256);
    load2<uint64_t>(x0 , x15, 256);
    load2<uint64_t>(x0 , x0 , x0 );
    load2<uint64_t>(x15, x0 , x0 );
    load2<uint64_t>(x0 , x15, x0 );
    load2<uint64_t>(x0 , x0 , x15);

    load2<int64_t>(x0, x0);
    load2<int64_t>(x0 , x0 , 256);
    load2<int64_t>(x15, x0 , 256);
    load2<int64_t>(x0 , x15, 256);
    load2<int64_t>(x0 , x0 , x0 );
    load2<int64_t>(x15, x0 , x0 );
    load2<int64_t>(x0 , x15, x0 );
    load2<int64_t>(x0 , x0 , x15);

    load2<uint32_t>(x0, x0);
    load2<uint32_t>(x0 , x0 , 256);
    load2<uint32_t>(x15, x0 , 256);
    load2<uint32_t>(x0 , x15, 256);
    load2<uint32_t>(x0 , x0 , x0 );
    load2<uint32_t>(x15, x0 , x0 );
    load2<uint32_t>(x0 , x15, x0 );
    load2<uint32_t>(x0 , x0 , x15);

    load2<int32_t>(x0, x0);
    load2<int32_t>(x0 , x0 , 256);
    load2<int32_t>(x15, x0 , 256);
    load2<int32_t>(x0 , x15, 256);
    load2<int32_t>(x0 , x0 , x0 );
    load2<int32_t>(x15, x0 , x0 );
    load2<int32_t>(x0 , x15, x0 );
    load2<int32_t>(x0 , x0 , x15);

    load2<uint16_t>(x0, x0);
    load2<uint16_t>(x0 , x0 , 256);
    load2<uint16_t>(x15, x0 , 256);
    load2<uint16_t>(x0 , x15, 256);
    load2<uint16_t>(x0 , x0 , x0 );
    load2<uint16_t>(x15, x0 , x0 );
    load2<uint16_t>(x0 , x15, x0 );
    load2<uint16_t>(x0 , x0 , x15);

    load2<int16_t>(x0, x0);
    load2<int16_t>(x0 , x0 , 256);
    load2<int16_t>(x15, x0 , 256);
    load2<int16_t>(x0 , x15, 256);
    load2<int16_t>(x0 , x0 , x0 );
    load2<int16_t>(x15, x0 , x0 );
    load2<int16_t>(x0 , x15, x0 );
    load2<int16_t>(x0 , x0 , x15);

    load2<uint8_t>(x0, x0);
    load2<uint8_t>(x0 , x0 , 256);
    load2<uint8_t>(x15, x0 , 256);
    load2<uint8_t>(x0 , x15, 256);
    load2<uint8_t>(x0 , x0 , x0 );
    load2<uint8_t>(x15, x0 , x0 );
    load2<uint8_t>(x0 , x15, x0 );
    load2<uint8_t>(x0 , x0 , x15);

    load2<int8_t>(x0, x0);
    load2<int8_t>(x0 , x0 , 256);
    load2<int8_t>(x15, x0 , 256);
    load2<int8_t>(x0 , x15, 256);
    load2<int8_t>(x0 , x0 , x0 );
    load2<int8_t>(x15, x0 , x0 );
    load2<int8_t>(x0 , x15, x0 );
    load2<int8_t>(x0 , x0 , x15);

    newiopNoret(OP_SELECT, { x0, IC_EQ, x0, x0 });
    newiopNoret(OP_SELECT, { x0, IC_NE, x0, x0 });
    newiopNoret(OP_SELECT, { x0, IC_LT, x0, x0 });
    newiopNoret(OP_SELECT, { x0, IC_GT, x0, x0 });
    newiopNoret(OP_SELECT, { x0, IC_LE, x0, x0 });
    newiopNoret(OP_SELECT, { x0, IC_GE, x0, x0 });
    newiopNoret(OP_SELECT, { x0, IC_S,  x0, x0 });
    newiopNoret(OP_SELECT, { x0, IC_NS, x0, x0 });

    newiopNoret(OP_SELECT, { x15, IC_EQ, x0, x0 });
    newiopNoret(OP_SELECT, { x0, IC_EQ, x15, x0 });
    newiopNoret(OP_SELECT, { x0, IC_EQ, x0, x15 });

    newiopNoret(OP_CINC, { x0 ,  x0, IC_EQ });
    newiopNoret(OP_CINC, { x0 ,  x0, IC_NE });
    newiopNoret(OP_CINC, { x0 ,  x0, IC_LT });
    newiopNoret(OP_CINC, { x0 ,  x0, IC_GT });
    newiopNoret(OP_CINC, { x0 ,  x0, IC_LE });
    newiopNoret(OP_CINC, { x0 ,  x0, IC_GE });
    newiopNoret(OP_CINC, { x0 ,  x0, IC_S  });
    newiopNoret(OP_CINC, { x0 ,  x0, IC_NS });
    newiopNoret(OP_CINC, { x15,  x0, IC_EQ });
    newiopNoret(OP_CINC, { x0 , x15, IC_EQ });

    newiopNoret(OP_CNEG, { x0 ,  x0, IC_EQ });
    newiopNoret(OP_CNEG, { x0 ,  x0, IC_NE });
    newiopNoret(OP_CNEG, { x0 ,  x0, IC_LT });
    newiopNoret(OP_CNEG, { x0 ,  x0, IC_GT });
    newiopNoret(OP_CNEG, { x0 ,  x0, IC_LE });
    newiopNoret(OP_CNEG, { x0 ,  x0, IC_GE });
    newiopNoret(OP_CNEG, { x0 ,  x0, IC_S  });
    newiopNoret(OP_CNEG, { x0 ,  x0, IC_NS });
    newiopNoret(OP_CNEG, { x15,  x0, IC_EQ });
    newiopNoret(OP_CNEG, { x0 , x15, IC_EQ });

    newiopNoret(OP_SAR, { x0,  x0, argIImm(63, _f) });
    newiopNoret(OP_SAR, { x15, x0, argIImm(63, _f) });
    newiopNoret(OP_SAR, { x0, x15, argIImm(63, _f) });

    newiopNoret(OP_SAR, { x0 ,  x0,  x0 });
    newiopNoret(OP_SAR, { x15,  x0,  x0 });
    newiopNoret(OP_SAR, { x0 , x15,  x0 });
    newiopNoret(OP_SAR, { x0 ,  x0, x15 });

    newiopNoret(OP_SHR, { x0,  x0, argIImm(63, _f) });
    newiopNoret(OP_SHR, { x15, x0, argIImm(63, _f) });
    newiopNoret(OP_SHR, { x0, x15, argIImm(63, _f) });

    newiopNoret(OP_SHR, { x0 ,  x0,  x0 });
    newiopNoret(OP_SHR, { x15,  x0,  x0 });
    newiopNoret(OP_SHR, { x0 , x15,  x0 });
    newiopNoret(OP_SHR, { x0 ,  x0, x15 });

    newiopNoret(OP_SHL, { x0,  x0, argIImm(63, _f) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(31, _f) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(15, _f) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(7, _f) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(1, _f) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(0, _f) });

    newiopNoret(OP_SHL, { x15, x0, argIImm(63, _f) });
    newiopNoret(OP_SHL, { x0, x15, argIImm(63, _f) });

    newiopNoret(OP_SHL, { x0 ,  x0,  x0 });
    newiopNoret(OP_SHL, { x15,  x0,  x0 });
    newiopNoret(OP_SHL, { x0 , x15,  x0 });
    newiopNoret(OP_SHL, { x0 ,  x0, x15 });

    newiopNoret(OP_AND, { x0 ,   x0,  x0 });
    newiopNoret(OP_AND, { x15,   x0,  x0 });
    newiopNoret(OP_AND, { x0 ,  x15,  x0 });
    newiopNoret(OP_AND, { x0 ,  x0 , x15 });
    newiopNoret(OP_AND, { x0 ,  x0 , argIImm(0xFF, _f) });
    newiopNoret(OP_AND, { x0 ,  x0 , argIImm(0xFF00, _f) });
    newiopNoret(OP_AND, { x0 ,  x0 , argIImm(0xFF00000000000000, _f) });
    newiopNoret(OP_AND, { x15,  x0 , argIImm(0xFF, _f) });
    newiopNoret(OP_AND, { x0,  x15 , argIImm(0xFF, _f) });

    newiopNoret(OP_OR, { x0 ,   x0,  x0 });
    newiopNoret(OP_OR, { x15,   x0,  x0 });
    newiopNoret(OP_OR, { x0 ,  x15,  x0 });
    newiopNoret(OP_OR, { x0 ,  x0 , x15 });
    newiopNoret(OP_OR, { x0 ,  x0 , argIImm(0xFF, _f) });
    newiopNoret(OP_OR, { x0 ,  x0 , argIImm(0xFF00, _f) });
    newiopNoret(OP_OR, { x0 ,  x0 , argIImm(0xFF00000000000000, _f) });
    newiopNoret(OP_OR, { x15,  x0 , argIImm(0xFF, _f) });
    newiopNoret(OP_OR, { x0,  x15 , argIImm(0xFF, _f) });

    newiopNoret(OP_XOR, { x0 ,   x0,  x0 });
    newiopNoret(OP_XOR, { x15,   x0,  x0 });
    newiopNoret(OP_XOR, { x0 ,  x15,  x0 });
    newiopNoret(OP_XOR, { x0 ,  x0 , x15 });
    newiopNoret(OP_XOR, { x0 ,  x0 , argIImm(0xFF, _f) });
    newiopNoret(OP_XOR, { x0 ,  x0 , argIImm(0xFF00, _f) });
    newiopNoret(OP_XOR, { x0 ,  x0 , argIImm(0xFF00000000000000, _f) });
    newiopNoret(OP_XOR, { x15,  x0 , argIImm(0xFF, _f) });
    newiopNoret(OP_XOR, { x0,  x15 , argIImm(0xFF, _f) });

    newiopNoret(OP_NOT, { x0 , x0 });
    newiopNoret(OP_NOT, { x15 , x0});
    newiopNoret(OP_NOT, { x0 , x15});

    newiopNoret(OP_NEG, { x0 , x0 });
    newiopNoret(OP_NEG, { x15 , x0});
    newiopNoret(OP_NEG, { x0 , x15});

});

};
#endif
#endif//__LOOPS_TEST_AARCH64_HPP__
