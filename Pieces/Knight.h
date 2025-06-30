#ifndef KNIGHT_H
#define KNIGHT_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../EvalFunc.h"
#include "../ChessGame.h"

int *FindKnight(enum Piece *board, int *NewCoord, bool isWhite, int priorityFile);
void GetKnightMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char **FoundMoves);
bool DoKnightMove(enum Piece *board, char *move, bool isWhite);


#endif //KNIGHT_H
