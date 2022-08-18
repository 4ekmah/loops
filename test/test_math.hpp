/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_TEST_MATH_HPP__
#define __LOOPS_TEST_MATH_HPP__

#include "tests.hpp"
#include <math.h>

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

void MaxPool(float* data, int H, int W, int C, float* result, int H0, int W0, int kh, int kw){
    for (int c = 0; c < C; c++)
    {
        for (int y = 0; y < H0; y++) {
            for (int x = 0; x < W0; x++) {
                float tmp = data[(c*H*W)+(y*W+x)];
                for (int hi = 0; hi < kh; hi++)
                    for (int hj = 0; hj < kw; hj++)
                    {
                        tmp = std::max(tmp, data[(c*H*W)+(y*W+x)+(hi*W+hj)]);
                    }
                
                result[c*H0*W0+y*W0+x] = tmp;
            }
        }
    }
}

PTEST_2(max_pool, int64_t, kh, int64_t, kw, {
    IReg data, H, W, C, result, H0, W0;
    USE_CONTEXT_(CTX);
    STARTFUNC_(TESTNAME, &data, &H, &W, &C, &result, &H0, &W0)
    {
        getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 0, 1, 2, 3, 4, 5, 6, 7 }, { 0, 1, 2, 3, 4, 5, 6, 7 }, {}, { 18, 19, 20, 21, 22 });
        IReg offset = CONST_(0);
        IReg c = CONST_(0);
        IReg check = W0 * sizeof(float);
        // std::vector<std::vector<IReg> > offstab(kh, std::vector<IReg>(kw, IReg()));
        // for(int i = 0; i < kh; i++ ) 
        //     for(int j = 0; j < kh; j++)
        //         offstab[i][j] = IReg((CONST_(i)*W+CONST_(j)) * sizeof(float);  
        IReg offstab_0_0 = (0*W+0)* sizeof(float); 
        IReg offstab_0_1 = (0*W+1)* sizeof(float); 
        IReg offstab_0_2 = (0*W+2)* sizeof(float); 
        IReg offstab_1_0 = (1*W+0)* sizeof(float); 
        IReg offstab_1_1 = (1*W+1)* sizeof(float); 
        IReg offstab_1_2 = (1*W+2)* sizeof(float); 
        IReg offstab_2_0 = (2*W+0)* sizeof(float); 
        IReg offstab_2_1 = (2*W+1)* sizeof(float); 
        IReg offstab_2_2 = (2*W+2)* sizeof(float); 
        WHILE_(c < C)
        {
            IReg y = CONST_(0);
            WHILE_(y < H0) {
                IReg x = CONST_(0);
                IReg next_x = x + CTX.vbytes();
                // IReg next_data += CTX.vbytes();
                WHILE_(x < check) {
                    IReg border = W * sizeof(float);
                    IF_(next_x > border){
                        x = x - (next_x - border);
                        data = data - (next_x - border);
                    }

                    IReg base_offset = (c*H*W* sizeof(float))+(y*W* sizeof(float)+x);
                    VReg<float> tmp1 = max(loadvec<float>(data, offstab_0_0 ),
                                           loadvec<float>(data, offstab_0_1 ));
                    VReg<float> tmp2 = max(tmp1, loadvec<float>(data, offstab_0_2));
                    VReg<float> tmp3 = max(tmp2, loadvec<float>(data, offstab_1_0));
                    VReg<float> tmp4 = max(tmp3, loadvec<float>(data, offstab_1_1));
                    VReg<float> tmp5 = max(tmp4, loadvec<float>(data, offstab_1_2));
                    VReg<float> tmp6 = max(tmp5, loadvec<float>(data, offstab_2_0));
                    VReg<float> tmp7 = max(tmp6, loadvec<float>(data, offstab_2_1));
                    VReg<float> tmp8 = max(tmp7, loadvec<float>(data, offstab_2_2));

                    storevec(result, (c*H0*W0* sizeof(float)+y*W0* sizeof(float)+x), tmp8);
                    x = x + CTX.vbytes();
                    data += CTX.vbytes();
                }
                y = y + 1;
                // data += sizeof(float) * W;
            }
            c = c + 1;
            // data += sizeof(float) * W * H;
        }
        RETURN_();
    }
    });


PTESTexe_2(max_pool, int64_t, kh, int64_t, kw, {
    typedef void (*max_pool_f)(float* data, int H, int W, int C, float* result, int H0, int W0);
    max_pool_f tested = reinterpret_cast<max_pool_f>(EXEPTR);

    int W = 9;
    int H = 9;
    int C = 3;
    int H_W = W*H;
    int H0 = H-kh+1;
    int W0 = W-kw+1;

    std::vector<float> storage(W*H*C, 0);
    float* c = &(storage[0]);

    for (int i = 0 ; i < C ; i++) {
        float* HW = c + i * H_W;
        for (int j = 0; j < H_W; j++) {
            HW[j] = rand() % 1000;
        }
    }
    printData(c, H, W, C);

    std::vector<float> result(H0*W0*C, 0);
    float* c_out = &(result[0]);

    MaxPool(c, H, W, C, c_out, H0, W0, kh, kw);
    printData(c_out, H0, W0, C);

    std::vector<float> result_vec(H0*W0*C, 0);
    float* c_out_vec = &(result_vec[0]);

    tested(c, H, W, C, c_out_vec, H0, W0);
    printData(c_out_vec, H0, W0, C);

    for(int channel = 0; channel < C; channel++) 
        for(int row = 0; row<H0; row++) 
            for(int col = 0; col<H0 ; col++)
                // if(c_out[channel* W0 * H0 + row*W0 + col] != c_out_vec[channel* W0 * H0 + row*W0 + col])
                //     std::cout<<"(Channel:"<< channel << ", Row:" << row << ", Col:" << col <<")|" << c_out[channel* W0 * H0 + row*W0 + col]<<"!="<<c_out_vec[channel* W0 * H0 + row*W0 + col]<<std::endl;
                EXPECT_EQ(c_out[channel* W0 * H0 + row*W0 + col], c_out_vec[channel* W0 * H0 + row*W0 + col]);

    });

PTESTfix_2(max_pool, 3, 3);

#endif //__LOOPS_ARCH == __LOOPS_AARCH64

};
#endif//__LOOPS_TEST_MATH_HPP__
