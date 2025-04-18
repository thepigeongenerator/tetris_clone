#pragma once

extern char const* restrict path_dat;
extern char const* restrict path_opts;
extern char const* restrict path_font;
extern char const* restrict path_music;
extern char const* restrict path_place_sfx;

/* initializes the paths, paths are evaluated to NULL upon failure.
   returns 0 upon success, >0 upon failure <0 upon non-critical failure */
int paths_init(void);

/* frees allocated data */
void paths_free(void);
