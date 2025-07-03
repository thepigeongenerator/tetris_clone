#include "shapes.h"

#include "../../io/colour/colour8.h"
#include "../../util/types.h"
#include "../../util/vec.h"
#include "../../error.h"

void shape_getblocks(u8 id, i8vec2* restrict out) {
	struct blockdat {
		u8 ax : 2, ay : 2;
		u8 bx : 2, by : 2;
		u8 cx : 2, cy : 2;
		u8 dx : 2, dy : 2;
	} dat;

	switch (id) {
	// O tetromino
	case TET_O | TET_R0:
	case TET_O | TET_R90:
	case TET_O | TET_R180:
	case TET_O | TET_R270: dat = (struct blockdat){1, 1, 2, 1, 1, 2, 2, 2}; break;

	// I tetromino
	case TET_I | TET_R0:   dat = (struct blockdat){0, 1, 1, 1, 2, 1, 3, 1}; break;
	case TET_I | TET_R90:  dat = (struct blockdat){2, 0, 2, 1, 2, 2, 2, 3}; break;
	case TET_I | TET_R180: dat = (struct blockdat){0, 2, 1, 2, 2, 2, 3, 2}; break;
	case TET_I | TET_R270: dat = (struct blockdat){1, 0, 1, 1, 1, 2, 1, 3}; break;

	// S tetromino
	case TET_S | TET_R0:   dat = (struct blockdat){1, 0, 2, 0, 0, 1, 1, 1}; break;
	case TET_S | TET_R90:  dat = (struct blockdat){1, 0, 1, 1, 2, 1, 2, 2}; break;
	case TET_S | TET_R180: dat = (struct blockdat){1, 1, 2, 1, 0, 2, 1, 2}; break;
	case TET_S | TET_R270: dat = (struct blockdat){0, 0, 0, 1, 1, 1, 1, 2}; break;

	// Z tetromino
	case TET_Z | TET_R0:   dat = (struct blockdat){0, 0, 1, 0, 1, 1, 2, 1}; break;
	case TET_Z | TET_R90:  dat = (struct blockdat){2, 0, 1, 1, 2, 1, 1, 2}; break;
	case TET_Z | TET_R180: dat = (struct blockdat){0, 1, 1, 1, 1, 2, 2, 2}; break;
	case TET_Z | TET_R270: dat = (struct blockdat){1, 0, 0, 1, 1, 1, 0, 2}; break;

	// T tetromino
	case TET_T | TET_R0:   dat = (struct blockdat){0, 1, 1, 1, 2, 1, 1, 2}; break;
	case TET_T | TET_R90:  dat = (struct blockdat){1, 0, 0, 1, 1, 1, 1, 2}; break;
	case TET_T | TET_R180: dat = (struct blockdat){1, 0, 0, 1, 1, 1, 2, 1}; break;
	case TET_T | TET_R270: dat = (struct blockdat){1, 0, 1, 1, 2, 1, 1, 2}; break;

	// L tetromino
	case TET_L | TET_R0:   dat = (struct blockdat){1, 0, 1, 1, 1, 2, 2, 2}; break;
	case TET_L | TET_R90:  dat = (struct blockdat){0, 1, 1, 1, 2, 1, 0, 2}; break;
	case TET_L | TET_R180: dat = (struct blockdat){0, 0, 1, 0, 1, 1, 1, 2}; break;
	case TET_L | TET_R270: dat = (struct blockdat){2, 0, 0, 1, 1, 1, 2, 1}; break;

	// J tetromino
	case TET_J | TET_R0:   dat = (struct blockdat){1, 0, 1, 1, 0, 2, 1, 2}; break;
	case TET_J | TET_R90:  dat = (struct blockdat){0, 0, 0, 1, 1, 1, 2, 1}; break;
	case TET_J | TET_R180: dat = (struct blockdat){1, 0, 2, 0, 1, 1, 1, 2}; break;
	case TET_J | TET_R270: dat = (struct blockdat){0, 1, 1, 1, 2, 1, 2, 2}; break;

	default:
#ifndef NDEBUG
		fatal(1, "something went wrong; couldn't reconise the ID.", );
#endif
		break;
	}

	out[0] = (i8vec2){dat.ax, dat.ay};
	out[1] = (i8vec2){dat.bx, dat.by};
	out[2] = (i8vec2){dat.cx, dat.cy};
	out[3] = (i8vec2){dat.dx, dat.dy};
}

colour8 colour_from_id(u8 id) {
	switch (id & 7) {
	case TET_O: return COLOUR8_YELLOW;
	case TET_I: return COLOUR8_CYAN;
	case TET_S: return COLOUR8_GREEN;
	case TET_Z: return COLOUR8_RED;
	case TET_T: return COLOUR8_MAGENTA;
	case TET_L: return COLOUR8_ORANGE;
	case TET_J: return COLOUR8_BLUE;
	default:    return COLOUR8_BLACK;
	}
}
