/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "mptest.hpp"
#if __LOOPS_ARCH == __LOOPS_AARCH64

#include "loops/loops.hpp"
#include "loopslayers/loopslayers.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <vector>
#include <iostream>
#include <iomanip>
#include "arm_neon.h"
#include "tests.hpp"

namespace loops
{
MaxpoolTest::MaxpoolTest(Context aCTX, std::ostream* a_out): CTX(aCTX), out(a_out) {}

template<class T>
void printData(T* data, int H, int W) { //TODO(ch): move it to test.hpp 
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            std::cout << std::setw(6) << data[i*W+j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<class T>
void printData(T* data, int H, int W, int stride) { //TODO(ch): move it to test.hpp 
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            std::cout << std::setw(15) << data[i*stride+j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
void MaxpoolTest::run() 
{
    const int TESTITERATIONS = 30;

    // std::vector< std::pair<std::vector<int>, std::vector<int>> > limitsFixtures = {
    //     {{3, 3, 3, 10, 3, 0, 0, 0, 0}, {4, 3, 0, 2, 9, 8, 0, 6, 0, 0}},
    //     {{3, 3, 3, 10, 10, 0, 0, 0, 0}, {0, 2, 0, 2, 0, 6, 0, 7, 0, 4}},
    //     {{3, 3, 3, 10, 10, 1, 0, 1, 0}, {1, 2, 0, 2, 1, 6, 0, 8, 0, 4}},
    //     {{3, 3, 3, 10, 10, 0, 1, 0, 1}, {1, 2, 1, 2, 1, 6, 1, 7, 1, 4}},
    //     {{3, 3, 3, 10, 10, 1, 1, 1, 1}, {1, 2, 1, 2, 2, 6, 2, 8, 1, 4}},
    //     {{3, 3, 1, 10, 10, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 6, 0, 7, 0, 4}},
    //     {{3, 3, 1, 10, 10, 1, 0, 1, 0}, {1, 0, 0, 0, 1, 7, 0, 8, 0, 4}},
    //     {{3, 3, 1, 10, 10, 0, 1, 0, 1}, {1, 0, 1, 0, 1, 7, 1, 7, 1, 4}},
    //     {{3, 3, 1, 10, 10, 1, 1, 1, 1}, {1, 0, 1, 0, 2, 8, 2,  8, 1, 4}},
    //     {{3, 3, 1, 1, 1, 1, 1, 1, 1},   {2, 1, 2, 1, 2, 1, 2, 1, 1, 1}},
    //     {{3, 3, 100, 1, 1, 1, 1, 1, 1}, {101, 100, 1, 94, 2, 1, 2, 0, 1, 1}}
    //     };
    // for(auto fxt: limitsFixtures)
    // {
    //     int kh = fxt.first[0];
    //     int kw = fxt.first[1];
    //     const int C = fxt.first[2];
    //     const int H = fxt.first[3];
    //     const int W = fxt.first[4];
    //     const int padding_top = fxt.first[5];
    //     const int padding_left = fxt.first[6];
    //     const int padding_bottom = fxt.first[7];
    //     const int padding_right = fxt.first[8];
    //     const maxpool_algs_limits ref(fxt.second[0], fxt.second[1], fxt.second[2], fxt.second[3], fxt.second[4], fxt.second[5], fxt.second[6], fxt.second[7], fxt.second[8], fxt.second[9]);
    //     const int H0 = H-kh+1+padding_top+padding_bottom;
    //     const int W0 = W-kw+1+padding_left+padding_right;

    //     (*out) << "Maxpooling "<<kh<<"x"<<kw<<", C = "<< C << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << std::endl;
    //     maxpool_algs_limits tocheck;
    //     calc_maxpool_algs_limits_f32(&tocheck, C, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right);
    //     if(!compare_alg_limits(tocheck, ref))
    //         return;
    // }
    enum {PERF, REGRESS};
    enum {SMALL_ALPHA, BIG_ALPHA};
    std::vector<std::vector<int> > fixtures = {
        {5, 5, 3,  10,  10, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},



        // {3, 3, 3, 10, 10, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3, 10, 10, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3, 10, 10, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3, 10, 10, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3, 10, 10, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3, 10, 10, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3, 10, 10, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3, 10, 10, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3,  9,  9, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3,  9,  9, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3,  9,  9, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3,  9,  9, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3,  9,  9, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3,  9,  9, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3,  9,  9, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3,  9,  9, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {3, 3, 3, 10,  3, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},

        // {5, 5, 256,  40,  40, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        // // // // // {7, 7, 256,  40,  40, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        // // // // // {9, 9, 256,  40,  40, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {5, 5, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        // {7, 7, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {9, 9, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // // // // // // {5, 5, 256,  39,  39, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        // // // // // // {7, 7, 256,  39,  39, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        // // // // // // {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {5, 5, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        // {7, 7, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        // {9, 9, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},

        // {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU, SMALL_ALPHA, REGRESS},
        // {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU6, SMALL_ALPHA, REGRESS},
        // {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, SMALL_ALPHA, REGRESS},
        // {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, BIG_ALPHA, REGRESS},

        // {13, 13, 32, 200, 200, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, PERF},
        // {5, 5, 1632, 7, 7, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, PERF},
        // {3, 3, 32, 112, 112, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, PERF},
        // {3, 3, 192, 56, 56, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, PERF},

//      {kh,kw, C, H, W, padding_top, padding_left, padding_bottom, padding_right}
    };
    int DUBUGGGfixtureNum = 0;
    for(auto fxt: fixtures)
    {
        int kh = fxt[0];
        int kw = fxt[1];
        const int C = fxt[2];
        const int H = fxt[3];
        const int W = fxt[4];
        const int padding_top = fxt[5];
        const int padding_left = fxt[6];
        const int padding_bottom = fxt[7];
        const int padding_right = fxt[8];
        const int activation = fxt[9];
        const float alpha = fxt[10] == BIG_ALPHA ? 1.25 : 0.25;
        bool perf = (fxt[11] == PERF);
        const int H0 = H-kh+1+padding_top+padding_bottom;
        const int W0 = W-kw+1+padding_left+padding_right;
        // std::cout << "LALALAL START H0:" << H0 << std::endl; 
        // std::cout << "LALALAL START H:" << H << std::endl; 
        const float empty_value = kh * kw * 2000 + 1;
        (*out) << "Maxpooling "<<kh<<"x"<<kw<<", C = "<< C << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << std::endl;
        maxpool_algs_limits algsLimits;
        calc_maxpool_algs_limits_f32(&algsLimits, C, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right);

        maxpool_f32_t func = generate_maxpool_f32(kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation, alpha);
        std::vector<float> indata(W*H*C);
        std::vector<float> outdata(H0*W0*C * 3, empty_value);
        std::vector<float> outdataref(H0*W0*C, 0);
        float* inptr = &(indata[0]);
        float* optr = &(outdata[0]) + H0*W0*C;
        float* optrref = &(outdataref[0]);
        gendata(inptr, kh, kw, H, W, C);
        // if(DUBUGGGfixtureNum == 0)
        // {
//             std::vector<float> interm =  
// {  -30,    -1,   -83,   -61,    69,    63,   -78,    94,    73,   -53,
//    -69,   -38,   -18,    90,    -8,    91,   -43,    15,    21,    57,
//     74,    -9,    47,    51,   -69,   -79,    37,    40,   -46,   -70,
//     -2,    25,   -19,    16,    16,   -98,   -69,    39,    96,   -96,
//     38,    80,   -82,   -79,    70,   -38,   -88,    79,    77,   -15,
//     36,     4,    76,   -17,   -93,    59,   -43,    44,    -1,    11,
//     27,    50,   -64,    60,   -82,     5,    63,   -51,   -56,    11,
//    -95,    34,    -9,    75,    55,   -86,    89,    68,    93,    18,
//    -95,   -18,   -78,    82,    17,   -70,    -7,    74,    26,    93,
//    -14,   -47,    43,   -26,   -86,    13,    79,    77,    62,    75,

//    -12,    19,    10,    32,    -6,   -83,    46,   -65,    37,    -9,
//     53,    43,    73,    28,    25,    -9,    10,   -82,   -83,   -64,
//     63,   -45,   -10,   -42,    30,     4,    71,   -39,   -67,   -15,
//     89,   -27,   -96,   -49,   -95,   -50,   -32,     3,   -15,   -94,
//     95,   -61,    49,    20,    67,   -74,    63,   -23,    96,    81,
//    -35,    60,   -64,    55,    70,    18,   -89,    42,    32,    96,
//     79,    21,   -30,    84,    72,   -73,   -66,   -60,   -17,   -28,
//     98,   -70,   -37,    47,    50,   -70,    73,    14,   -41,    22,
//    -53,    24,   -18,   -65,   -68,   -96,    54,   -57,    -2,   -14,
//    -60,   -22,    59,   -38,   -38,   -17,   -59,   -52,    23,    24,

//    -28,    22,    54,    35,   -79,   -43,    65,    47,    71,    76,
//    -31,   -82,     1,     3,   -47,    33,     7,    59,    28,   -94,
//     97,    20,   -16,     8,    34,    -2,    91,    76,    -2,    15,
//    -48,    71,    89,    59,     6,   -90,   -84,   -76,     9,    39,
//   -100,    78,     9,   -47,   -19,    14,    38,    89,   -74,   -33,
//     47,   -77,    87,   -69,    32,    22,   -19,   -25,   -50,    79,
//     90,   -46,    50,    31,   -87,   -43,    -6,    81,   -19,   -97,
//     20,   -67,    82,    81,   -13,   -85,    -4,   -75,   -96,    22,
//     -8,    51,    -3,   -68,    34,    81,     6,   -85,   -43,     8,
//     95,    99,    62,    -3,   -17,    76,    54,    77,     9,    87};

//         float* inptr = &(interm[0]);
        // } 
        
        // (*out) << "Maxpooling "<<kh<<"x"<<kw<<", C = "<< C << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << std::endl;
        if (W < 4 || H < 4)
            ref_simple(inptr, H, W, C, optrref, H0, W0, alpha, kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation);
        else
            ref(inptr, H, W, C, optrref, H0, W0, alpha, kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation);
        if(perf)
        {
            Timer t;
            int ret;
            for(int testiter = 0; testiter < TESTITERATIONS; testiter++)
            {
                t.start();
                ret = func(inptr, H, W, C, optr, H0, W0, &algsLimits);
                t.stop();
            }
            if(compare(&(outdata[0]), optrref, C, H0, W0, empty_value))
                (*out)<<"    Optimized time = "<<t.str()<<std::endl;
            else
                return;
        }
        else
        {
            /////////////
            (*out)<<"        DATA"<< std::endl;
            printData(inptr+2*H*W, H, W);
            ////////////
            int ret;
            ret = func(inptr, H, W, C, optr, H0, W0, &algsLimits);
            std::cout << "DUBUGGGret:" << ret << std::endl; 
            if(!compare(&(outdata[0]), optrref, C, H0, W0, empty_value))
            {
                print_algs_limits(algsLimits);
                printData(optrref+2*H0*W0, H0, W0);
                printData(optr+2*H0*W0, H0, W0);
                // printData((int32_t*)(void*)(optr+0*H0*W0), H0, W0);
                (*out)<<"    FAILED!"<<std::endl;
                return;
            }
        }
        DUBUGGGfixtureNum++;
    }
}
                                          
void MaxpoolTest::gendata(float* data, int kh, int kw, int H, int W, int C)
{
    // for (int i = 0 ; i < C*H*W ; i++)
    //     data[i] = (rand() % 10000)/5000.0f - 1;
    for (int i = 0 ; i < C*H*W ; i++) //DUBUGGG : delete and uncomment upper 
        data[i] = rand() % 200 - 100;
}

void MaxpoolTest::ref(float* data, int H, int W, int C, float* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type)
{
    // std::cout << "LALALAL H0:" << H0 << std::endl; 
    std::vector<float> padded;
    {
        const int WP = (W+padding_left+padding_right);
        const int HP = (H+padding_top+padding_bottom);
        padded.resize(WP*HP*C, 0);
        for(int ch = 0; ch < C; ch++)
        {
            float* dest_ = &(padded[WP*HP*ch]);
            float* data_ = &(data[W*H*ch]);
            for(int nprow = 0; nprow < H; nprow++)
            {
                float* dest = dest_ + (nprow + padding_top) * WP + padding_left;
                memcpy(dest, data_ + W * nprow, W * sizeof(float));
            }
        }
        W = WP;
        H = HP;
        data = &padded[0];
    }


    // int* tab = (int*)alloca(kh*kw*sizeof(tab[0]));
    // for (int i = 0; i < kh; i++)
    //     for (int j = 0; j < kw; j++)
    //         tab[i*kw + j] = i*W+j;
    std::vector<int> tab(kh*kw, 0);
    for(int r = 0; r < kh; r++)
        for(int c = 0; c < kw; c++)
            tab[r*kw + c] = W * r + c;
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

void MaxpoolTest::print_algs_limits(const maxpool_algs_limits& toprint)
{
    (*out)<<"    Cms: = " << toprint.Cms<<std::endl;
    (*out)<<"    Cme: = " << toprint.Cme<<std::endl;
    (*out)<<"    Cis: = " << toprint.Cis<<std::endl;
    (*out)<<"    Cie: = " << toprint.Cie<<std::endl;
    (*out)<<"    Yms: = " << toprint.Yms<<std::endl;
    (*out)<<"    Yme: = " << toprint.Yme<<std::endl;
    (*out)<<"    Yis: = " << toprint.Yis<<std::endl;
    (*out)<<"    Yie: = " << toprint.Yie<<std::endl;
    (*out)<<"    Xis: = " << toprint.Xis<<std::endl;
    (*out)<<"    Xie: = " << toprint.Xie<<std::endl;
}

void MaxpoolTest::ref_simple(float* data, int H, int W, int C, float* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type)
// void MaxPool(float* data, int H, int W, int C, float* result, int H0, int W0, int kh, int kw)
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

// void MaxpoolTest::ref(float* data, float* kernel, float* bias, int H, int W, int C, float* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type)
// {
//     std::vector<float> padded;
//     {
//         const int WP = (W+padding_left+padding_right);
//         const int HP = (H+padding_top+padding_bottom);
//         padded.resize(WP*HP*C, 0);
//         for(int ch = 0; ch < C; ch++)
//         {
//             float* dest_ = &(padded[WP*HP*ch]);
//             float* data_ = &(data[W*H*ch]);
//             for(int nprow = 0; nprow < H; nprow++)
//             {
//                 float* dest = dest_ + (nprow + padding_top) * WP + padding_left;
//                 memcpy(dest, data_ + W * nprow, W * sizeof(float));
//             }
//         }
//         W = WP;
//         H = HP;
//         data = &padded[0];
//     }
//     std::vector<int> koffsets(kh*kw, 0);
//     for(int r = 0; r < kh; r++)
//         for(int c = 0; c < kw; c++)
//             koffsets[r*kw + c] = W * r + c;
//     for(int c = 0; c < C; c++)
//     {
//         float* ker = kernel + c * kw * kh;
//         float* inL = data + c * W * H;
//         float* resL = result + c * W0 * H0;
//         float32x4_t b = vdupq_n_f32(bias[c]);
//         for(int i = 0; i < H0; i++) 
//         {
//             float* inR = inL + i * W;
//             float* resR = resL + i * W0;
//             for(int j = 0; j < W0; j++)
//             {
//                 float* inC = inR + j;
//                 float* resC = resR + j;
//                 float32x4_t res = b;
//                 for(int k = 0; k < kh * kw; k++)
//                 {
//                     int krow = k/kw;
//                     int kcol = k - krow * kw;
//                     float32x4_t toAdd = vdupq_n_f32(inC[koffsets[k]]);
//                     float32x4_t weight = vdupq_n_f32(ker[k]);
//                     res = vfmaq_f32(res, toAdd, weight);
//                 }
//                 switch(activation_type)
//                 {
//                     case(ACT_NONE): break;
//                     case(ACT_RELU): res = vmaxq_f32(res, vdupq_n_f32(0)); break;
//                     case(ACT_RELU6): res = vmaxq_f32(vminq_f32(res, vdupq_n_f32(6)), vdupq_n_f32(0)); break;
//                     case(ACT_LRELU):
//                         res = alpha == 1 ? res :
//                               alpha <  1 ? vmaxq_f32(vmulq_f32(res, vdupq_n_f32(alpha)), res) : 
//                                            vminq_f32(vmulq_f32(res, vdupq_n_f32(alpha)), res);
//                     break;
//                     defaout: throw std::runtime_error("Unknown activation");
//                 };                
//                 float res_ = vgetq_lane_f32(res, 0);
//                 // res_ = (res_ > 0 ? res_ : (res_ * alpha));
//                 *resC = res_;
//             }
//         }
//     }
// }

bool MaxpoolTest::compare(float* tocheck, float* ref, int C, int H, int W, float empty_value)
{
    for(int k = 0; k < C; k++)
        for(int i = 0; i < H; i++)
            for(int j = 0; j < W; j++)
                if(tocheck[(k * H + i) * W + j] != empty_value) 
                {
                    (*out)<<"    Memory writing violation at output ["<< -(C - k) <<", "<< i <<", "<< j<<"]"<<std::endl;
                    return false;
                }
    for(int k = 0; k < C; k++)
        for(int i = 0; i < H; i++)
            for(int j = 0; j < W; j++)
                if(tocheck[2*C*W*H+(k * H + i) * W + j] != empty_value) 
                {
                    (*out)<<"    Memory writing violation at output ["<< C + k <<", "<< i <<", "<< j<<"]"<<std::endl;
                    return false;
                }
    tocheck += C*H*W;
    float maxdiff = 0;
    for(int k = 0; k < C; k++)
        for(int i = 0; i < H; i++)
            for(int j = 0; j < W; j++)
            {
                float tchk = tocheck[(k * H + i) * W + j];
                float r = ref[(k * H + i) * W + j];
                if(tchk != r)
                {
                //     // float absErr = std::abs(tchk - r);
                //     // float relErr = absErr / std::abs(r);
                    (*out)<<"    Result non-equal to reference at ["<< k <<", "<< i <<", "<< j<<"]"<<std::endl;
                    (*out)<<"        Result got:"<< tchk <<std::endl;
                    (*out)<<"        Reference:"<< r <<std::endl;
                //     // (*out)<<"        Absolute error:"<< absErr <<std::endl;
                //     // (*out)<<"        Relative error:"<< relErr <<std::endl;
                    return false;
                }
            }
    // ///////////
    // (*out)<<"        Ref"<< std::endl;
    // // printData(ref + 2*H*W, H, W);
    // printData(ref, H, W);
    // (*out)<<"        Check"<< std::endl;
    // // printData(tocheck + 2*H*W, H, W);
    // printData(tocheck, H, W);
    // ///////////
    return true;
}

bool MaxpoolTest::compare_alg_limits(const maxpool_algs_limits& tocheck, const maxpool_algs_limits& reference)
{
    bool res = true;
    if(tocheck.Cms != reference.Cms) {std::cout<<"    Cms:ref = " << reference.Cms << " | checked =  " << tocheck.Cms<<std::endl; res = false;}
    if(tocheck.Cme != reference.Cme) {std::cout<<"    Cme:ref = " << reference.Cme << " | checked =  " << tocheck.Cme<<std::endl; res = false;}
    if(tocheck.Cis != reference.Cis) {std::cout<<"    Cis:ref = " << reference.Cis << " | checked =  " << tocheck.Cis<<std::endl; res = false;}
    if(tocheck.Cie != reference.Cie) {std::cout<<"    Cie:ref = " << reference.Cie << " | checked =  " << tocheck.Cie<<std::endl; res = false;}
    if(tocheck.Yms != reference.Yms) {std::cout<<"    Yms:ref = " << reference.Yms << " | checked =  " << tocheck.Yms<<std::endl; res = false;}
    if(tocheck.Yme != reference.Yme) {std::cout<<"    Yme:ref = " << reference.Yme << " | checked =  " << tocheck.Yme<<std::endl; res = false;}
    if(tocheck.Yis != reference.Yis) {std::cout<<"    Yis:ref = " << reference.Yis << " | checked =  " << tocheck.Yis<<std::endl; res = false;}
    if(tocheck.Yie != reference.Yie) {std::cout<<"    Yie:ref = " << reference.Yie << " | checked =  " << tocheck.Yie<<std::endl; res = false;}
    if(tocheck.Xis != reference.Xis) {std::cout<<"    Xis:ref = " << reference.Xis << " | checked =  " << tocheck.Xis<<std::endl; res = false;}
    if(tocheck.Xie != reference.Xie) {std::cout<<"    Xie:ref = " << reference.Xie << " | checked =  " << tocheck.Xie<<std::endl; res = false;}
    return res;
}
};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64