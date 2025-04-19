#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../error.h"
#include "../util/compat.h"
#include "paths.h"

#define BUF_SIZE 32

/* attempts to load the options,
   returns 1 upon failure */
int load_opts(void) {
    if (!path_opts) return 1;
    if (faccess(path_opts, FA_R)) {
        error("attempted to load opts file, but the given path wasn't readable");
        return 1;
    }

    // open the file in read mode
    FILE* fptr = fopen(path_opts, "r");
    if (!fptr) return 1;

    // read the file line by line
    char buf[BUF_SIZE]; // buffer will contain \0 terminated string. fgets stores into the buffer until \n or EOF
    while (fgets(buf, BUF_SIZE, fptr)) {
        unsigned key_len = 0;
        unsigned val_len = 0;
        char* key = NULL;
        char* val = NULL;
        bool feq = false; // whether we found the equal sign

        for (unsigned i = 0; i < BUF_SIZE; i++) {
            // handling of special characters (in the order of most common to least)
            if (buf[i] == ' ') continue;  // spaces are ignored; neither keys or values can contain spaces
            if (buf[i] == '\t') continue; // ^ same with tabs
            if (buf[i] == '\n') break;    // don't check \0, since if \n isn't present, the string will be BUF_SIZE length
            if (buf[i] == '#') break;     // stop the rest of the data is a comment

            // increment the key/value length if set
            if (val) val_len++; // first check if value is set, as it is set last
            else if (key) key_len++;

            // get the values
            if (!key) key = &buf[i];              // store the key's pointer at this point
            else if (!feq) feq = (buf[i] == '='); // otherwise store whether we've found the equal sign
            else if (!val) val = &buf[i];       // otherwise store the start of the value pointer PERF: we can optimise this further by stopping the loop as soon as we reach another space at this point
            else { }
        }

        // ignore if either the key or value haven't been set
        if (!key || !val) continue;
        // TODO: load opts from the file
        // the options shall be loaded as key-value-pairs
        // lines starting with # are seen as comments
        // the keys are stored on the left size, and the values on the right.
        // if a field couldn't be parsed the program is not allowed to fail. I'd say we fix the field for the user.
    }

    fclose(fptr);
    return 0;
}
