#include "methods.h"
#include "utils.h"
#include <assert.h>
#include <emmintrin.h>

typedef __m128d doubleX2;
typedef doubleX2 (VECTORCALL *vec2_func_t)(doubleX2);

DECLARE_METHOD(integrate_rectangle_sse)
{
    vec2_func_t f = func.vec2;
    int n_has_tail = params->n % 2;
    int n_no_tail = params->n - n_has_tail;
    doubleX2 resultX2 = {0.0, 0.0};
    double h = (params->b - params->a) / (double)params->n;
    double a_plus_05_h = params->a + h * 0.5;

    const doubleX2 twoX2 = {2.0, 2.0};
    const doubleX2 a_plus_05_h_X2 = {a_plus_05_h, a_plus_05_h};
    const doubleX2 h_h = {h, h};
    doubleX2 i_iplus1 = {0.0, 1.0};
    for (int i = 0; i < n_no_tail; i += 2) {
        doubleX2 arg = _mm_add_pd(a_plus_05_h_X2, _mm_mul_pd(h_h, i_iplus1));
        resultX2 = _mm_add_pd(resultX2, f(arg));
        i_iplus1 = _mm_add_pd(i_iplus1, twoX2);
    }

    double result = _mm_cvtsd_f64(resultX2) + _mm_cvtsd_f64(_mm_unpackhi_pd(resultX2, resultX2));
    if (n_has_tail) {
        doubleX2 arg = {a_plus_05_h + h * (double)n_no_tail, 0/*unused*/};
        result += _mm_cvtsd_f64(f(arg));
    }

    return result * h;
}

DECLARE_METHOD(integrate_trapezoidal_sse)
{
    vec2_func_t f = func.vec2;
    int n_has_tail = (params->n - 1) % 2;
    int iter_num = (params->n - 1) / 2;
    doubleX2 resultX2 = {0.5, 0.5};
    double h = (params->b - params->a) / (double)params->n;

    const doubleX2 a_b = {params->a, params->b};
    resultX2 = _mm_mul_pd(resultX2, f(a_b));

    const doubleX2 twoX2 = {2.0, 2.0};
    const doubleX2 a_a = {params->a, params->a};
    const doubleX2 h_h = {h, h};
    doubleX2 i_iplus1 = {1.0, 2.0};
    for (int i = 0; i < iter_num; i++) {
        doubleX2 arg = _mm_add_pd(a_a, _mm_mul_pd(h_h, i_iplus1));
        resultX2 = _mm_add_pd(resultX2, f(arg));
        i_iplus1 = _mm_add_pd(i_iplus1, twoX2);
    }

    double result = _mm_cvtsd_f64(resultX2) + _mm_cvtsd_f64(_mm_unpackhi_pd(resultX2, resultX2));
    if (n_has_tail) {
        doubleX2 arg = {params->a + h * (double)(params->n - 1), 0};
        result += _mm_cvtsd_f64(f(arg));
    }

    return result * h;
}

DECLARE_METHOD(integrate_simpson_sse)
{
    vec2_func_t f = func.vec2;
    double h = (params->b - params->a) / (double)params->n;

    assert(params->n % 2 == 0);

    const doubleX2 four_two = {4.0, 2.0};
    doubleX2 resultX2 = {0.0, 0.0};

    int n = params->n;
    const doubleX2 twoX2 = {2.0, 2.0};
    const doubleX2 a_a = {params->a, params->a};
    const doubleX2 h_h = {h, h};
    doubleX2 i_iplus1 = {1.0, 2.0};
    for (int i = 1; i < n; i += 2) {
        doubleX2 arg = _mm_add_pd(a_a, _mm_mul_pd(h_h, i_iplus1));
        resultX2 = _mm_add_pd(resultX2, f(arg));
        i_iplus1 = _mm_add_pd(i_iplus1, twoX2);
    }

    resultX2 = _mm_mul_pd(resultX2, four_two);

    const doubleX2 a_b = {params->a, params->b};
    resultX2 = _mm_add_pd(resultX2, f(a_b));

    double result = _mm_cvtsd_f64(resultX2) + _mm_cvtsd_f64(_mm_unpackhi_pd(resultX2, resultX2));
    return result * h / 3.0;
}
