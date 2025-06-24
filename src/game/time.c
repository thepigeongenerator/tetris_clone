#include "time.h"

#include <stdbool.h>
#include <time.h>

#if __has_include(<features.h>)
#include <features.h>
#endif
#if __has_include(<features.h>) && _POSIX_C_SOURCE >= 199309L
#include <bits/time.h>
static void gettime(struct timespec* ts) {
	clock_gettime(CLOCK_MONOTONIC, ts);
}
#elif defined(_WIN32)
#include <profileapi.h>
#include <windows.h>
#include <winnt.h>
static void gettime(struct timespec* ts) {
	LARGE_INTEGER cnt, frq;
	QueryPerformanceCounter(&cnt);
	QueryPerformanceFrequency(&frq);
	ts->tv_sec = (time_t)(cnt.QuadPart / frq.QuadPart);
	ts->tv_nsec = (time_t)((cnt.QuadPart % frq.QuadPart) * 1000000000 / frq.QuadPart);
}
#else
#error no implementation of a monotonic clock was available
#endif

time_t time_pull(void) {
	struct timespec ts;
	gettime(&ts);
	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int time_poll(time_t curr, time_t delta, time_t* restrict proj) {
	bool tpass = curr >= *proj;
	*proj += tpass * ((curr + delta) - *proj); // adds 0, or the difference to proj
	return tpass;
}
