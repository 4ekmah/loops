/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_TEST_MATH_HPP__
#define __LOOPS_TEST_MATH_HPP__

#include "tests.hpp"
#include <math.h>
#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "arm_neon.h"
#endif

namespace loops
{
PTEST_1(exponentiation_by_squaring, int64_t, _p, {
    int p = _p;
    IReg ptrA, n, ptrPow;
    STARTFUNC_(TESTNAME, &ptrA, &n, &ptrPow)
    {
        IReg res = CONST_(1);
        IReg offset = CONST_(0);
        IReg i = CONST_(0);
        WHILE_(i < n)
            {
                IReg x = load_<int>(ptrA, offset);
                store_<int>(ptrPow, pow(x, p));
                i += 1;
                offset += sizeof(int);
                ptrPow += sizeof(int);
            };
        RETURN_(0);
    }
});

PTESTexe_1(exponentiation_by_squaring, int64_t, _p, {
    typedef int64_t(*exponential_by_squaring_f)(const int* ptrA, int64_t n, int* ptrPow);
    exponential_by_squaring_f tested = reinterpret_cast<exponential_by_squaring_f>(EXEPTR);
    std::vector<int> X = {3,-5,7,-3,2,0,-1};
    int resArr[7];
    EXPECT_EQ(tested(&X[0], X.size(), resArr), (int64_t)(0));
    for (size_t n = 0; n < X.size(); n++) {
        int tmp = static_cast<int>(::pow(X[n], _p));
        EXPECT_EQ(resArr[n], tmp);
    }
});

PTESTfix_1(exponentiation_by_squaring, 0);
PTESTfix_1(exponentiation_by_squaring, 1);
PTESTfix_1(exponentiation_by_squaring, 9);

#if __LOOPS_ARCH == __LOOPS_AARCH64
PTEST_2(exponentiation_by_squaring_v, typename, _Tp, int64_t, _p, {
    IReg src, powdest, v_size;
    int p = _p;
    
    STARTFUNC_(TESTNAME, &src, &powdest, &v_size)
    {
        IReg offset  = CONST_(0);
        v_size *= sizeof(_Tp);
        WHILE_(offset < v_size)
        {
            VReg<_Tp> in = loadvec<_Tp>(src, offset);
            VReg<_Tp> res = pow(in, p);

            storevec(powdest, offset, res);
            offset += CTX.vbytes();
        }
        RETURN_(0);
    }
});

PTESTexe_2(exponentiation_by_squaring_v, typename, _Tp, int64_t, _p, {
    typedef int (*exponential_by_squaring_v)(const _Tp* src, _Tp* powdest, int64_t v_size);
    exponential_by_squaring_v tested = reinterpret_cast<exponential_by_squaring_v>(EXEPTR);
    std::vector<_Tp> v =   { 0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<_Tp> res_pow = { 0, 0, 0, 0, 0, 0, 0, 0 };
    EXPECT_EQ(tested(&v[0], &res_pow[0], v.size()), (int)(0));
    for (size_t vnum = 0; vnum < v.size(); vnum++) {
        _Tp tmp = static_cast<int>(::pow(v[vnum], _p));
        EXPECT_EQ(res_pow[vnum], tmp);
    }
});

PTESTfix_2(exponentiation_by_squaring_v, uint32_t, 0);
PTESTfix_2(exponentiation_by_squaring_v, uint32_t, 4);
PTESTfix_2(exponentiation_by_squaring_v, uint32_t, 9);
PTESTfix_2(exponentiation_by_squaring_v, int32_t, 0);
PTESTfix_2(exponentiation_by_squaring_v, int32_t, 4);
PTESTfix_2(exponentiation_by_squaring_v, int32_t, 9);
PTESTfix_2(exponentiation_by_squaring_v, float, 0);
PTESTfix_2(exponentiation_by_squaring_v, float, 4);
PTESTfix_2(exponentiation_by_squaring_v, float, 9);
PTESTfix_2(exponentiation_by_squaring_v, double, 0);
PTESTfix_2(exponentiation_by_squaring_v, double, 4);
PTESTfix_2(exponentiation_by_squaring_v, double, 9);

LTEST(exp_f32, {
    IReg dest, src, n;
    USE_CONTEXT_(CTX);
    STARTFUNC_(TESTNAME, &dest, &src, &n)
    {
        auto expc = expInit(CTX);
        IReg offset = CONST_(0);
        WHILE_(n > 0)
        {
            VReg<float> x = loadvec<float>(src,offset);
            storevec(dest, offset, exp(x, expc));
            offset += CTX.vbytes();
            n -= CTX.vlanes<float>();
        }
        RETURN_();
    }
    });

LTESTexe(exp_f32, {
    typedef void (*exp_f32_f)(float* dest, const float* src, int n);
    exp_f32_f tested = reinterpret_cast<exp_f32_f>(EXEPTR);
    const float ln15 = ::log(15);
    std::vector<float> src  = { 88.3762626647949f, -90, 1, 2, -15, 4.6, 23.1, -3, 13.7, -14.8, 18.2, 56, 22.12, 85.05, -12.6, -36.6,
                                9.9, -12.5, 44, 1.7, 64.2, 34.8, -15.7, 55.5, 69, -34, ln15, 9, 0.2, 62.13, -74.5, -18.1 };
    std::vector<float> dest(src.size(), 0);
    tested(&dest[0], &src[0], src.size());
    for (size_t i = 0; i < src.size(); i++ )
        EXPECT_NEAR((float)(dest[i]), (float)(::exp(src[i])), 1.e-39f);
    });

template<class T>
void printData(T* data, int H, int W, int C) {
    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < H * W; j++) {
            std::cout << data[i*H*W+j] << " ";
        }
        std::cout << std::endl;
    }
}

