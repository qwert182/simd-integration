#include "methods.h"
#include <assert.h>

/*
 * https://ru.wikipedia.org/wiki/Метод_прямоугольников#Пример_реализации
 */
number_t integrate_rectangle_method(func_t func,
                                    struct integrate_params_t *params)
{
    number_t result = 0;
    number_t h = (params->b - params->a) / (number_t)params->n;

    for (int i = 0; i < params->n; i++) {
        result += func(params->a + h * ((number_t)i + NUMBER_T_VALUE(0.5)));
    }

    return result * h;
}

/*
 * https://ru.wikipedia.org/wiki/Метод_трапеций#Формула_Котеса
 */
number_t integrate_trapezoidal_method(func_t func,
                                      struct integrate_params_t *params)
{
    number_t result = 0;
    number_t h = (params->b - params->a) / (number_t)params->n;

    result += NUMBER_T_VALUE(0.5) * (func(params->a) + func(params->b));

    for (int i = 1; i < params->n; i++) {
        result += func(params->a + h * (number_t)i);
    }

    return result * h;
}

/*
 * https://ru.wikipedia.org/wiki/Формула_Симпсона#Составная_формула_(формула_Котеса)
 */
number_t integrate_simpson_method(func_t func,
                                  struct integrate_params_t *params)
{
    number_t result = 0;
    number_t h = (params->b - params->a) / (number_t)params->n;

    assert(params->n % 2 == 0);

    result += func(params->a) + func(params->b);

    for (int i = 1; i < params->n; i += 2) {
        result += 4 * func(params->a + h * (number_t)i);
        result += 2 * func(params->a + h * (number_t)(i + 1));
    }

    return result * h / NUMBER_T_VALUE(3.0);
}
