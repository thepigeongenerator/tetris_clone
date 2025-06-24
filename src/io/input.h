#pragma once

/* 8 bit enumeration storing the movement data */
enum movdat {
	MOVL = 1,   // move left
	MOVR = 2,   // move right
	MOVD = 4,   // move down
	MOVRL = 8,  // move roll left
	MOVRR = 16, // move roll right
	MOVPL = 32, // move place
};

/* returns an OR'd string from `enum movdat`, containing the movement data.
 * assumes that SDL has been initialized and a window has successfully been created. */
__attribute__((__pure__)) int input_getdat(void);
