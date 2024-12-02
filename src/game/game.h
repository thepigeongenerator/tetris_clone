#pragma once
#include <SDL2/SDL.h>

// stores the data used in the game
typedef struct {
} GameData;

// updates the game's state
void game_update(GameData game_data, const Uint8* keys);
