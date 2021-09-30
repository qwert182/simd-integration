typedef float main_t;

typedef main_t (*func_t)(main_t x);

struct integrate_params_t {
    main_t x_start, x_end;
};

typedef main_t (*integrate_func_t)(func_t func, struct integrate_params_t params);
