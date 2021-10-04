#include "integration.h"
#include "utils.h"
#include <stdio.h>

#include "methods.h"
#include "functions.h"

enum vecsize_t {
    SCALAR,
    VECTOR2
};

void test_computing_pi(integrate_method_t integrate,
                       const char *method_name,
                       int n,
                       enum vecsize_t vec)
{
    struct integrate_params_t params = {
        -1.0, 1.0, 10000
    };
    double pi = 0;
    union func_t semicircle_func = {0};
    utime_t start, end;

    switch (vec) {
    case SCALAR:
        semicircle_func.scalar = semicircle;
        break;
    case VECTOR2:
        semicircle_func.vec2 = semicircle_vec2;
        break;
    }
    volatile double warm_up = integrate(semicircle_func, &params);
    (void)warm_up;

    params.n = n;

    printf("using %s method\n", method_name);
    fflush(stdout);

    utime_get(&start);
    pi = 2 * integrate(semicircle_func, &params);
    utime_get(&end);

    printf("  pi ~= %.15lf", pi);
    printf(", time = %lf\n", (double)utime_diff_us(&start, &end) * 1e-6);
    fflush(stdout);
}

int main() {
    int n = 300000000;
    test_computing_pi(integrate_rectangle, "rectangle", n, SCALAR);
    //test_computing_pi(integrate_trapezoidal, "trapezoidal", n, SCALAR);
    //test_computing_pi(integrate_simpson, "simpson", n, SCALAR);
    test_computing_pi(integrate_rectangle_optimized, "rectangle (-O2)", n, SCALAR);
    //test_computing_pi(integrate_trapezoidal_optimized, "trapezoidal (-O2)", n, SCALAR);
    //test_computing_pi(integrate_simpson_optimized, "simpson (-O2)", n, SCALAR);
    test_computing_pi(integrate_rectangle_sse, "rectangle vec2 (manual SSE)", n, VECTOR2);
    return 0;
}
