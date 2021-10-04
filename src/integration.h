#pragma once

#include "functions.h"

struct integrate_params_t {
    double a, b;
    int n;
};

typedef double (*integrate_method_t)(union func_t, struct integrate_params_t *);
