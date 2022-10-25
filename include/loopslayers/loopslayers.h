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
    typedef int64_t (*dwconv_f32_t)(float* data, float* kernel, float* bias, int64_t H, int64_t W, int64_t C, float* result, int64_t H0, int64_t W0, struct dwc_algs_limits* algsLimits);
    dwconv_f32_t generate_dwc_f32(int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha);
    void calc_dwc_algs_limits_f32(struct dwc_algs_limits* out, int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right);
#ifdef __cplusplus
}
#endif
#endif //__LOOPS_LOOPSLAYERS_HPP__