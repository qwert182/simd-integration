#pragma once

#if defined(__linux__)

typedef struct {
    long __sec, __nsec;
} utime_t;

void utime_get(utime_t *out);
unsigned long long utime_diff_us(utime_t *start, utime_t *end);

#elif defined(__APPLE__)

typedef unsigned long long utime_t;

void utime_get(utime_t *out);
unsigned long long utime_diff_us(utime_t *start, utime_t *end);

#elif defined(_WIN32)

#if defined(__MINGW32__)
typedef unsigned long long utime_t;
unsigned long long utime_diff_us(utime_t *start, utime_t *end);
#else
typedef unsigned __int64 utime_t;
unsigned __int64 utime_diff_us(utime_t *start, utime_t *end);
#endif
void utime_get(utime_t *out);

#else
#error Unknown target platform
#endif

#if __STDC_VERSION__ > 201112L
#define COMPILE_TIME_ASSERT(cond) _Static_assert(cond, #cond)
#else
#define COMPILE_TIME_ASSERT(cond) typedef char COMPILE_TIME_ASSERT_FAILED##__LINE__[!!(cond)*2 - 1]
#endif

// __vectorcall is available only on Windows but NOT with gcc
#if defined(_WIN32) && !defined(__GNUC__)
#define VECTORCALL __vectorcall
#else
#define VECTORCALL
#endif
