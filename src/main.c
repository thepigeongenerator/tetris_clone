#include "main.h"

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#include "SDL_ttf.h"
#include "errors.h"
#include "game/game.h"
#include "window/renderer.h"

#ifdef __EMSCRIPTEN__ // for web builds
#include <emscripten.h>
#endif


bool playing = true;

render_data render_dat = {0};
game_data game_dat = {0};


// handles game application initialisation
static void init(void) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        error(ERROR_SDL_INIT, "SDL could not initialize! SDL Error: %s", SDL_GetError());
    if (TTF_Init() != 0)
        error(ERROR_SDL_FONT_INIT, "the TTF module of SDL could not initialize! TTF Error: %s", TTF_GetError());

    // initialize units
    game_init(&game_dat);
    renderer_init(&render_dat, &game_dat);
}

// handles game application updating
static void update(void) {
    // update the input
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                exit(SUCCESS);
                break;
            }
        }
    }

    // preform updates
    game_update(&game_dat, SDL_GetKeyboardState(NULL));
    renderer_update(&render_dat);
}

// handles game application quitting
void stop(void) {
    playing = false;
}

// entry point of the application
int main(int const argc, char const* const* const argv) {
    (void)argc;
    (void)argv;

    init();

    printf("target framerate: %u\n", 0);

    while (playing) {
        update();
    }

    // cleanup of resources
    game_free(&game_dat);
    SDL_Quit();

    return 0;
}
