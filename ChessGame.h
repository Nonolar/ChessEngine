#ifndef CHESSGAME_H
#define CHESSGAME_H
#define INVALID_COORDINATE -999
#define NOT_FOUND -900
#define SIDE_lENGHT 8
#include <stdbool.h>
#include "EvalFunc.h"
#pragma once

enum Types {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
};

int LetterToCoordinate(char letter);
bool IsLegitCoordinate(int *Coordinate);
bool IsTileLight(int Coordinate[]);
bool SameColor(bool White, enum Piece Comparand);
bool ProcessMove(enum Piece *Board, char *move, bool WhitePlay);
bool GameIsOver(enum Piece *Board);

#endif //CHESSGAME_H
