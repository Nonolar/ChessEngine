#ifndef PAWN_H
#define PAWN_H
#include <stdlib.h>
#include <stdbool.h>
#pragma once

void FindSinglePawnCapture(enum Piece const *Board, int const *NewCoord, int *OldCoord, bool IsWhite);
bool DoPawnMove(enum Piece *Board, char move[5], bool WhiteTurn);

#endif //PAWN_H
