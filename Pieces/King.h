#ifndef KING_H
#define KING_H
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../ChessGame.h"
#include  "../EvalFunc.h"

int *FindKing(enum Piece *board, int *NewCoord, bool isWhite);
void GetKingMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char ***FoundMoves);
bool DoKingMove(enum Piece *Board, char *move, bool isWhite);
bool Castle(enum Piece *board, char *move, bool isWhite);
#endif //KING_H
