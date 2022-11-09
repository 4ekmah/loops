/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_LOOPSLAYERS_HPP__
#define __LOOPS_LOOPSLAYERS_HPP__

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif
    enum {ACT_NONE, ACT_RELU, ACT_RELU6, ACT_LRELU};
    struct dwc_algs_limits
    {
        int64_t Cms; //Channel on which starts clear multiline scheme behaviour
        int64_t Cme; //Channel after clear multiline scheme behaviour
        int64_t Cis; //Channel on which starts clear oneline SIMD scheme behaviour
        int64_t Cie; //Channel after clear oneline SIMD scheme behaviour
        int64_t Yms; //Y on (Cms - 1) on which starts multiline scheme behaviour
        int64_t Yme; //Y on Cme after multiline scheme behaviour
        int64_t Yis; //Y on (Cis - 1) on which starts oneline SIMD scheme behaviour
        int64_t Yie; //Y on Cie after oneline SIMD scheme behaviour
        int64_t Xis; //X on (Cis - 1) on Y < Yis on which starts oneline SIMD scheme behaviour
        int64_t Xie; //X on Cie on Y >= Yie after oneline SIMD scheme behaviour
#ifdef __cplusplus
        dwc_algs_limits(int64_t _Cms, int64_t _Cme, int64_t _Cis, int64_t _Cie, int64_t _Yms, int64_t _Yme, int64_t _Yis, int64_t _Yie, int64_t _Xis, int64_t _Xie):
                        Cms(_Cms),Cme(_Cme),Cis(_Cis),Cie(_Cie),Yms(_Yms),Yme(_Yme),Yis(_Yis),Yie(_Yie),Xis(_Xis),Xie(_Xie){}
        dwc_algs_limits():Cms(0),Cme(0),Cis(0),Cie(0),Yms(0),Yme(0),Yis(0),Yie(0),Xis(0),Xie(0){}
#endif
    };
    //NC: Let's pretend, that we have N batches and C channels and we are calling dwconv_t in multithreading mode.
    //Each thread will take part of N * C channels. NC is size of such part. kCS is start channel index of current part,
    //which is obviously needed because kernel and bias arrays still have C channels and data part can start with
    //channel, which does not corresponds to 0 channel of kernel and bias.
    typedef int64_t (*dwconv_f32_t)(float* data, float* kernel, float* bias, int64_t H, int64_t W, int64_t C, int64_t NC, int64_t kCS, float* result, int64_t H0, int64_t W0, struct dwc_algs_limits* algsLimits);
    dwconv_f32_t generate_dwc_f32(int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_x, int stride_y, int dilation_x, int dilation_y, int activation_type, float alpha);
    void calc_dwc_algs_limits_f32(struct dwc_algs_limits* out, int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_x, int stride_y, int dilation_x, int dilation_y);

    typedef void* fp16_ptr; //TODO(ch): IMPORTANT Choose correct fp16* pointer instead of void*. 
    typedef int64_t (*dwconv_f16_t)(fp16_ptr data, fp16_ptr kernel, fp16_ptr bias, int64_t H, int64_t W, int64_t C, int64_t NC, int64_t kCS, fp16_ptr result, int64_t H0, int64_t W0, struct dwc_algs_limits* algsLimits);
    dwconv_f16_t generate_dwc_f16(int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_x, int stride_y, int dilation_x, int dilation_y, int activation_type, float alpha);
    void calc_dwc_algs_limits_f16(struct dwc_algs_limits* out, int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_x, int stride_y, int dilation_x, int dilation_y);

    bool good_alg_limits(struct dwc_algs_limits* out);
#ifdef __cplusplus
}
#endif
#endif //__LOOPS_LOOPSLAYERS_HPP__