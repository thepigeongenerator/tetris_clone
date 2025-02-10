#pragma once

typedef unsigned char error_code;
enum {
    ERROR_MISC = -1,

    SUCCESS = 0,
    ERROR_INIT = 1,

    // SDL errors
    ERROR_SDL = 2,
    ERROR_SDL_INIT = ERROR_SDL | ERROR_INIT,

    // renderer errors
    ERROR_SDL_RENDERER = ERROR_SDL | 4,
    ERROR_SDL_RENDERER_INIT = ERROR_SDL_RENDERER | ERROR_INIT,


    // audio errors
    ERROR_SDL_AUDIO = ERROR_SDL | 8,
    ERROR_SDL_AUDIO_INIT = ERROR_SDL_AUDIO | ERROR_INIT,

    // font errors
    ERROR_SDL_FONT = ERROR_SDL | 16,
    ERROR_SDL_FONT_INIT = ERROR_SDL_FONT | ERROR_INIT,
};

// call when a fatal error has occurred, the program will immediately terminate when called
void error(error_code error_code, char const* format, ...);
void warn(char const* format, ...);
