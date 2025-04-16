#include "error.h"

#include <SDL_messagebox.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#define PRINT_BUFFER_SIZE 128 // defines the buffer size for printing

// writes the arguments to the specified buffer
// using a macro instead of an inline function because fmt otherwise gets horribly messed up
#define write_args(buf, fmt)                            \
    va_list args;                                       \
    va_start(args, fmt);                                \
    (void)vsnprintf(buf, PRINT_BUFFER_SIZE, fmt, args); \
    va_end(args);

void debug(char const* restrict fmt, ...) {
    char const* env = getenv("DEBUG");
    if (env == NULL || *env != '1')
        return;

    char buf[PRINT_BUFFER_SIZE] = {0};
    write_args(buf, fmt);

    (void)fprintf(stdout, "\033[95m%s\033[0m\n", buf);
}

void info(char const* restrict fmt, ...) {
    char buf[PRINT_BUFFER_SIZE] = {0};
    write_args(buf, fmt);
    (void)fprintf(stdout, "\033[0m%s\033[0m\n", buf); // write colour here for consistency
}

void warn(char const* restrict fmt, ...) {
    char buf[PRINT_BUFFER_SIZE] = {0};
    write_args(buf, fmt);
    (void)fprintf(stderr, "\033[93mW: %s\033[0m\n", buf);
}

void error(char const* restrict fmt, ...) {
    char buf[PRINT_BUFFER_SIZE] = {0};
    write_args(buf, fmt);
    (void)fprintf(stderr, "\033[91mE: %s\033[0m\n", buf);
}

noreturn void fatal(unsigned error_code, char const* restrict fname, unsigned ln, char const* restrict fmt, ...) {
    char buf1[PRINT_BUFFER_SIZE] = {0};
    write_args(buf1, fmt);

    char buf2[PRINT_BUFFER_SIZE * 2] = {0};
    sprintf(buf2, "%s\n    at %s:%u (exitcode: %u)", buf1, fname, ln, error_code);

    (void)fprintf(stderr, "\033[101mF: %s\033[0m\n", buf2);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "something went wrong! :O", buf2, NULL);

    // set status, but exit immediately, as code is not allowed to execute beyond this point
    exit(error_code);
}
