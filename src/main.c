#include <SDL.h>
#include <stdlib.h>

#include "error.h"
#include "game/game.h"
#include "io/window.h"

static void stop(void) {
	debug("stopping...", );
	window_close();
	window_free();
	SDL_Quit();
}

// entry-point of the application
int main(int argc, char** argv) {
	(void)argc, (void)argv;
	// register stop as exit function
	atexit(stop);

	window_init(game_init());
	window_open();

	return 0;
}
