#pragma once

#include <stdint.h>

#include "../window/colour.h"
#include "tetromino/shapes.h"

// stores the data used in the game
#define COLUMNS ((uint8_t)16)
#define ROWS    ((uint8_t)(COLUMNS * 2))

typedef struct {
    ShapeId id;
    uint8_t x;
    uint8_t y;
} SelectedShape;

typedef struct {
    Colour columns[COLUMNS];
} Row;


typedef struct {
    SelectedShape selected;
    Row row[ROWS];
} GameData;

// updates the game's state
void game_update(GameData* game_data, const uint8_t* keys);
