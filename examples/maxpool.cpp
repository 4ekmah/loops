/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#include "loops/loops.hpp"
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../src/func_impl.hpp"
#include "../test/tests.hpp"
#include "arm_neon.h"


template<class T>
void printData(T* data, int H, int W) 
{
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            std::cout << std::setw(6) << data[i*W+j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
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

typedef int (*minmaxpos_t)(float* data, int H, int W, int C, float* result, int H0, int W0);
// typedef int (*minmaxpos_t)(const int* ptr, int64_t n, int* minpos, int* maxpos);
loops::Func genminmaxloc(loops::Context& CTX, int64_t kh, int64_t kw)
{
    using namespace loops;

    IReg data, H, W, C, result, H0, W0;
    USE_CONTEXT_(CTX);
    STARTFUNC_("minmaxloc", &data, &H, &W, &C, &result, &H0, &W0)
    {
        int horVecsPerOut = (kw + 3)/4 + ((kw + 3)%4?1:0);

        getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, 
            { 0, 1, 2, 3, 4, 5, 6, 7 }, 
            { 0, 1, 2, 3, 4, 5, 6, 7 }, 
            { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 }, 
            { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 });

        IReg offset = CONST_(0);
        IReg c = CONST_(0);
        IReg check = W0 * sizeof(float);

        std::vector<IReg> offstab(kw, IReg());
        for(int i = 0; i < kw; i++ )
            offstab[i].rawcopy(IReg(CONST_(i) * sizeof(float)));
        IReg H0x3 = H0 - H0 % 3;
        IReg stride = W * sizeof(float);
        IReg stride0 = W0 * sizeof(float);
        WHILE_(c < C)
        {
            IReg y = CONST_(0);
            IReg result_ = result + c * W0 * H0 * sizeof(float);
            IReg data_ = data + c * W * H * sizeof(float);

            WHILE_(y < H0x3) 
            {
                IReg x = CONST_(0);

                std::vector<std::vector<VReg<float> > > vertMaxes(3, std::vector<VReg<float>>(horVecsPerOut, VReg<float>()));
                for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
                {
                    IReg vertMaxesPtr = data_ + vRegNum * CTX.vbytes();
                    VReg<float> temp;
                    vertMaxes[0][vRegNum].rawcopy(loadvec<float>(vertMaxesPtr));
                    vertMaxes[1][vRegNum].rawcopy(loadvec<float>(vertMaxesPtr, stride));
                    vertMaxes[0][vRegNum] = max(vertMaxes[0][vRegNum], vertMaxes[1][vRegNum]);
                    
                    if (kh == 3)
                    {
                        vertMaxes[2][vRegNum].rawcopy(loadvec<float>(vertMaxesPtr, stride * 2));
                        temp.rawcopy(loadvec<float>(vertMaxesPtr, stride * 3));
                    }
                    if (kh == 4)
                    {
                        vertMaxes[2][vRegNum].rawcopy(loadvec<float>(vertMaxesPtr, stride * 2));
                        temp.rawcopy(loadvec<float>(vertMaxesPtr, stride * 3));
                        vertMaxes[2][vRegNum] = max(vertMaxes[2][vRegNum], temp);
                    }
                    if (kh > 4)
                    {
                        vertMaxes[2][vRegNum].rawcopy(loadvec<float>(vertMaxesPtr, stride * 2));
                        temp.rawcopy(loadvec<float>(vertMaxesPtr, stride * 3));
                        vertMaxes[2][vRegNum] = max(vertMaxes[2][vRegNum], temp);
                        for(int i = 4; i < kh; i++)
                        {
                            temp = loadvec<float>(vertMaxesPtr, stride * i);
                            vertMaxes[2][vRegNum] = max(vertMaxes[2][vRegNum], temp);
                        }
                    }
                    vertMaxes[0][vRegNum] = max(vertMaxes[0][vRegNum], vertMaxes[2][vRegNum]);
                    temp = loadvec<float>(vertMaxesPtr, stride * kh);
                    vertMaxes[2][vRegNum] = max(vertMaxes[2][vRegNum], temp);
                    vertMaxes[1][vRegNum] = max(vertMaxes[1][vRegNum], vertMaxes[2][vRegNum]);
                    temp = loadvec<float>(vertMaxesPtr, stride * (kh + 1));
                    vertMaxes[2][vRegNum] = max(vertMaxes[2][vRegNum], temp);

                }

                WHILE_(x < check) 
                {
                    //TODO(ch): Restore halide trick here(is 3lines scheme, actually) 
                    IReg result__ = result_ + x;
                    int lastVRegNum = horVecsPerOut-1;

                    IReg vertMaxesPtr = data_ + x + lastVRegNum * CTX.vbytes();
                    VReg<float> temp;

                    vertMaxes[0][lastVRegNum].rawcopy(loadvec<float>(vertMaxesPtr));
                    vertMaxes[1][lastVRegNum].rawcopy(loadvec<float>(vertMaxesPtr, stride));
                    vertMaxes[0][lastVRegNum] = max(vertMaxes[0][lastVRegNum], vertMaxes[1][lastVRegNum]);
                    
                    if (kh == 3)
                    {
                        vertMaxes[2][lastVRegNum].rawcopy(loadvec<float>(vertMaxesPtr, stride * 2));
                        temp.rawcopy(loadvec<float>(vertMaxesPtr, stride * 3));
                    }
                    if (kh == 4)
                    {
                        vertMaxes[2][lastVRegNum].rawcopy(loadvec<float>(vertMaxesPtr, stride * 2));
                        temp.rawcopy(loadvec<float>(vertMaxesPtr, stride * 3));
                        vertMaxes[2][lastVRegNum] = max(vertMaxes[2][lastVRegNum], temp);
                    }
                    if (kh > 4)
                    {
                        vertMaxes[2][lastVRegNum].rawcopy(loadvec<float>(vertMaxesPtr, stride * 2));
                        temp.rawcopy(loadvec<float>(vertMaxesPtr, stride * 3));
                        vertMaxes[2][lastVRegNum] = max(vertMaxes[2][lastVRegNum], temp);
                        for(int i = 4; i < kh; i++)
                        {
                            temp = loadvec<float>(vertMaxesPtr, stride * i);
                            vertMaxes[2][lastVRegNum] = max(vertMaxes[2][lastVRegNum], temp);
                        }
                    }
                    vertMaxes[0][lastVRegNum] = max(vertMaxes[0][lastVRegNum], vertMaxes[2][lastVRegNum]);
                    temp = loadvec<float>(vertMaxesPtr, stride * kh);
                    vertMaxes[2][lastVRegNum] = max(vertMaxes[2][lastVRegNum], temp);
                    vertMaxes[1][lastVRegNum] = max(vertMaxes[1][lastVRegNum], vertMaxes[2][lastVRegNum]);
                    temp = loadvec<float>(vertMaxesPtr, stride * (kh + 1));
                    vertMaxes[2][lastVRegNum] = max(vertMaxes[2][lastVRegNum], temp);               

                    std::vector<VReg<float> > res(3, VReg<float>());
                    for(int n = 0; n < 3; n++)
                        res[n].rawcopy(max(vertMaxes[n][0], ext(vertMaxes[n][0], vertMaxes[n][1], 1)));

                    for(int i = 2; i < kw; i++ )
                    {
                        for(int n = 0; n < 3; n++)
                        {
                            int vRegNum = i / 4;
                            if (i % 4 == 0) 
                                res[n] = max(res[n], vertMaxes[n][vRegNum]);
                            else
                                res[n] = max(res[n], ext(vertMaxes[n][vRegNum], vertMaxes[n][vRegNum+1], i % 4));
                        }
                    }

                    for(int n = 0; n < 3; n++)
                    {
                        for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
                        {
                            vertMaxes[n][vRegNum] = vertMaxes[n][vRegNum+1];
                        }
                    }
                    
                    storevec(result__, res[0]);
                    storevec(result__ + stride0, res[1]);
                    storevec(result__ + 2 * stride0, res[2]);
                    
                    x += CTX.vbytes();
                }
                y += 3;
                result_ += W0 * (3 * sizeof(float));
                data_ += W *  (3 * sizeof(float));
            }
            WHILE_(y < H0) 
            {
                IReg x = CONST_(0);

                int horVecsPerOut = (kw + 3)/4 + ((kw + 3)%4?1:0);
                IReg stride = W * sizeof(float);
                std::vector<VReg<float> > vertMaxes(horVecsPerOut, VReg<float>());
                for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
                {
                    IReg vertMaxesPtr = data_ + vRegNum * CTX.vbytes();
                    vertMaxes[vRegNum].rawcopy(max(loadvec<float>(vertMaxesPtr, 0), loadvec<float>(vertMaxesPtr, stride)));
                    for(int i = 2; i < kh; i++)
                        vertMaxes[vRegNum] = max(vertMaxes[vRegNum], loadvec<float>(vertMaxesPtr, (i * stride)));
                }
                
                WHILE_(x < check) 
                {
                    //TODO(ch): Restore halide trick here(is 3lines scheme, actually) 

                    IReg data__ = data_ + x;
                    IReg result__ = result_ + x;

                    int lastVRegNum = horVecsPerOut-1;
                    IReg vertMaxesPtr = data__ + lastVRegNum * CTX.vbytes();
                    vertMaxes[lastVRegNum].rawcopy(max(loadvec<float>(vertMaxesPtr, 0), loadvec<float>(vertMaxesPtr, stride)));
                    for(int i = 2; i < kh; i++)
                        vertMaxes[lastVRegNum] = max(vertMaxes[lastVRegNum], loadvec<float>(vertMaxesPtr, (i * stride)));

                    VReg<float> res = max(vertMaxes[0], ext(vertMaxes[0], vertMaxes[1], 1));
                    for(int i = 2; i < kw; i++ )
                    {
                        int vRegNum = i / 4;
                        if (i % 4 == 0) 
                            res = max(res, vertMaxes[vRegNum]);
                        else
                            res = max(res, ext(vertMaxes[vRegNum], vertMaxes[vRegNum+1], i % 4));
                    }
                    for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
                    {
                        vertMaxes[vRegNum] = vertMaxes[vRegNum+1];
                    }
                    storevec(result__, res);
                    
                    x += CTX.vbytes();
                }
                y += 1;
                result_ += W0 * sizeof(float);
                data_ += W * sizeof(float);
            }
            c += 1;
        }
        RETURN_(0);
        return CTX.getFunc("minmaxloc");
    }
}

int main(int argc, char** argv)
{
    const int TESTITERATIONS = 30;
    int64_t kh = 13;
    int64_t kw = 13;
    loops::Context CTX;
    loops::Func mmlfunc = genminmaxloc(CTX, kh, kw);

    std::cout << "--------MINMAXLOCEXAMPLE---------" << std::endl;
    std::cout << "======--BYTECODE-LISTING--=======" << std::endl;
    mmlfunc.printBytecode(std::cout);
    std::string platform = CTX.getPlatformName();
    std::transform(platform.begin(), platform.end(), platform.begin(), ::toupper);
    std::cout << "======--" << platform << "--LISTING--====== = " << std::endl;
    mmlfunc.printAssembly(std::cout);
    std::cout << "======--FUNCTION-OUTPUT---=======" << std::endl;

    int W = 200;
    int H = 200;
    const int C = 32;
    if ((W - kw + 1) % 4)
        W = ((W -kw + 1) / 4 + 1) * 4 + kw -1;
    // if ((H - kh + 1) % 3)
    //     H = ((H -kh + 1) / 3 + 1) * 3 + kh -1;
    const int H0 = H-kh+1;
    const int W0 = W-kw+1;
    std::vector<float> inp(W*H*C, 0.f), out0(W0*H0*C, 0.f), out3(W0*H0*C, 0.f);

    for (int i = 0 ; i < C*H*W ; i++)
        inp[i] = rand() % 1000;

    minmaxpos_t f = reinterpret_cast<minmaxpos_t>(mmlfunc.ptr());

    MaxPool_vec_unroll4(&inp[0], H, W, C, &out3[0], H0, W0, kh, kw);
    Timer t;
    for(int testiter = 0; testiter < TESTITERATIONS; testiter++)
    {
        t.start();
        int retval = f(&inp[0], H, W, C, &out0[0], H0, W0);
        t.stop();
    }
    std::cout<<"    Maxpooling " << kh << "x" << kw <<". Optimized time = "<<t.str()<<std::endl;

    // printData(&inp[0], H, W);

    for(int i = 0; i < H0*W0*C; i++)
    {
        // EXPECT_EQ(out0[i], out1[i]);
        // EXPECT_EQ(out0[i], out2[i]);
        if(out0[i] != out3[i]){
            std::cout << "ERROR!!!" << std::endl;
            break;
        }
    }
    // printData(&out3[0], H0, W0);
    // printData(&out0[0], H0, W0);
    // std::cout << H0 << W0 << std::endl;

    return 0;
}
