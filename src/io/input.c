#include "input.h"

#include <SDL_events.h>
#include <SDL_scancode.h>
#include <time.h>

#include "../game/time.h"
#include "../util/types.h"
#include "window.h"

/* processes an incoming scancode, returns the associated movement data, or performs the close action directly
 * NOTE: if the action is mapped to multiple keys, pressing both and then releasing one, disables the action. Minor issue, won't fix. */
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
	// only add to the mask if time_poll returns `1`, negating becomes `-1`; 0b1111...
	// this is masked with the desired movement action.
	msk |= ((MOVR | MOVL | MOVD) & -!!time_poll(time, 64, &timeout));
	msk |= ((MOVRL | MOVRR) & -!!time_poll(time, 100, &timeout_roll));
	return msk;
}

int input_getdat(time_t time) {
	static u8 movdat = 0, nmovdat = 0, lmovdat = 0;   // stores the static movement data
	int mov = movdat, nmov = nmovdat, lmov = lmovdat; // stores the runtime movement data for easy register access

	// process the events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:    window_close(); break;
		case SDL_KEYDOWN: mov |= procscancode(e.key.keysym.scancode); break;
		case SDL_KEYUP:   nmov |= procscancode(e.key.keysym.scancode); break;
		}
	}

	// compute the current movement
	int mask = timeout_mask(time);

	// handle releasing of keys
	mov &= ~(nmov & lmov & mask);         // only remove the keys that have been pressed since lmov
	lmov = (mov & mask) | (lmov & ~mask); // set the value of lmov to the new value mov
	nmov &= mov;                          // set nmov to only those in mov
	int cmov = mov & mask;

	// write to static variables (shrinking the values, and memory usage)
	movdat = mov;
	lmovdat = lmov;
	nmovdat = nmov;
	return cmov;
}
