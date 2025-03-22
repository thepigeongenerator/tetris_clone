#pragma once

#include <time.h>

#include "../util/attributes.h"

typedef struct {
    struct timespec ts; // stores the time at the current update
    double sec;         // stores the current time in seconds
    float scale;        // multiplier for the time calculation, default value is 1.0
    float delta;        // the time that it took between updates
} gametime;

// initializes the gametime struct
atrb_const static inline gametime gametime_new(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);

    return (gametime){
        ts,
        0.0,
        1.0F,
        0.0F,
    };
}

// updates the internal variables
static inline void gametime_update(gametime* gt) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    gt->sec = (double)ts.tv_nsec * 1e-9;                                    // calculate the current time in seconds
    gt->delta = ((double)(ts.tv_nsec - gt->ts.tv_nsec) * 1e-9) * gt->scale; // calculate how much time has passed between this and last frame
    gt->ts = ts;                                                            // update the game's timespec
}

// gets how many times the game updates per second
atrb_const static inline float gametime_get_ups(gametime* gt) {
    return 1.0F / gt->delta;
}