void MaxPool(float* data, int H, int W, int C, float* result, int H0, int W0, int kh, int kw)
{
    int* tab = (int*)alloca(kh*kw*sizeof(tab[0]));
    for (int i = 0; i < kh; i++)
        for (int j = 0; j < kw; j++)
            tab[i*kw + j] = i*W+j;
    for (int c = 0; c < C; c++)
    {
        for (int y = 0; y < H0; y++)
        {
            for (int x = 0; x < W0; x++)
            {
                const float* ptr = &data[(c*H*W)+(y*W+x)];
                float tmp = *ptr;
                for (int k = 1; k < kh*kw; k++)
                    tmp = std::max(tmp, ptr[tab[k]]);
                result[c*H0*W0+y*W0+x] = tmp;
            }
        }
    }
}

void MaxPool_unroll4(float* data, int H, int W, int C, float* result, int H0, int W0, int kh, int kw)
{
    int* tab = (int*)alloca(kh*kw*sizeof(tab[0]));
    for (int i = 0; i < kh; i++)
        for (int j = 0; j < kw; j++)
            tab[i*kw + j] = i*W+j;
    for (int c = 0; c < C; c++)
    {
        for (int y = 0; y < H0; y++)
        {
            for (int x = 0; x < W0; x++)
            {
                const float* ptr = &data[(c*H*W)+(y*W+x)];
                float tmp = *ptr;
                int k = 0;
                for (; k <= kh*kw - 4; k += 4) {
                    tmp = std::max(tmp, ptr[tab[k]]);
                    tmp = std::max(tmp, ptr[tab[k+1]]);
                    tmp = std::max(tmp, ptr[tab[k+2]]);
                    tmp = std::max(tmp, ptr[tab[k+3]]);
                }
                for (; k < kh*kw; k++)
                    tmp = std::max(tmp, ptr[tab[k]]);
                result[c*H0*W0+y*W0+x] = tmp;
            }
        }
    }
}

void MaxPool_vec_unroll4(float* data, int H, int W, int C, float* result, int H0, int W0, int kh, int kw)
{
    int* tab = (int*)alloca(kh*kw*sizeof(tab[0]));
    for (int i = 0; i < kh; i++)
        for (int j = 0; j < kw; j++)
            tab[i*kw + j] = i*W+j;
    for (int c = 0; c < C; c++)
    {
        for (int y = 0; y < H0; y++)
        {
            for (int x = 0; x < W0; x += 4)
            {
                if (x + 4 > W0)
                {
                    x = W0 - 4;
                }
                const float* ptr = &data[(c*H*W)+(y*W+x)];
                float32x4_t tmp = vld1q_f32(ptr);
                int k = 0;
                for (; k <= kh*kw - 4; k += 4) {
                    tmp = vmaxq_f32(tmp, vld1q_f32(ptr + tab[k]));
                    tmp = vmaxq_f32(tmp, vld1q_f32(ptr + tab[k+1]));
                    tmp = vmaxq_f32(tmp, vld1q_f32(ptr + tab[k+2]));
                    tmp = vmaxq_f32(tmp, vld1q_f32(ptr + tab[k+3]));
                }
                for (; k < kh*kw; k++)
                    tmp = vmaxq_f32(tmp, vld1q_f32(ptr + tab[k]));
                vst1q_f32(result + c*H0*W0+y*W0+x, tmp);
            }
        }
    }
}

