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
#include "tests.hpp"

namespace loops
{

class MaxpoolTestImpl: public MaxpoolTest
{
public:
    MaxpoolTestImpl(std::ostream* a_out): MaxpoolTest(), out(a_out)
    {
        CTX = (Context*)create_context();
    }
    virtual ~MaxpoolTestImpl() {free_context(CTX);};
    virtual void run();
private:
    void gendata(float* data, int kh, int kw, int H, int W, int C);
    void ref(float* data, int H, int W, int C, float* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type);
    bool compare(float* tocheck, float* ref, int C, int H, int W, float empty_value);
    bool compare_alg_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference);
    void print_algs_limits(const dwc_algs_limits& tocheck);
    Context* CTX;
    std::ostream* out;
};

MaxpoolTest::MaxpoolTest(std::ostream* a_out)
{ impl = new MaxpoolTestImpl(a_out); }

MaxpoolTest::~MaxpoolTest()
{ delete impl; }

void MaxpoolTest::run()
{ impl->run(); }

void MaxpoolTestImpl::run() 
{
    const int TESTITERATIONS = 30;

    enum {PERF, REGRESS};
    enum {SMALL_ALPHA, BIG_ALPHA};
    std::vector<std::vector<int> > fixtures = {
        {3, 3, 3, 10, 10, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10,  3, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {5, 5, 256,  40,  40, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {7, 7, 256,  40,  40, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {9, 9, 256,  40,  40, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        {5, 5, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {7, 7, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {9, 9, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {5, 5, 256,  39,  39, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {7, 7, 256,  39,  39, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        {5, 5, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {7, 7, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {9, 9, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU, SMALL_ALPHA, REGRESS},
        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU6, SMALL_ALPHA, REGRESS},
        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, SMALL_ALPHA, REGRESS},
        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, BIG_ALPHA, REGRESS},
        {13, 13, 32, 200, 200, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, PERF},
        {5, 5, 1632, 7, 7, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, PERF},
        {3, 3, 32, 112, 112, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, PERF},
        {3, 3, 192, 56, 56, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, PERF},
//      {kh,kw, NC, H, W, padding_top, padding_left, padding_bottom, padding_right}
    };
    for(auto fxt: fixtures)
    {
        int kh = fxt[0];
        int kw = fxt[1];
        const int NC = fxt[2];
        const int H = fxt[3];
        const int W = fxt[4];
        const int padding_top = fxt[5];
        const int padding_left = fxt[6];
        const int padding_bottom = fxt[7];
        const int padding_right = fxt[8];
        const int stride_y = 1, stride_x = 1, dilation_y = 1, dilation_x = 1;
        const int activation = fxt[9];
        const float alpha = fxt[10] == BIG_ALPHA ? 1.25 : 0.25;
        bool perf = (fxt[11] == PERF);
        const int H0 = H-kh+1+padding_top+padding_bottom;
        const int W0 = W-kw+1+padding_left+padding_right;
        const float empty_value = kh * kw * 2000 + 1;
        (*out) << "Maxpooling "<</*(fxt[0]==TYPE_FP16?"FP16 ":"FP32 ")*/"FP32 "<<kh<<"x"<<kw<<", C = "<< NC << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << ", stride_y = " << stride_y << ", stride_x = " << stride_x  << std::endl;
        dwc_algs_limits algsLimits;
        calc_maxpool_algs_limits_f32(CTX, &algsLimits, NC, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, dilation_y, dilation_x);

        maxpool_f32_t func = generate_maxpool_f32(CTX, kh, kw, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, dilation_y, dilation_x, activation, alpha);
        std::vector<float> indata(W*H*NC);
        std::vector<float> outdata(H0*W0*NC * 3, empty_value);
        std::vector<float> outdataref(H0*W0*NC, 0);
        float* inptr = &(indata[0]);
        float* optr = &(outdata[0]) + H0*W0*NC;
        float* optrref = &(outdataref[0]);
        gendata(inptr, kh, kw, H, W, NC);
        ref(inptr, H, W, NC, optrref, H0, W0, alpha, kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation);
        // print_algs_limits(algsLimits); //DUBUGG
        // print_channel(inptr, H, W);    //DUBUGG
        
        if(perf)
        {
            Timer t;
            int ret;
            for(int testiter = 0; testiter < TESTITERATIONS; testiter++)
            {
                t.start();
                ret = func(inptr, H, W, NC, optr, H0, W0, &algsLimits);
                t.stop();
            }
            if(compare(&(outdata[0]), optrref, NC, H0, W0, empty_value))
                (*out)<<"    Optimized time = "<<t.str()<<std::endl;
            else
                return;
        }
        else
        {
            int ret;
            ret = func(inptr, H, W, NC, optr, H0, W0, &algsLimits);
            if(!compare(&(outdata[0]), optrref, NC, H0, W0, empty_value))
            {
                // print_channel(optrref, H0, W0);//DUBUGG
                // print_channel(optr, H0, W0);//DUBUGG
                // std::cout << "DUBUGGGret:" << ret << std::endl;
                (*out)<<"    FAILED!"<<std::endl;
                return;
            }
        }
    }
}
                                          
void MaxpoolTestImpl::gendata(float* data, int kh, int kw, int H, int W, int C)
{
    for (int i = 0 ; i < C*H*W ; i++)
        data[i] = (rand() % 10000)/5000.0f - 1;
    //DUBUGGG
    // for (int i = 0 ; i < C*H*W ; i++)
    //     data[i] = (rand() % 200) - 100;
}

void MaxpoolTestImpl::ref(float* data, int H, int W, int C, float* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type)
{
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

    std::vector<int> tab(kh*kw, 0);
    for(int r = 0; r < kh; r++)
        for(int c = 0; c < kw; c++)
            tab[r*kw + c] = W * r + c;

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
                for(int vnum = 0; vnum < 16/sizeof(float); vnum++)
                {
                    float* vres = result + c*H0*W0+y*W0+x + vnum;
                    switch(activation_type)
                    {
                        case(ACT_NONE): break;
                        case(ACT_RELU):  *vres = std::max(*vres, float(0)); break;
                        case(ACT_RELU6): *vres = std::min(std::max(*vres, float(0)), float(6)); break;;
                        case(ACT_LRELU):
                            *vres = alpha == 1 ? *vres :
                                    alpha <  1 ? std::max(alpha*(*vres), (*vres)):
                                                 std::min(alpha*(*vres), (*vres));
                        break;
                        default: throw std::runtime_error("Unknown activation");
                    };        
                }
            }
        }
    }
}

void MaxpoolTestImpl::print_algs_limits(const dwc_algs_limits& toprint)
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

bool MaxpoolTestImpl::compare(float* tocheck, float* ref, int C, int H, int W, float empty_value)
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
                    (*out)<<"    Result non-equal to reference at ["<< k <<", "<< i <<", "<< j<<"]"<<std::endl;
                    (*out)<<"        Result got:"<< tchk <<std::endl;
                    (*out)<<"        Reference:"<< r <<std::endl;
                    return false;
                }
            }

    return true;
}

bool MaxpoolTestImpl::compare_alg_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference) //DUBUG: create common class. This check will be needed in coming handleFixture.
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
