#ifndef EVALFUNC_H
#define EVALFUNC_H
#pragma once
enum Piece {
    B_PAWN,
    B_ROOK,
    B_ROOK_C,
    B_KNIGHT,
    B_BISHOP,
    B_KING,
    B_KING_C,
    B_QUEEN,
    W_PAWN,
    W_ROOK,
    W_ROOK_C,
    W_KNIGHT,
    W_BISHOP,
    W_KING,
    W_KING_C,
    W_QUEEN,
    B_PAWN_EN,
    W_PAWN_EN,
    EMPTY
};
bool SquareUnderAttack(enum Piece const *board, int const *Coord, bool CheckForWhite);
bool IsCheck(enum Piece const *board, bool const White);
float Evaluate(enum Piece *Board);
bool isBlack(enum Piece piece);
#endif //EVALFUNC_H
