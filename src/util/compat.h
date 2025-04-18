#pragma once

#if defined __unix__
# include <unistd.h>
#elif defined _WIN32
# include <io.h>
#else
# error platform not supported!
#endif

// define the constants if they haven't been
#ifndef F_OK
# define F_OK 0
#endif
#ifndef X_OK
# define X_OK 1
#endif
#ifndef W_OK
# define W_OK 2
#endif
#ifndef R_OK
# define R_OK 4
#endif

enum faccess_perms {
    FA_F = F_OK, // test for file's existence
    FA_X = X_OK, // test for executing permission
    FA_W = W_OK, // test for write permissions
    FA_R = R_OK, // test for read permissions
};

// tests a files access with F_OK, X_OK, R_OK, W_OK OR'd together
static inline int faccess(char const* restrict fname, int perms) {
#if defined __unix__
    return !access(fname, perms);
#elif defined _WIN32
    return !_access(fname, perms);
#endif
}
