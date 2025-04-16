#pragma once
#include <time.h>

struct gametime {
    struct timespec ts;
    time_t ms;
};

#if __has_include(<features.h>)
# include <features.h>
# if _POSIX_C_SOURCE >= 199309L
#  include <bits/time.h>
static inline void gametime_get(struct timespec* ts) {
    clock_gettime(CLOCK_MONOTONIC, ts);
}
#  define GTIME_USE_UNIX
# endif
#endif

#if defined _WIN32 && !defined GTIME_USE_UNIX
# include <profileapi.h>
# include <windows.h>
# include <winnt.h>
static inline void gametime_get(struct timespec* ts) {
    LARGE_INTEGER cnt, frq;
    QueryPerformanceCounter(&cnt);
    QueryPerformanceFrequency(&frq);
    ts->tv_sec = (time_t)(cnt.QuadPart / frq.QuadPart);
    ts->tv_nsec = (time_t)((cnt.QuadPart % frq.QuadPart) * 1000000000 / frq.QuadPart);
}
#elif defined GTIME_USE_UNIX
# undef GTIME_USE_UNIX
#else
# error platform not supported
#endif
