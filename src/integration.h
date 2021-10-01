#pragma once

#if 0 // float
    typedef float number_t;
    #define NUMBER_T_VALUE(c) c##f
    #define SQRT(x) sqrtf(x)
    #define NUMBER_T_FMT "f"
    #define NUMBER_T_FMT_DIGITS "7"
#else // double
    typedef double number_t;
    #define NUMBER_T_VALUE(c) c
    #define SQRT(x) sqrt(x)
    #define NUMBER_T_FMT "lf"
    #define NUMBER_T_FMT_DIGITS "15"
#endif

typedef number_t (*func_t)(number_t x);

struct integrate_params_t {
    number_t a, b;
    int n;
};

typedef number_t (*integrate_method_t)(func_t, struct integrate_params_t *);
