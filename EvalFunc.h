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
struct move {
    char *Move;
    float Evaluation;
};


bool SquareUnderAttack(enum Piece const *board, int const *Coord, bool CheckForWhite);
bool IsCheck(enum Piece const *board, bool const White);
void GetAllMoves(enum Piece *BoardState, bool White, char ***moves, int *NumberOfMoves);
float Evaluate(enum Piece *Board);
bool isBlack(enum Piece piece);
struct move Minimax(enum Piece *board, int depth, bool MaxPlayer, float alpha, float beta);
#endif //EVALFUNC_H
