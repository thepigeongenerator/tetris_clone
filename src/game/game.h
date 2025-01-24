#pragma once

#include <stdint.h>

#include "../window/colour.h"

typedef uint32_t PackedRow;

// stores the data used in the game
#define COLUMNS ((uint8_t)(sizeof(PackedRow) * 8 / 3))
#define ROWS    ((uint8_t)(COLUMNS * 2))

typedef struct {
    Colour columns[COLUMNS];
} Row;

typedef struct {
    Row row[ROWS];
} GameData;

// updates the game's state
void game_update(GameData* game_data, const uint8_t* keys);
