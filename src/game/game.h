#pragma once

#include <stdint.h>

#include "../window/audio.h"
#include "../window/colour8.h"
#include "tetromino/shapes.h"

// stores the data used in the game
#define COLUMNS ((int8_t)10)
#define ROWS    ((int8_t)24)

typedef const colour8* const CRow;
typedef colour8* Row;

typedef struct {
    Row rows[ROWS];
    AudioDevice const* audio_device;
    AudioData music;
    uint16_t score;
    ShapeId nxt[7];   // the order of the shape ids that they should appear in
    uint8_t curr_idx; // current shape index
    int8_t sel_x;     // selected shape x position
    int8_t sel_y;     // selected shape y position
} GameData;

void next_shape(GameData* game_data);
void game_init(GameData* game_data);                        // initializes the game
void game_update(GameData* game_data, uint8_t const* keys); // updates the game's state
void game_free(GameData* game_data);                        // free all data stored with the game
