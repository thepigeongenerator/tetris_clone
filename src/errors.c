#include "errors.h"

#include <SDL_messagebox.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_LEN 128

void error(const ErrorCode error_code, const char* format, ...) {
    char buffer[MAX_STR_LEN] = {0}; // contains the buffer of the final string

    va_list args = {0};
    va_start(args, format);
    vsnprintf(buffer, MAX_STR_LEN, format, args);
    va_end(args);

    printf("\033[91mE\033[0m: %s\n", buffer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "something went wrong! :O", buffer, NULL);

    exit(error_code);
}

void warn(const char* format, ...) {
    char buffer[MAX_STR_LEN] = {0}; // contains the buffer of the final string

    va_list args = {0};
    va_start(args, format);
    vsnprintf(buffer, MAX_STR_LEN, format, args);
    va_end(args);

    printf("\033[93mW\033[0m: %s\n", buffer);
}
