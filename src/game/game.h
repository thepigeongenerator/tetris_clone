#pragma once

#include <stdint.h>

#include "../window/colour.h"
#include "tetromino/shapes.h"

// stores the data used in the game
#define COLUMNS ((int8_t)10)
#define ROWS    ((int8_t)(COLUMNS * 2))

typedef const Colour* CRow;
typedef Colour* Row;

typedef struct {
    Row rows[ROWS];
    ShapeId nxt[7];   // the order of the shape ids that they should appear in
    uint8_t curr_idx; // current shape index
    int8_t sel_x;     // selected shape x position
    int8_t sel_y;     // selected shape y position
} GameData;

void next_shape(GameData* game_data);
void game_init(GameData* game_data);                        // initializes the game
void game_update(GameData* game_data, const uint8_t* keys); // updates the game's state
void game_free(GameData* game_data);                        // free all data stored with the game
