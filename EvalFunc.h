#ifndef EVALFUNC_H
#define EVALFUNC_H
#pragma once
enum Piece {
    B_PAWN,
    B_ROOK,
    B_KNIGHT,
    B_BISHOP,
    B_KING,
    B_QUEEN,
    W_PAWN,
    W_ROOK,
    W_KNIGHT,
    W_BISHOP,
    W_KING,
    W_QUEEN,
    B_PAWN_EN,
    W_PAWN_EN,
    EMPTY
};

float Evaluate(enum Piece *Board);
bool isBlack(enum Piece piece);
#endif //EVALFUNC_H
