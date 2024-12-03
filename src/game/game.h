#pragma once
#include <SDL2/SDL.h>

typedef Uint16 Shape;

//                                                         0    1    2    3
#define TETROMINO_I     ((Shape)0b1000100010001000)     // 1000 1000 1000 1000      the I tetromino with no rotation
#define TETROMINO_I_90  ((Shape)0b0000000000001111)     // 0000 0000 0000 1111      the I tetromino with a no rotation
#define TETROMINO_O     ((Shape)0b1100110000000000)     // 1100 1100 0000 0000      the O tetromino with no rotation
#define TETROMINO_T     ((Shape)0b1110010000000000)     // 1110 0100 0000 0000      the T tetromino with no rotation
#define TETROMINO_T_90  ((Shape)0b1100110010000000)     // 1100 1100 1000 0000      the T tetromino with a no rotation
#define TETROMINO_T_180 ((Shape)0b0100111000000000)     // 0100 1110 0000 0000      the T tetromino with a 180° rotation
#define TETROMINO_T_270 ((Shape)0b0010011000100000)     // 0010 0110 0010 0000      the T tetromino with a 270° rotation
#define TETROMINO_L     ((Shape)0b1000100011000000)     // 1000 1000 1100 0000      the L tetromino with no rotation
#define TETROMINO_L_90  ((Shape)0b1110100000000000)     // 1110 1000 0000 0000      the L tetromino with a no rotation
#define TETROMINO_L_180 ((Shape)0b1100010001000000)     // 1100 0100 0100 0000      the L tetromino with a 180° rotation
#define TETROMINO_L_270 ((Shape)0b0000001011100000)     // 0000 0010 1110 0000      the L tetromino with a 270° rotation
#define TETROMINO_J     ((Shape)0b0100010011000000)     // 0100 0100 1100 0000      the J tetromino with no rotation
#define TETROMINO_J_90  ((Shape)0b1000111000000000)     // 1000 1110 0000 0000      the J tetromino with a no rotation
#define TETROMINO_J_180 ((Shape)0b1100100010000000)     // 1100 1000 1000 0000      the J tetromino with a 180° rotation
#define TETROMINO_J_270 ((Shape)0b1110001000000000)     // 1110 0010 0000 0000      the J tetromino with a 270° rotation
#define TETROMINO_S     ((Shape)0b0110110000000000)     // 0110 1100 0000 0000      the S tetromino with no rotation
#define TETROMINO_S_90  ((Shape)0b1000110001000000)     // 1000 1100 0100 0000      the S tetromino with a no rotation
#define TETROMINO_Z     ((Shape)0b1100011000000000)     // 1100 0110 0000 0000      the Z tetromino with no rotation
#define TETROMINO_Z_90  ((Shape)0b0100110010000000)     // 0100 1100 1000 0000      the Z tetromino with a no rotation

// stores the data used in the game
#define ROWS 32
typedef struct {
    Uint16 row[ROWS];
} GameData;

// updates the game's state
void game_update(GameData* game_data, const Uint8* keys);
