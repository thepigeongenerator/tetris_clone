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

__attribute__((nonnull(1))) __attribute__((format(printf, 1, 2))) void debug(char const* restrict, ...); // prints a debug message to stdout if the DEBUG environment variable is set, otherwise the call is ignored.
__attribute__((nonnull(1))) __attribute__((format(printf, 1, 2))) void info(char const* restrict, ...);  // prints an info message to stdout
__attribute__((nonnull(1))) __attribute__((format(printf, 1, 2))) void warn(char const* restrict, ...);  // prints a warning message to stderr
__attribute__((nonnull(1))) __attribute__((format(printf, 1, 2))) void error(char const* restrcit, ...); // prints an warning message to stderr

// prints an error message to stderr before exiting
__attribute__((nonnull(2, 5))) __attribute__((format(printf, 4, 5))) noreturn void fatal(unsigned, char const* restrict file_name, unsigned line, char const* restrict fmt, ...);
