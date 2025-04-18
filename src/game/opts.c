#include "../error.h"
#include "../util/compat.h"
#include "paths.h"

/* attempts to load the options,
   returns 1 upon failure */
int load_opts(void) {
    if (!path_opts) return 1;
    if (!faccess(path_opts, FA_W | FA_R)) {
        error("attempted to load opts file, but the given path wasn't writable");
        return 1;
    }

    // TODO: load opts from the file
    // the options shall be loaded as key-value-pairs
    // lines starting with # are seen as comments
    // the keys are stored on the left size, and the values on the right.
    // if a field couldn't be parsed the program is not allowed to fail. I'd say we fix the field for the user.

    return 0;
}
