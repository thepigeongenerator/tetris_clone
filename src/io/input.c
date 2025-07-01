#include "input.h"

#include <SDL_events.h>
#include <SDL_scancode.h>
#include <time.h>

#include "../game/time.h"
#include "../util/types.h"
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

static int timeout_mask(time_t time) {
	static time_t timeout = 0, timeout_roll = 0;
	int msk = 0;
	msk |= ((MOVR | MOVL | MOVD) & -!!time_poll(time, 64, &timeout));
	msk |= ((MOVRL | MOVRR) & -!!time_poll(time, 100, &timeout_roll));
	return msk;
}

// NOTE: if an action is mapped to multiple keys, pressing both and releasing one will cause the action to be disabled. Minor issue, Won't fix.
int input_getdat(time_t time) {
	static u8 movdat = 0, nmovdat = 0;
	int mov = movdat, nmov = nmovdat;

	// process the event
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT: window_close(); break;
		case SDL_KEYDOWN: mov |= procscancode(e.key.keysym.scancode); break;
		case SDL_KEYUP: nmov |= procscancode(e.key.keysym.scancode); break;
		}
	}

	// compute the current movement
	int mask = timeout_mask(time);
	int cmov = movdat & mask;

	// handle releasing of keys
	nmovdat &= movdat;
	movdat &= ~(nmovdat & mask);

	// write to static variables (shrinking the values, and memory usage)
	movdat = mov;
	nmovdat = nmov;

	return cmov;
}
