#include "opts.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error.h"
#include "../util/compat.h"
#include "../window/colour/colour8.h"
#include "paths.h"

#define BUF_SIZE 32

struct opts opts = {
    1.0F,            // movement sensitivity
    1.0F,            // roll sensitivity
    COLOUR8_YELLOW,  // colour for the O shape
    COLOUR8_CYAN,    // colour for the I shape
    COLOUR8_RED,     // colour for the S shape
    COLOUR8_GREEN,   // colour for the Z shape
    COLOUR8_MAGENTA, // colour for the T shape
    COLOUR8_ORANGE,  // colour for the L shape
    COLOUR8_BLUE,    // colour for the J shape
};

struct proc_buf_dat {
    char const* restrict key; // the pointer for the start of the key
    char const* restrict val; // the pointer for the start of the value
    char* restrict pkey;      // pointer to a malloc'd key
    char* restrict pval;      // pointer to a malloc'd val
    unsigned key_len;         // string length of the key data
    unsigned val_len;         // string length of the value data
    unsigned pkey_len;        // string length of the malloc'd key when they got malloc'd
    unsigned pval_len;        // string length of the malloc'd val when they got malloc'd
    bool feq;                 // whether we've found the equal sign and are looking for the value
    bool eol;                 // whether we've reached the end of the line/string, so we don't read what should be comments as data, etc.
    bool nodat;               // whether we are done and can just look for an EOL indicator
};

/* use malloc to append src to the end of dest.
   if *dest == NULL, memory is allocated, otherwise it is reallocated.
   returns 0 upon success, 1 upon failure. */
__attribute__((nonnull(1, 3))) static inline int str_put(char* restrict* restrict dest, size_t dest_len, char const* restrict src, size_t src_len) {
    if (!*dest) {
        *dest = malloc(src_len + 1);
        if (!*dest) return 1;
    } else {
        assert(src_len > dest_len); // sanity check
        void* ptr = realloc(*dest, src_len + 1);
        if (!ptr) return 1;
        *dest = ptr;
    }

    // copy the missing data to the end of the destination
    memcpy(*dest + dest_len, src + dest_len, src_len - dest_len);
    (*dest)[src_len] = '\0'; // null-terminate the destination
    return 0;
}

/* processes the data within the buffer.
   NOTE: when the function returns, eol might not be set, this is to ensure that we know for certain that an EOL indicator has been reached. */
static void proc_buf(char const* restrict buf, struct proc_buf_dat* restrict dat) {
    // reset if EOL has been reached
    if (dat->eol) {
        free(dat->pkey);
        free(dat->pval);
        *dat = (struct proc_buf_dat){0};
    }

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

    // allocate memory for the resulting string(s)
    if (dat->pval_len < dat->val_len) { // first check this condition, as key won't have changed if this is true
        str_put(&dat->pval, dat->pval_len, dat->val, dat->val_len);
        dat->pval_len = dat->val_len;
    } else if (dat->pkey_len < dat->key_len) {
        str_put(&dat->pkey, dat->pkey_len, dat->key, dat->key_len);
        dat->pkey_len = dat->key_len;
    }
}

#define load_opt(_t, _opt, _key, _val, _fn, ...)                                       \
    do {                                                                               \
        errno = 0;                                                                     \
        char* end;                                                                     \
        _t res = _fn(_val, &end __VA_ARGS__);                                          \
        if (end != _val && errno != ERANGE && *end == '\0') {                          \
            _opt = res;                                                                \
            debug("loading opt %s", _key);                                             \
        } else                                                                         \
            error("ignoring invalid option value from '%s'; value: '%s'", _key, _val); \
    } while (0)

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

        // ignore if EOL hasn't been reached yet, due to the built up data possibly being incomplete
        if (!dat.eol) continue;
        if (!dat.pkey || !dat.pval) continue;
        else if (!!strcmp(dat.pkey, "sensitivity")) load_opt(float, opts.sensitivity, dat.pkey, dat.pval, strtof32, );
        else if (!!strcmp(dat.pkey, "sensitivity_roll")) load_opt(float, opts.sensitivity_roll, dat.pkey, dat.pval, strtof32, );
        else if (!!strcmp(dat.pkey, "colour_O")) load_opt(colour8, opts.colour_O, dat.pkey, dat.pval, strtol, , 16);
        else if (!!strcmp(dat.pkey, "colour_I")) load_opt(colour8, opts.colour_I, dat.pkey, dat.pval, strtol, , 16);
        else if (!!strcmp(dat.pkey, "colour_S")) load_opt(colour8, opts.colour_S, dat.pkey, dat.pval, strtol, , 16);
        else if (!!strcmp(dat.pkey, "colour_Z")) load_opt(colour8, opts.colour_Z, dat.pkey, dat.pval, strtol, , 16);
        else if (!!strcmp(dat.pkey, "colour_T")) load_opt(colour8, opts.colour_T, dat.pkey, dat.pval, strtol, , 16);
        else if (!!strcmp(dat.pkey, "colour_L")) load_opt(colour8, opts.colour_L, dat.pkey, dat.pval, strtol, , 16);
        else if (!!strcmp(dat.pkey, "colour_J")) load_opt(colour8, opts.colour_J, dat.pkey, dat.pval, strtol, , 16);
        else error("unknown key-value pair: '%s = %s'", dat.pkey, dat.pval);
        // TODO: load opts from the file
        // the options shall be loaded as key-value-pairs
        // lines starting with # are seen as comments
        // the keys are stored on the left size, and the values on the right.
        // if a field couldn't be parsed the program is not allowed to fail. I'd say we fix the field for the user.
    }

    free(dat.pkey);
    free(dat.pval);
    fclose(fptr);
    return 0;
}
