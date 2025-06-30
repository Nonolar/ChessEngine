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
    EMPTY_SQR
};
enum Types PieceToType(enum Piece piece);
enum Piece LetterToPiece(char letter, bool White);
int LetterToCoordinate(char letter);
bool IsLegitCoordinate(int *Coordinate);
bool IsTileLight(int Coordinate[]);
bool SameColor(bool White, enum Piece Comparand);
void GetMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char const Piece, int const *StepsToTake, bool const OneMove, int MovesSize, char **Moves);
bool ProcessMove(enum Piece *Board, char *move, bool WhitePlay);
bool GameIsOver(enum Piece *Board);

#endif //CHESSGAME_H
