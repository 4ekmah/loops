/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#include "loops/loops.hpp"
#include <algorithm>
#include <vector>
#include <iostream>
#include "../src/func_impl.hpp"
#include "arm_neon.h"


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

typedef int (*minmaxpos_t)(float* data, int H, int W, int C, float* result, int H0, int W0);
// typedef int (*minmaxpos_t)(const int* ptr, int64_t n, int* minpos, int* maxpos);
loops::Func genminmaxloc(loops::Context& CTX)
{
    using namespace loops;
    // IReg ptr, n, minpos_addr, maxpos_addr;
    // USE_CONTEXT_(CTX);
    // STARTFUNC_("minmaxloc", &ptr, &n, &minpos_addr, &maxpos_addr)
    // {
    //     IReg i = CONST_(0);
    //     IReg minpos = CONST_(0);
    //     IReg maxpos = CONST_(0);
    //     IReg minval = load_<int>(ptr);
    //     IReg maxval = minval;
    //     n *= sizeof(int);
    //     WHILE_(i < n)
    //     {
    //         IReg x = load_<int>(ptr, i);
    //         IF_(x < minval)
    //         {
    //             minval = x;
    //             minpos = i;
    //         }
    //         IF_(x > maxval)
    //         {
    //             maxval = x;
    //             maxpos = i;
    //         }
    //         i += sizeof(int);
    //     }
    //     IReg elemsize = CONST_(sizeof(int));
    //     minpos /= elemsize;
    //     maxpos /= elemsize;
    //     store_<int>(minpos_addr, minpos);
    //     store_<int>(maxpos_addr, maxpos);
    //     RETURN_(0);
    // }
    // return CTX.getFunc("minmaxloc");
    IReg data, H, W, C, result, H0, W0;
    USE_CONTEXT_(CTX);
    STARTFUNC_("minmaxloc", &data, &H, &W, &C, &result, &H0, &W0)
    {
        int64_t kh = 3;
        int64_t kw = 3;
        getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, 
            { 0, 1, 2, 3, 4, 5, 6, 7 }, 
            { 0, 1, 2, 3, 4, 5, 6, 7 }, 
            { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 }, 
            { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 });

        IReg offset = CONST_(0);
        IReg c = CONST_(0);
        IReg check = W0 * sizeof(float);
        // std::vector<std::vector<IReg> > offstab(kh, std::vector<IReg>(kw, IReg()));
        // for(int i = 0; i < kh; i++ ) 
        //     for(int j = 0; j < kw; j++)
        //         offstab[i][j].rawcopy(IReg((CONST_(i)*W+CONST_(j)) * sizeof(float)));
        std::vector<IReg> offstab(kw, IReg());
        for(int i = 0; i < kw; i++ )
            offstab[i].rawcopy(IReg(CONST_(i) * sizeof(float)));
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

                    // VReg<float> res = max(loadvec<float>(data__, offstab[0][0] ), loadvec<float>(data__, offstab[0][1] ));
                    // for(int i = 0; i < kh; i++ )
                    //     for(int j = (i == 0 ? 2 : 0); j < kw; j++)
                    //         res = max(res, loadvec<float>(data__, offstab[i][j] ));

                    // VReg<float> res = max(loadvec<float>(data__, 0), loadvec<float>(data__, offstab[1] ));
                    // VReg<float> temp = ext(loadvec<float>(data__, 0), loadvec<float>(data__, 16), 1);
                    VReg<float> res = max(loadvec<float>(data__, 0), ext(loadvec<float>(data__, 0), loadvec<float>(data__, 4 * sizeof(float)), 1));
                    
                    for(int j = 2; j < kw; j++)
                        if (j % 4 == 0) {
                            res = max(res, loadvec<float>(data__, j * sizeof(float)));
                        }
                        else {
                            // res = max(res, loadvec<float>(data__, offstab[j] ));
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
        // RETURN_(0);
        return CTX.getFunc("minmaxloc");
    }
}

int main(int argc, char** argv)
{
    loops::Context CTX;
    loops::Func mmlfunc = genminmaxloc(CTX);

    std::cout << "--------MINMAXLOCEXAMPLE---------" << std::endl;
    std::cout << "======--BYTECODE-LISTING--=======" << std::endl;
    mmlfunc.printBytecode(std::cout);
    std::string platform = CTX.getPlatformName();
    std::transform(platform.begin(), platform.end(), platform.begin(), ::toupper);
    std::cout << "======--" << platform << "--LISTING--====== = " << std::endl;
    mmlfunc.printAssembly(std::cout);
    std::cout << "======--FUNCTION-OUTPUT---=======" << std::endl;

    const int W = 200;
    const int H = 200;
    const int C = 32;
    int64_t kh = 3;
    int64_t kw = 3;
    const int H0 = H-kh+1;
    const int W0 = W-kw+1;
    std::vector<float> inp(W*H*C, 0.f), out0(W0*H0*C, 0.f), out3(W0*H0*C, 0.f);

    for (int i = 0 ; i < C*H*W ; i++)
        inp[i] = rand() % 1000;

    minmaxpos_t f = reinterpret_cast<minmaxpos_t>(mmlfunc.ptr());
    // std::vector<int> v = { 8, 2, -5, 7, 6 };
    // int minpos = -1, maxpos = -1;
    // int retval = f(&v[0], v.size(), &minpos, &maxpos);

    int retval = f(&inp[0], H, W, C, &out0[0], H0, W0);
    MaxPool_vec_unroll4(&inp[0], H, W, C, &out3[0], H0, W0, kh, kw);

    for(int i = 0; i < H0*W0*C; i++)
    {
        // EXPECT_EQ(out0[i], out1[i]);
        // EXPECT_EQ(out0[i], out2[i]);
        if(out0[i] != out3[i]){
            std::cout << "ERROR!!!" << std::endl;
        }
    }

    std::cout << "retval=" << retval<< ", kw="<< (int)kw <<", kh="<< (int)kh << std::endl;
    std::cout << std::endl << std::endl << std::endl;

    return 0;
}
