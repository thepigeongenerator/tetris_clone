#pragma once

#include <stdint.h>

#include "../window/colour.h"
#include "tetromino/shapes.h"

// stores the data used in the game
#define COLUMNS ((int8_t)16)
#define ROWS    ((int8_t)(COLUMNS * 2))

typedef struct {
    ShapeId id;
    int8_t x;
    int8_t y;
} SelectedShape;

typedef const Colour* CRow;
typedef Colour* Row;

typedef struct {
    Colour* row[ROWS];             // stores how to interpert the raw level data
    Colour row_raw[ROWS][COLUMNS]; // stores the raw level data
    SelectedShape selected;
    ShapeId next_shape;
} GameData;

void set_next_shape(GameData* game_data);
void game_init(GameData* game_data);                        // initializes the game
void game_update(GameData* game_data, const uint8_t* keys); // updates the game's state
