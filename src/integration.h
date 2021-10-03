#pragma once

typedef double (*func_t)(double x);

struct integrate_params_t {
    double a, b;
    int n;
};

typedef double (*integrate_method_t)(func_t, struct integrate_params_t *);
