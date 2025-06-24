#include "input.h"

#include <SDL_events.h>
#include <SDL_scancode.h>

#include "window.h"

__attribute__((const)) static int procscancode(SDL_Scancode code) {
	switch (code) {
	case SDL_SCANCODE_Q: return MOVRL;
	case SDL_SCANCODE_E: return MOVRR;

	case SDL_SCANCODE_LEFT:
	case SDL_SCANCODE_A:
		return MOVL;

	case SDL_SCANCODE_RIGHT:
	case SDL_SCANCODE_D:
		return MOVR;

	case SDL_SCANCODE_DOWN:
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_SPACE:
		return MOVD;

	case SDL_SCANCODE_ESCAPE:
		window_close();
		return 0;
	default: return 0;
	}
}

int input_getdat(void) {
	int movdat = 0;
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT: window_close(); break;
		case SDL_KEYDOWN: movdat |= procscancode(e.key.keysym.scancode); break;
		}
	}

	return movdat;
}
