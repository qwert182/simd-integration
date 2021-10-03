#include "methods.h"
#include <assert.h>

/*
 * https://ru.wikipedia.org/wiki/Метод_прямоугольников#Пример_реализации
 */
DECLARE_METHOD(integrate_rectangle)
{
    double result = 0;
    double h = (params->b - params->a) / (double)params->n;
    double a_plus_half_h = params->a + h * 0.5;

    for (int i = 0; i < params->n; i++) {
        result += func(a_plus_half_h + h * (double)i);
    }

    return result * h;
}

/*
 * https://ru.wikipedia.org/wiki/Метод_трапеций#Формула_Котеса
 */
DECLARE_METHOD(integrate_trapezoidal)
{
    double result = 0;
    double h = (params->b - params->a) / (double)params->n;

    result += 0.5 * (func(params->a) + func(params->b));

    for (int i = 1; i < params->n; i++) {
        result += func(params->a + h * (double)i);
    }

    return result * h;
}

/*
 * https://ru.wikipedia.org/wiki/Формула_Симпсона#Составная_формула_(формула_Котеса)
 */
DECLARE_METHOD(integrate_simpson)
{
    double result = 0;
    double h = (params->b - params->a) / (double)params->n;

    assert(params->n % 2 == 0);

    result += func(params->a) + func(params->b);

    for (int i = 1; i < params->n; i += 2) {
        result += 4 * func(params->a + h * (double)i);
        result += 2 * func(params->a + h * (double)(i + 1));
    }

    return result * h / 3.0;
}
