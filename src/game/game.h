#pragma once
#include <SDL2/SDL.h>

typedef uint32_t PackedRow;

// stores the data used in the game
#define COLUMNS ((uint8_t)(sizeof(PackedRow) * 8 / 3))
#define ROWS ((uint8_t)(COLUMNS * 2))

typedef union {
    // NOTE: this has packing issues
    struct {
        uint8_t block : 3;
    } columns[COLUMNS];
    PackedRow val;
} Row;

typedef struct {
    Row row[ROWS];
} GameData;

// updates the game's state
void game_update(GameData* game_data, const Uint8* keys);
