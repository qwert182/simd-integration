#include "functions.h"
//#include <math.h>
#include <immintrin.h>

/*
 * https://en.wikipedia.org/wiki/Semicircle#Equation
 */
double semicircle(double x) {
    //return sqrt(1.0 - x*x);
    __m128d s = _mm_set_sd(1.0 - x*x);
    return _mm_cvtsd_f64(_mm_sqrt_sd(s, s));
}

__m128d VECTORCALL semicircle_vec2(__m128d x1_x2) {
    static const __m128d one_one = {1.0, 1.0};
    __m128d s = _mm_sub_pd(one_one, _mm_mul_pd(x1_x2, x1_x2));
    return _mm_sqrt_pd(s);
}
