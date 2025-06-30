#include "input.h"

#include <SDL_events.h>
#include <SDL_scancode.h>
#include <time.h>

#include "../game/time.h"
#include "window.h"

/* processes an incoming scancode, returns the associated movement data, or performs the close action directly */
__attribute__((const)) static int procscancode(SDL_Scancode code) {
	switch (code) {
	case SDL_SCANCODE_Q:
		return MOVRL;

	case SDL_SCANCODE_E:
		return MOVRR;

	case SDL_SCANCODE_LEFT:
	case SDL_SCANCODE_A:
	case SDL_SCANCODE_H:
		return MOVL;

	case SDL_SCANCODE_RIGHT:
	case SDL_SCANCODE_D:
	case SDL_SCANCODE_L:
		return MOVR;

	case SDL_SCANCODE_DOWN:
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_J:
	case SDL_SCANCODE_SPACE:
		return MOVD;

	case SDL_SCANCODE_ESCAPE:
		window_close();
		return 0;
	default: return 0;
	}
}

int input_getdat(time_t time) {
	static int movdat = 0, nmovdat = 0;
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT: window_close(); break;
		case SDL_KEYDOWN: movdat |= procscancode(e.key.keysym.scancode); break;
		case SDL_KEYUP: nmovdat |= procscancode(e.key.keysym.scancode); break;
		}
	}

	static time_t timeout = 0, timeout_roll = 0;
	int mask = 0;
	mask |= ((MOVR | MOVL | MOVD) & -!!time_poll(time, 64, &timeout));
	mask |= ((MOVRL | MOVRR) & -!!time_poll(time, 164, &timeout_roll));

	int cmov = movdat & mask;
	nmovdat &= movdat;
	movdat &= ~(nmovdat & mask);
	return cmov;
}
