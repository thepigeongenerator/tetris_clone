#pragma once

#include <stdint.h>

#include "../window/colour/colour8.h"

struct opts {
    float sensitivity;      // sensitivity for moving the tetromino around
    float sensitivity_roll; // sensitivity for rotating the tetromino
    colour8 colour_O;       // colour for the O shape
    colour8 colour_I;       // colour for the I shape
    colour8 colour_S;       // colour for the S shape
    colour8 colour_Z;       // colour for the Z shape
    colour8 colour_T;       // colour for the T shape
    colour8 colour_L;       // colour for the L shape
    colour8 colour_J;       // colour for the J shape
};

extern struct opts opts;

int load_opts(void);
