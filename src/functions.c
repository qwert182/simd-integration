#include "functions.h"
#include <math.h>

/*
 * https://en.wikipedia.org/wiki/Semicircle#Equation
 */
double semicircle(double x) {
    return sqrt(1.0 - x*x);
}
