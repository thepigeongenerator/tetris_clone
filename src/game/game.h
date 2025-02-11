#pragma once

#include <stdint.h>
#include <time.h>

#include "../window/audio.h"
#include "../window/colour8.h"
#include "tetromino/shapes.h"

// stores the data used in the game
#define COLUMNS ((int8_t)10)
#define ROWS    ((int8_t)24)

typedef colour8 const* const row_const;
typedef colour8* row;

typedef struct {
    row rows[ROWS];
    audio_device* audio_device;
    audio_data music;
    time_t music_timer;
    uint16_t score;
    shape_id nxt[7];  // the order of the shape ids that they should appear in
    uint8_t curr_idx; // current shape index
    int8_t sel_x;     // selected shape x position
    int8_t sel_y;     // selected shape y position
} game_data;

void next_shape(game_data* dat);
void game_init(game_data* dat);                        // initializes the game
void game_update(game_data* dat, uint8_t const* keys); // updates the game's state
void game_free(game_data* dat);                        // free all data stored with the game
