#include "integration.h"
#include <stdio.h>
#include <math.h>

#include "methods.h"

/*
 * https://en.wikipedia.org/wiki/Semicircle#Equation
 */
number_t semicircle(number_t x) {
    return SQRT(NUMBER_T_VALUE(1.0) - x*x);
}

int main() {
    number_t y;
    struct integrate_params_t params = {
        NUMBER_T_VALUE(-1.0), NUMBER_T_VALUE(1.0), 10000
    };
    y = 2 * integrate_rectangle_method(semicircle, &params);
    printf("rectangle: %"NUMBER_T_FMT"\n", y);
    y = 2 * integrate_trapezoidal_method(semicircle, &params);
    printf("trapezoidal: %"NUMBER_T_FMT"\n", y);
    y = 2 * integrate_simpson_method(semicircle, &params);
    printf("simpson: %"NUMBER_T_FMT"\n", y);
    return 0;
}
