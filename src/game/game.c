#include "game.h"

#include "../main.h"


static void add_shape(const Shape shape, Uint16* row) {
    for (int i = 0; i < 4; i++) {
        Uint8 rowData = (shape >> (4 * i)) & 0x0F;

        if (rowData != 0)
            row[4 - i] |= rowData;
    }
}

// called every time the game's state is updated
void game_update(GameData* game_data, const Uint8* keys) {
    if (keys[SDL_SCANCODE_ESCAPE]) {
        stop();
    }

    add_shape(TETROMINO_I, &game_data->row[4]);
}
