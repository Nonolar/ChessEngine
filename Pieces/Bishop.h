#ifndef BISHOP_H
#define BISHOP_H
#include <stdbool.h>
#include <stdlib.h>
#include "../EvalFunc.h"
#pragma once
int *FindBishop(enum Piece const *Board, int const *NewCoord, bool const White, int const PriorityFile);
void GetBishopMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char ***FoundMoves);
bool DoBishopMove(enum Piece *Board, char Move[5], bool IsWhite);

#endif //BISHOP_H
