#include "integration.h"
#include "utils.h"
#include <stdio.h>
#include <math.h>

#include "methods.h"

/*
 * https://en.wikipedia.org/wiki/Semicircle#Equation
 */
double semicircle(double x) {
    return sqrt(1.0 - x*x);
}

void test_computing_pi(integrate_method_t integrate,
                       const char *method_name,
                       int n)
{
    struct integrate_params_t params = {
        -1.0, 1.0, 10000
    };
    double pi;
    utime_t start, end;

    volatile double warm_up = integrate(semicircle, &params);
    (void)warm_up;

    params.n = n;

    printf("using %s method\n", method_name);
    fflush(stdout);

    utime_get(&start);
    pi = 2 * integrate(semicircle, &params);
    utime_get(&end);

    printf("  pi ~= %.15lf", pi);
    printf(", time = %lf\n", (double)utime_diff_us(&start, &end) * 1e-6);
    fflush(stdout);
}

int main() {
    int n = 300000000;
    test_computing_pi(integrate_rectangle, "rectangle", n);
    test_computing_pi(integrate_trapezoidal, "trapezoidal", n);
    test_computing_pi(integrate_simpson, "simpson", n);
    return 0;
}
