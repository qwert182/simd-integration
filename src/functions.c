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
