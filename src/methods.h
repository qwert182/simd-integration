#pragma once

#include "integration.h"

number_t integrate_rectangle_method(func_t, struct integrate_params_t *);
number_t integrate_trapezoidal_method(func_t, struct integrate_params_t *);
number_t integrate_simpson_method(func_t, struct integrate_params_t *);
