#ifndef ROOK_H
#define ROOK_H
#include <stdbool.h>
#include <stdlib.h>
#include "../EvalFunc.h"
#pragma once

int *FindRook(enum Piece const *Board, int const *Coord, bool const SearchWhite, int const PreferredRank);
void GetRookMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char **FoundMoves);
bool DoRookMove(enum Piece *Board, char *move, bool WhitePlay);

#endif //ROOK_H
