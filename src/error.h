#pragma once

#include <stdint.h>
#include <stdnoreturn.h>

/* defines statuses in the 0..127, any higher/negative values are POSIX-reserved.
 * The max value (or -1) shall mean the application is running, anything else shall mean an exit code of some kind */
enum gamestatus {
	// clang-format off
	STATUS_SUCCESS = 0,                // 0; successful exit
	STATUS_ERROR   = 1,                // miscellaneous error
	ERROR_INIT     = STATUS_ERROR | 2, // initialisation error

	ERROR_STD             = STATUS_ERROR   | 64, // standard library error
	ERROR_STD_INIT        = ERROR_INIT     | 64, // standard library initialisation error
	ERROR_STD_MEMORY      = ERROR_STD      | 32, // memory error
	ERROR_STD_MEMORY_INIT = ERROR_STD_INIT | 32, // memory initialization error

	ERROR_SDL                = STATUS_ERROR   | 32, // SDL error
	ERROR_SDL_INIT           = ERROR_INIT     | 32, // SDL initialization error
	ERROR_SDL_RENDERING      = ERROR_SDL      | 16, // rendering error
	ERROR_SDL_RENDERING_INIT = ERROR_SDL_INIT | 16, // rendering initialization error
	ERROR_SDL_AUDIO          = ERROR_SDL      | 8,  // audio error
	ERROR_SDL_AUDIO_INIT     = ERROR_SDL_INIT | 8,  // audio initialization error
	ERROR_SDL_FONT           = ERROR_SDL      | 4,  // font error
	ERROR_SDL_FONT_INIT      = ERROR_SDL_INIT | 4,  // font initialization error

	STATUS_RUNNING = -1,
	// clang-format on
};

#if __INCLUDE_LEVEL__ > 0
#include <SDL_messagebox.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/macro.h"
#endif

#define debug(s, ...) printf("\033[95m" __FILE__ ":" MACRO_STR2(__LINE__) ": [DBG]: " s "\033[0m\n" __VA_OPT__(, __VA_ARGS__))
#define info(s, ...)  printf(__FILE__ ":" MACRO_STR2(__LINE__) ": [INF]: " s "\n", __VA_OPT__(, __VA_ARGS__))
#define warn(s, ...)  fprintf(stderr, "\033[93m" __FILE__ ":" MACRO_STR2(__LINE__) ": [WAR]: " s "\033[0m\n" __VA_OPT__(, __VA_ARGS__))
#define error(s, ...) fprintf(stderr, "\033[91m" __FILE__ ":" MACRO_STR2(__LINE__) ": [ERR]: " s "\033[0m\n" __VA_OPT__(, __VA_ARGS__))

#define fatal(c, s, ...)                                                                                                        \
	do {                                                                                                                        \
		printf("\033[101m" __FILE__ ":" MACRO_STR2(__LINE__) ": [FAT]: " s "\033[0m\n" __VA_OPT__(, __VA_ARGS__));              \
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "something went wrong! :O", "view stderr for full details: \n" s, NULL); \
		exit(c);                                                                                                                \
	} while (0)
