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

    unsigned key_len = 0;   // length of the key data
    unsigned val_len = 0;   // length of the value data
    char const* key = NULL; // the pointer for the start of the key
    char const* val = NULL; // the pointer for the start of the value
    bool feq = false;       // whether we've found the equal sign
    bool eol = false;       // whether we've reached the end of the line/string, so we don't read what should be comments as data, etc.
    bool nodat = false;     // whether we are done and can just look for an EOL indicator

    // read the file line by line
    char buf[BUF_SIZE]; // buffer will contain \0 terminated string. fgets stores into the buffer until \n or EOF
    while (fgets(buf, BUF_SIZE, fptr)) {
        // reset if EOL has been reached
        if (eol) {
            key_len = 0;
            val_len = 0;
            key = NULL;
            val = NULL;
            feq = false;
            eol = false;
            nodat = false;
        }

        // loop through each character
        for (unsigned i = 0; i < BUF_SIZE; i++) {
            // check EOL / EOF indicators (or just end of string) (CRLF vs LF doesn't matter here, since both end with \n)
            if (buf[i] == '\n' || buf[i] == '\0') {
                eol = true;
                break;
            }
            if (nodat) continue;

            // ignore whitespace, to acquire a correct starting point
            if (buf[i] == ' ' || buf[i] == '\t') {
                if (val) nodat = true; // don't continue processing if we encounter a space after starting val
                continue;
            }

            // ignore comments
            if (buf[i] == '#') {
                nodat = true;
                continue;
            }

            // increment the key/value length if set
            if (val) val_len++; // first check if value is set, as it is set last
            else if (key) key_len++;

            // get the values
            if (!key) key = &buf[i];              // store the key's pointer at this point
            else if (!feq) feq = (buf[i] == '='); // otherwise store whether we've found the equal sign
            else if (!val) val = &buf[i];         // otherwise store the start of the value pointer
            else { }
        }

        // ignore if either the key or value haven't been set
        //  NOTE: eol might not be set at this point, this is intentional so we don't accidentally read a comment outside the buffer size as a key-value pair
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
