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

    STATUS_RUNNING = -1,
    // clang-format on
};
typedef int8_t gamestatus;

void set_gamestatus(gamestatus); // sets the current status of the game
gamestatus get_gamestatus(void); // gets the current status of the game
void debug(char const*, ...);    // prints a debug message to stdout if the DEBUG environment variable is set, otherwise the call is ignored.
void info(char const*, ...);     // prints an info message to stdout
void warn(char const*, ...);     // prints a warning message to stderr
void error(char const*, ...);    // prints an warning message to stderr

// prints an error message to stderr before exiting
noreturn void fatal(gamestatus, char const* fmt, ...);