PTEST_2(max_pool, int64_t, kh, int64_t, kw, {
    IReg data, H, W, C, result, H0, W0;
    USE_CONTEXT_(CTX);
    STARTFUNC_(TESTNAME, &data, &H, &W, &C, &result, &H0, &W0)
    {
        getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, 
            { 0, 1, 2, 3, 4, 5, 6, 7 }, 
            { 0, 1, 2, 3, 4, 5, 6, 7 }, 
            { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 }, 
            { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 });

        IReg offset = CONST_(0);
        IReg c = CONST_(0);
        IReg check = W0 * sizeof(float);
        std::vector<std::vector<IReg> > offstab(kh, std::vector<IReg>(kw, IReg()));
        for(int i = 0; i < kh; i++ ) 
            for(int j = 0; j < kw; j++)
                offstab[i][j].copyidx(IReg((CONST_(i)*W+CONST_(j)) * sizeof(float)));
        WHILE_(c < C)
        {
            IReg y = CONST_(0);
            IReg result_ = result + c * W0 * H0 * sizeof(float);
            IReg data_ = data + c * W * H * sizeof(float);
            WHILE_(y < H0) 
            {
                IReg x = CONST_(0);
                IReg next_x = CONST_(CTX.vbytes());
                WHILE_(x < check) 
                {
                    IF_(next_x > check)
                    {
                        IReg corr = (next_x - check);
                        x -= corr;
                        next_x -= corr;
                    }

                    IReg data__ = data_ + x;
                    IReg result__ = result_ + x;

                    VReg<float> res = max(loadvec<float>(data__, 0), ext(loadvec<float>(data__, 0), loadvec<float>(data__, 4 * sizeof(float)), 1));
                    
                    for(int j = 2; j < kw; j++)
                        if (j % 4 == 0) {
                            res = max(res, loadvec<float>(data__, j * sizeof(float)));
                        }
                        else {
                            res = max(res, ext(loadvec<float>(data__, (j / 4 * 4) * sizeof(float)), loadvec<float>(data__, (j / 4 * 4 + 4) * sizeof(float)), j % 4));
                        }

                    for(int i = 1; i < kh; i++ ){
                        data__ += W * sizeof(float);
                        for(int j = 0; j < kw; j++)
                            if (j % 4 == 0) {
                                res = max(res, loadvec<float>(data__, j * sizeof(float)));
                            }
                            else {
                                res = max(res, ext(loadvec<float>(data__, (j / 4 * 4) * sizeof(float)), loadvec<float>(data__, (j / 4 * 4 + 4) * sizeof(float)), j % 4));
                            }
                    }

                    storevec(result__, res);
                    x = next_x;
                    next_x += CTX.vbytes();
                }
                y += 1;
                result_ += W0 * sizeof(float);
                data_ += W * sizeof(float);
            }
            c += 1;
        }
        RETURN_(0);
    }
    });


PTESTexe_2(max_pool, int64_t, kh, int64_t, kw, {
    typedef void (*max_pool_f)(float* data, int H, int W, int C, float* result, int H0, int W0);
    max_pool_f tested = reinterpret_cast<max_pool_f>(EXEPTR);

    const int TESTITERATIONS = 30;
    const int W = 200;
    const int H = 200;
    const int C = 32;
    const int H0 = H-kh+1;
    const int W0 = W-kw+1;
    std::vector<float> inp(W*H*C, 0.f), out0(W0*H0*C, 0.f), out1(W0*H0*C, 0.f), out2(W0*H0*C, 0.f), out3(W0*H0*C, 0.f);

    for (int i = 0 ; i < C*H*W ; i++)
        inp[i] = rand() % 1000;

    Timer t0, t1, t2, t3;
    for(int testiter = 0; testiter < TESTITERATIONS; testiter++)
    {
        t0.start();
        tested(&inp[0], H, W, C, &out0[0], H0, W0);
        t0.stop();
        t1.start();
        // MaxPool(&inp[0], H, W, C, &out1[0], H0, W0, kh, kw);
        // t1.stop();
        // t2.start();
        // MaxPool_unroll4(&inp[0], H, W, C, &out2[0], H0, W0, kh, kw);
        // t2.stop();
        // t3.start();
        MaxPool_vec_unroll4(&inp[0], H, W, C, &out3[0], H0, W0, kh, kw);
        t3.stop();
    }
    // printf("maxpool %dx%d. ref time: %s, ref time (unrolled): %s, ref time (vec-ed & unrolled): %s, loops time: %s\n", (int)kh, (int)kw, t1.str(), t2.str(), t3.str(), t0.str());
    printf("maxpool %dx%d. ref time (vec-ed & unrolled): %s, loops time: %s\n", (int)kh, (int)kw, t3.str(), t0.str());
    for(int i = 0; i < H0*W0*C; i++)
    {
        // EXPECT_EQ(out0[i], out1[i]);
        // EXPECT_EQ(out0[i], out2[i]);
        EXPECT_EQ(out0[i], out3[i]);
    }
    });

PTESTfix_2(max_pool, 3, 3);
PTESTfix_2(max_pool, 5, 5);
PTESTfix_2(max_pool, 9, 9);
PTESTfix_2(max_pool, 13, 13);

#endif //__LOOPS_ARCH == __LOOPS_AARCH64

};
#endif//__LOOPS_TEST_MATH_HPP__
