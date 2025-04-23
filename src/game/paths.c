#include "paths.h"

#include <stdlib.h>
#include <string.h>

#include "../util/compat.h"

char const* restrict path_dat = NULL;
char const* restrict path_opts = NULL;
char const* restrict path_font = NULL;
char const* restrict path_music = NULL;
char const* restrict path_place_sfx = NULL;

// gets the game's data path, returns 0 on failure, otherwise the datapath's string length
static unsigned getdatpath(void) {
    char const* home = getenv(unixonly("HOME") winonly("APPDATA")); // get the user data directory path (appropriated for each platform)
    if (!home) home = ".";                                          // if this failed, set the path to `.`, which represents cwd

    unsigned len = strlen(home);
    len += 1 + 20 unixonly(+13); // add 21 bytes to the home length, to account for adding .local/share later
    char* datpath = malloc(len);
    if (!datpath) return 0;

    // copy the data from home into the datapath
    strcpy(datpath, home);

#ifdef __unix__
    // include the .local/share directory, if the HOME environment variable was valid
    if (home[0] != '.') strcat(datpath, "/.local/share");
    else {
        // if the HOME directory wasn't defined, shrink the string
        len -= 13;
        void* ptr = realloc(datpath, len);
        if (ptr) datpath = ptr; // likely doesn't actually change the pointer, but just to be sure
    }
#endif

    strcat(datpath, PATH_SEP_STR "quinns_tetris_clone");
    path_dat = datpath;
    return len;
}

static inline char const* init_path(char const* const restrict str, unsigned len) {
    void* ptr = malloc(len);
    if (!ptr) return NULL;
    strcpy(ptr, path_dat);
    strcat(ptr, str);
    return ptr;
}

int paths_init(void) {
    unsigned len = getdatpath();
    if (!len) return 1;

    // these are explicitly static, as string literals just work like that
    path_opts = init_path("/opts.cfg", len + 9);                      // TODO: shouldn't opts be stored at .config/?
    path_font = init_path("/pixeldroid_botic-regular.ttf", len + 29); // TODO: these three paths should not be stored like opts
    path_music = init_path("/korobeiniki.wav", len + 16);
    path_place_sfx = init_path("place.wav", len + 10);
    return -(!path_opts || !path_font || !path_music || !path_place_sfx);
}

void paths_free(void) {
    free((void*)path_dat), path_dat = NULL;
    free((void*)path_opts), path_opts = NULL;
    free((void*)path_music), path_music = NULL;
    free((void*)path_place_sfx), path_place_sfx = NULL;
}
