#pragma once

#include "integration.h"

#define DECLARE_METHOD_DEFAULT(name) \
    double name(union func_t func, \
                struct integrate_params_t *params)

#define DECLARE_METHOD_OPT(name, opt) \
    double name##_##opt(union func_t func, \
                        struct integrate_params_t *params)

#ifdef INTEGRATE_OPT
    #define DECLARE_METHOD_expand1(name, opt) \
        DECLARE_METHOD_OPT(name, opt)
    #define DECLARE_METHOD(name) \
        DECLARE_METHOD_expand1(name, INTEGRATE_OPT)
#else
    #define DECLARE_METHOD(name) \
        DECLARE_METHOD_DEFAULT(name)
#endif

DECLARE_METHOD_DEFAULT(integrate_rectangle);
DECLARE_METHOD_DEFAULT(integrate_trapezoidal);
DECLARE_METHOD_DEFAULT(integrate_simpson);

DECLARE_METHOD_OPT(integrate_rectangle, optimized);
DECLARE_METHOD_OPT(integrate_trapezoidal, optimized);
DECLARE_METHOD_OPT(integrate_simpson, optimized);

DECLARE_METHOD_OPT(integrate_rectangle, sse);
