#ifndef QUEEN_H
#define QUEEN_H
#include <stdbool.h>
#include <stdlib.h>
#include "../EvalFunc.h"

int *FindQueen(enum Piece const *board, int const *NewCoord, enum Piece const ToFind, int PriorityFile);
void GetQueenMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char ***FoundMoves);
bool DoQueenMove(enum Piece *board, char *move, bool isWhite);

#endif //QUEEN_H
