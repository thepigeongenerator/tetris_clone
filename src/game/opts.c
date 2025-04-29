#include "opts.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* contains the basic structure for a string */
struct str {
    char* dat;    // pointer to the string data
    unsigned cap; // current string capacity
};

struct proc_buf_dat {
    struct str pkey;          // contains a memory address for storing the key string, the memory stored grows via 2ˣ
    struct str pval;          // contains a memory address for storing the val string, the memory stored grows via 2ˣ
    char const* restrict key; // the pointer for the start of the key
    char const* restrict val; // the pointer for the start of the value
    unsigned pkey_len;        // string length of the malloc'd key when they got malloc'd
    unsigned pval_len;        // string length of the malloc'd val when they got malloc'd
    unsigned key_len;         // string length of the key data
    unsigned val_len;         // string length of the value data
    bool feq;                 // whether we've found the equal sign and are looking for the value
    bool eol;                 // whether we've reached the end of the line/string, so we don't read what should be comments as data, etc.
    bool nodat;               // whether we are done and can just look for an EOL indicator
};

/* appends src onto dest.dat, if test.dat hasn't been allocated, or does not have enough space.
   The size is the nearest 2ˣ rounded-up value of src_len.
   returns 0 upon success, 1 upon failure. */
__attribute__((nonnull(1, 3))) static inline int str_put(struct str* restrict dest, size_t dest_len, char const* restrict src, size_t src_len) {
    // check if (re)allocation needs to occur
    if (dest->cap <= src_len) {                       // check for equality as well due to `\0`
        dest->cap = src_len;                          // using src_len as-is, since it is string length, thus already one short.
        for (unsigned i = 1; i < SIZE_WIDTH; i <<= 1) // then loop through each 2ˣ bit in size_t
            dest->cap |= dest->cap >> i;              // OR together the shifted result (shifted 1, 2, 4, 8, 16 on 32 bit systems)
        dest->cap++;                                  // round to the most significant bit (0111 -> 1000)

        // (re)allocate the array
        if (!dest->dat) {
            dest->dat = malloc(dest->cap); // allocate memory for this capacity
            if (!dest->dat) return 1;      // return 1 upon failure
        } else {
            void* ptr = realloc(dest->dat, dest->cap); // reallocate to the new capacity
            if (!ptr) {
                free(dest->dat); // free up resources by the old (still valid) pointer, and return failure
                return 1;
            }
            dest->dat = ptr;
        }
    }


    // copy the missing data to the end of the destination
    memcpy(dest->dat + dest_len, src, src_len - dest_len);
    dest->dat[src_len] = '\0'; // null-terminate the destination
    return 0;
}

/* processes the data within the buffer.
   NOTE: when the function returns, eol might not be set, this is to ensure that we know for certain that an EOL indicator has been reached. */
static void proc_buf(char const* restrict buf, struct proc_buf_dat* restrict dat) {
    // reset if EOL has been reached
    if (dat->eol) {
        // zero-initialize all except the pointers; keep the pointers.
        *dat = (struct proc_buf_dat){
            .pkey = dat->pkey,
            .pval = dat->pval,
        };
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
        if (str_put(&dat->pval, dat->pval_len, dat->val, dat->val_len))
            fatal(ERROR_STD_MEMORY_INIT, __FILE_NAME__, __LINE__, "something went wrong when attempting to allocate space for the option string");
        dat->pval_len = dat->val_len;
    } else if (dat->pkey_len < dat->key_len) {
        if (str_put(&dat->pkey, dat->pkey_len, dat->key, dat->key_len))
            fatal(ERROR_STD_MEMORY_INIT, __FILE_NAME__, __LINE__, "something went wrong when attempting to allocate space for the option string");
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
    if (!path_opts) {
        error("the variable to the path to the options failed to initialize");
        return 1;
    }

    if (faccess(path_opts, FA_R)) {
        warn("attempted to load '%s', but the given path wasn't readable or doesn't exist", path_opts);
        return 1;
    }

    // open the file in read mode
    FILE* fptr = fopen(path_opts, "r");
    if (!fptr) {
        error("failed to open '%s'", path_opts);
        return 1;
    }

    debug("loading options configuration from '%s'...", path_opts);

    struct proc_buf_dat dat = {0};

    // read the file line by line
    char buf[BUF_SIZE]; // buffer will contain \0 terminated string. fgets stores into the buffer until \n or EOF
    while (fgets(buf, BUF_SIZE, fptr)) {
        // process the data in the current buffer
        proc_buf(buf, &dat);
        if (dat.pkey.dat) debug("key: '%s'", dat.pkey.dat);
        if (dat.pval.dat) debug("val: '%s'", dat.pval.dat);

        // ignore if EOL hasn't been reached yet, due to the built up data possibly being incomplete
        if (!dat.eol) continue;

        if (!dat.pkey.dat || !dat.pval.dat) continue;
        else if (!!strcmp(dat.pkey.dat, "sensitivity")) load_opt(float, opts.sensitivity, dat.pkey.dat, dat.pval.dat, strtof32, );
        else if (!!strcmp(dat.pkey.dat, "sensitivity_roll")) load_opt(float, opts.sensitivity_roll, dat.pkey.dat, dat.pval.dat, strtof32, );
        else if (!!strcmp(dat.pkey.dat, "colour_O")) load_opt(colour8, opts.colour_O, dat.pkey.dat, dat.pval.dat, strtol, , 16);
        else if (!!strcmp(dat.pkey.dat, "colour_I")) load_opt(colour8, opts.colour_I, dat.pkey.dat, dat.pval.dat, strtol, , 16);
        else if (!!strcmp(dat.pkey.dat, "colour_S")) load_opt(colour8, opts.colour_S, dat.pkey.dat, dat.pval.dat, strtol, , 16);
        else if (!!strcmp(dat.pkey.dat, "colour_Z")) load_opt(colour8, opts.colour_Z, dat.pkey.dat, dat.pval.dat, strtol, , 16);
        else if (!!strcmp(dat.pkey.dat, "colour_T")) load_opt(colour8, opts.colour_T, dat.pkey.dat, dat.pval.dat, strtol, , 16);
        else if (!!strcmp(dat.pkey.dat, "colour_L")) load_opt(colour8, opts.colour_L, dat.pkey.dat, dat.pval.dat, strtol, , 16);
        else if (!!strcmp(dat.pkey.dat, "colour_J")) load_opt(colour8, opts.colour_J, dat.pkey.dat, dat.pval.dat, strtol, , 16);
        else error("unknown key-value pair: '%s = %s'", dat.pkey.dat, dat.pval.dat);
        // TODO: load opts from the file
        // the options shall be loaded as key-value-pairs
        // lines starting with # are seen as comments
        // the keys are stored on the left size, and the values on the right.
        // if a field couldn't be parsed the program is not allowed to fail. I'd say we fix the field for the user.
    }

    free(dat.pkey.dat);
    free(dat.pval.dat);
    fclose(fptr);
    return 0;
}
