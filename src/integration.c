#include "integration.h"
#include "utils.h"
#include <stdio.h>
#include <math.h>

#include "methods.h"

/*
 * https://en.wikipedia.org/wiki/Semicircle#Equation
 */
number_t semicircle(number_t x) {
    return SQRT(NUMBER_T_VALUE(1.0) - x*x);
}

void test_computing_pi(integrate_method_t integrate_method,
                       const char *method_name,
                       int n)
{
    struct integrate_params_t params = {
        NUMBER_T_VALUE(-1.0), NUMBER_T_VALUE(1.0), n
    };
    number_t pi;
    utime_t start, end;

    printf("using %s method\n", method_name);
    fflush(stdout);

    utime_get(&start);
    pi = 2 * integrate_method(semicircle, &params);
    utime_get(&end);

    printf("  pi ~= %." NUMBER_T_FMT_DIGITS NUMBER_T_FMT, pi);
    printf(", time = %lf\n", (double)utime_diff_us(&start, &end) * 1e-6);
    fflush(stdout);
}

int main() {
    int n = 300000000;
    test_computing_pi(integrate_rectangle_method, "rectangle", n);
    test_computing_pi(integrate_trapezoidal_method, "trapezoidal", n);
    test_computing_pi(integrate_simpson_method, "simpson", n);
    return 0;
}
