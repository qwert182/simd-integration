#include "methods.h"
#include "utils.h"
#include <assert.h>
#include <emmintrin.h>

typedef __m128d (VECTORCALL *vec2_func_t)(__m128d);

DECLARE_METHOD(integrate_rectangle_sse)
{
    vec2_func_t f = func.vec2;
    int n_has_tail = params->n % 2;
    int n_no_tail = params->n - n_has_tail;
    __m128d result_vec2 = {0.0, 0.0};
    double h = (params->b - params->a) / (double)params->n;
    double a_plus_half_h = params->a + h * 0.5;

    for (int i = 0; i < n_no_tail; i += 2) {
        __m128d arg = {a_plus_half_h + h * (double)i, a_plus_half_h + h * (double)(i+1)};
        result_vec2 = _mm_add_pd(result_vec2, f(arg));
    }

    double result = _mm_cvtsd_f64(result_vec2) + _mm_cvtsd_f64(_mm_unpackhi_pd(result_vec2, result_vec2));
    if (n_has_tail) {
        __m128d arg = {a_plus_half_h + h * (double)n_no_tail, 0};
        result += _mm_cvtsd_f64(f(arg));
    }

    return result * h;
}
