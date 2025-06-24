#pragma once

#include <sys/cdefs.h>
#include <time.h>

/* gets the current time in milliseconds */
time_t time_pull(void);

/* Polls the time whether a given timeout has passed, comparing against `curr` as the current time.
 * if `curr` ≥ `*proj`, `curr` + `delta` is written to `*proj`. `1` is returned.
 * otherwise, we just return `0`. */
__nonnull((3)) int time_poll(time_t curr, time_t delta, time_t* restrict proj);
