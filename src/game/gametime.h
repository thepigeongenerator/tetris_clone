#pragma once
#include <time.h>

struct gametime {
    struct timespec ts;
    time_t ms;
};

#if _POSIX_C_SOURCE >= 199309L
static inline void gametime_get(struct timespec* ts) {
    clock_gettime(CLOCK_MONOTONIC, ts);
}
#elif defined _WIN32
# include <windows.h>
static inline void gametime_get(struct timespec* ts) {
    LARGE_INTEGER cnt, frq;
    QueryPerformanceCounter(&cnt);
    QueryPerformanceFrequency(&frq);
    ts->tv_sec = (time_t)(cnt.QuadPart / frq.QuadPart);
    ts->tv_nsec = (time_t)((cnt.QuadPart % frq.QuadPart) * 1000000000 / frq.QuadPart);
}
#else
# error platform not supported
#endif
