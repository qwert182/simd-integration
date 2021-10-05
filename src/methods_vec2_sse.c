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

    const __m128d two_two = {2.0, 2.0};
    const __m128d a_plus_half_h_vec2 = {a_plus_half_h, a_plus_half_h};
    const __m128d h_h = {h, h};
    __m128d i_iplus1 = {0.0, 1.0};
    for (int i = 0; i < n_no_tail; i += 2) {
        __m128d arg = _mm_add_pd(a_plus_half_h_vec2, _mm_mul_pd(h_h, i_iplus1));
        result_vec2 = _mm_add_pd(result_vec2, f(arg));
        i_iplus1 = _mm_add_pd(i_iplus1, two_two);
    }

    double result = _mm_cvtsd_f64(result_vec2) + _mm_cvtsd_f64(_mm_unpackhi_pd(result_vec2, result_vec2));
    if (n_has_tail) {
        __m128d arg = {a_plus_half_h + h * (double)n_no_tail, 0};
        result += _mm_cvtsd_f64(f(arg));
    }

    return result * h;
}

DECLARE_METHOD(integrate_trapezoidal_sse)
{
    vec2_func_t f = func.vec2;
    int n_has_tail = (params->n - 1) % 2;
    int iter_num = (params->n - 1) / 2;
    __m128d result_vec2 = {0.5, 0.5};
    double h = (params->b - params->a) / (double)params->n;

    const __m128d a_b = {params->a, params->b};
    result_vec2 = _mm_mul_pd(result_vec2, f(a_b));

    const __m128d two_two = {2.0, 2.0};
    const __m128d a_a = {params->a, params->a};
    const __m128d h_h = {h, h};
    __m128d i_iplus1 = {1.0, 2.0};
    for (int i = 0; i < iter_num; i++) {
        __m128d arg = _mm_add_pd(a_a, _mm_mul_pd(h_h, i_iplus1));
        result_vec2 = _mm_add_pd(result_vec2, f(arg));
        i_iplus1 = _mm_add_pd(i_iplus1, two_two);
    }

    double result = _mm_cvtsd_f64(result_vec2) + _mm_cvtsd_f64(_mm_unpackhi_pd(result_vec2, result_vec2));
    if (n_has_tail) {
        __m128d arg = {params->a + h * (double)(params->n - 1), 0};
        result += _mm_cvtsd_f64(f(arg));
    }

    return result * h;
}

DECLARE_METHOD(integrate_simpson_sse)
{
    vec2_func_t f = func.vec2;
    double h = (params->b - params->a) / (double)params->n;

    assert(params->n % 2 == 0);

    const __m128d four_two = {4.0, 2.0};
    __m128d result_vec2 = {params->a, params->b};
    result_vec2 = _mm_div_pd(f(result_vec2), four_two);

    int n = params->n;
    const __m128d two_two = {2.0, 2.0};
    const __m128d a_a = {params->a, params->a};
    const __m128d h_h = {h, h};
    __m128d i_iplus1 = {1.0, 2.0};
    for (int i = 1; i < n; i += 2) {
        __m128d arg = _mm_add_pd(a_a, _mm_mul_pd(h_h, i_iplus1));
        result_vec2 = _mm_add_pd(result_vec2, f(arg));
        i_iplus1 = _mm_add_pd(i_iplus1, two_two);
    }

    result_vec2 = _mm_mul_pd(result_vec2, four_two);

    double result = _mm_cvtsd_f64(result_vec2) + _mm_cvtsd_f64(_mm_unpackhi_pd(result_vec2, result_vec2));
    return result * h / 3.0;
}
