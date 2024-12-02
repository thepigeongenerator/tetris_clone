#include "game.h"

#include "../main.h"


// called every time the game's state is updated
void game_update(GameData game_data, const Uint8* keys) {
    if (keys[SDL_SCANCODE_ESCAPE]) {
        stop();
    }
}
