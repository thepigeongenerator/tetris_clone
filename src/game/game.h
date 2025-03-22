#pragma once
#include <SDL_scancode.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "../window/audio.h"
#include "../window/colour/colour8.h"
#include "./tetromino/shapes.h"
#include "gametime.h"

// constants for pi(π) and tau(τ)
#define PI   (M_PI)         // π constant
#define TAU  (M_PI * 2.0)   // τ constant
#define PIf  (M_PIf)        // π constant as a 32-bit floating point
#define TAUf (M_PIf * 2.0F) // τ constant as a 32-bit floating point

// stores the data used in the game
#define COLUMNS ((int8_t)10)
#define ROWS    ((int8_t)24)

typedef colour8 const* const row_const;
typedef colour8* row;

typedef struct {
    row rows[ROWS];
    gametime* time;
    audiodevice* audio_device;
    audiodata music;
    uint32_t timer_music;
    uint32_t timer_update;
    uint32_t timer_input;
    uint16_t score;
    shape_id nxt[7];  // the order of the shape ids that they should appear in
    uint8_t curr_idx; // current shape index
    int8_t sel_x;     // selected shape x position
    int8_t sel_y;     // selected shape y position
    bool run;
} gamedata;

void next_shape(gamedata*);           // initializes everything needed to start the game; outputs to gamedata
void game_init(gamedata*, gametime*); // initializes the game
void game_update(gamedata*);          // causes an update to occur within the game
void game_free(gamedata*);            // frees the resources associated with the game
