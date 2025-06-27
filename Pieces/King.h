#ifndef KING_H
#define KING_H
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../ChessGame.h"
#include  "../EvalFunc.h"

bool Castle(enum Piece *board, char *move, bool isWhite);

bool DoKingMove(enum Piece *Board, char *move, bool isWhite);

#endif //KING_H
