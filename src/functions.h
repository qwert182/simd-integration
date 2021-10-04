#pragma once

#include "utils.h"
#include <immintrin.h>

union func_t {
    double (*scalar)(double);
    __m128d (VECTORCALL *vec2)(__m128d);
};

double semicircle(double x);
__m128d VECTORCALL semicircle_vec2(__m128d x1_x2);
