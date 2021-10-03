#pragma once

#include "integration.h"

double integrate_rectangle(func_t, struct integrate_params_t *);
double integrate_trapezoidal(func_t, struct integrate_params_t *);
double integrate_simpson(func_t, struct integrate_params_t *);
