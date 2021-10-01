#include "utils.h"

#if defined(__linux__)

#include <time.h>

void utime_get(utime_t *out)
{
    COMPILE_TIME_ASSERT(sizeof(utime_t) == sizeof(struct timespec));
    clock_gettime(CLOCK_MONOTONIC_RAW, (struct timespec *)out);
}

unsigned long long utime_diff_us(utime_t *start, utime_t *end)
{
    struct timespec *s = (struct timespec *)start;
    struct timespec *e = (struct timespec *)end;

    return (unsigned long long)(e->tv_sec - s->tv_sec) * 1000000 +
        (unsigned long long)((e->tv_nsec - s->tv_nsec) / 1000);
}

#elif defined(__APPLE__)

#include <mach/mach.h>
#include <mach/mach_time.h>

void utime_get(utime_t *out)
{
    COMPILE_TIME_ASSERT(sizeof(utime_t) == sizeof(uint64_t));
    *out = mach_absolute_time();
}

unsigned long long utime_diff_us(utime_t *start, utime_t *end)
{
    return (*end - *start) / 1000;
}

#elif defined(_WIN32)

#include <Windows.h>

void utime_get(utime_t *out)
{
    COMPILE_TIME_ASSERT(sizeof(utime_t) == sizeof(LARGE_INTEGER));
    QueryPerformanceCounter((LARGE_INTEGER*)out);
}

unsigned __int64 utime_diff_us(utime_t *start, utime_t *end)
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (*end - *start) * 1000000 / (unsigned __int64)freq.QuadPart;
}

#endif
