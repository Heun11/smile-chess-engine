#ifndef CHESS_H
#define CHESS_H

#include"main.h"

#define CHESS_Pawn 1
#define CHESS_Rook 2
#define CHESS_Knight 3
#define CHESS_Bishop 4
#define CHESS_Queen 5
#define CHESS_King 6

/*
    0 -> NULL

    1 -> pawn (-)
    2 -> rook (R)
    3 -> knight (N)
    4 -> bishop (B)
    5 -> queen (Q)
    6 -> king (K)

    p<0 -> BLACK (-6)
    p>0 -> WHILE (+6)
*/

typedef struct{
    int x;
    int y;
}CHESS_Move;

typedef struct{
    CHESS_Move* moves;
    int len;
}CHESS_Moves;

extern SDL_Rect CHESS_WhitePiece[7];
extern SDL_Rect CHESS_BlackPiece[7];
extern SDL_Rect CHESS_MoveIcon;
extern int board[64];

void CHESS_LoadPositionFromFen(char* fen);
void CHESS_LoadPieces();
void CHESS_DrawBoardAndPieces();
void CHESS_MovePiece();

#endif