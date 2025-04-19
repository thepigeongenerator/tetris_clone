#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../error.h"
#include "../util/compat.h"
#include "paths.h"

#define BUF_SIZE 32

struct proc_buf_dat {
    char const* restrict key; // length of the key data
    char const* restrict val; // length of the value data
    unsigned key_len;         // the pointer for the start of the key
    unsigned val_len;         // the pointer for the start of the value
    bool feq;                 // whether we've found the equal sign and are looking for the value
    bool eol;                 // whether we've reached the end of the line/string, so we don't read what should be comments as data, etc.
    bool nodat;               // whether we are done and can just look for an EOL indicator
};

/* processes the data within the buffer.
   NOTE: when the function returns, eol might not be set, this is to ensure that we know for certain that an EOL indicator has been reached. */
static void proc_buf(char const* restrict buf, struct proc_buf_dat* restrict dat) {
    // reset if EOL has been reached
    if (dat->eol) *dat = (struct proc_buf_dat){0};

    // loop through each character
    for (unsigned i = 0; i < BUF_SIZE; i++) {
        // check EOL / EOF indicators (or just end of string) (CRLF vs LF doesn't matter that much here, since both end with \n)
        if (buf[i] == '\n' || buf[i] == '\0') {
            dat->eol = true;
            break;
        }
        if (dat->nodat) continue; // just continue until we've found an EOL/EOF indicator

        // ignore whitespace, to acquire a correct starting point
        if (buf[i] == ' ' || buf[i] == '\t') {
            if (dat->val) dat->nodat = true; // don't continue processing if we encounter a space after starting val
            continue;
        }

        // ignore comments
        if (buf[i] == '#') {
            dat->nodat = true;
            continue;
        }

        // increment the key/value length if set
        if (dat->val) dat->val_len++; // first check if value is set, as it is set last
        else if (dat->key) dat->key_len++;

        // get the values
        if (!dat->key) dat->key = &buf[i];              // store the key's pointer at this point
        else if (!dat->feq) dat->feq = (buf[i] == '='); // otherwise store whether we've found the equal sign
        else if (!dat->val) dat->val = &buf[i];         // otherwise store the start of the value pointer
    }
}

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

    struct proc_buf_dat dat = {0};

    // read the file line by line
    char buf[BUF_SIZE]; // buffer will contain \0 terminated string. fgets stores into the buffer until \n or EOF
    while (fgets(buf, BUF_SIZE, fptr)) {
        // process the data in the current buffer
        proc_buf(buf, &dat);

        // ignore if either the key or value haven't been set
        if (!dat.key || !dat.val) continue;
        // TODO: load opts from the file
        // the options shall be loaded as key-value-pairs
        // lines starting with # are seen as comments
        // the keys are stored on the left size, and the values on the right.
        // if a field couldn't be parsed the program is not allowed to fail. I'd say we fix the field for the user.
    }

    fclose(fptr);
    return 0;
}
