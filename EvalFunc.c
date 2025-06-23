#include <stdio.h>
#include <stdbool.h>
#include "EvalFunc.h"

bool isBlack(enum Piece piece) {
    switch (piece) {
        case B_PAWN:
            return true;
        case B_PAWN_EN:
            return true;
        case B_ROOK:
            return true;
        case B_KNIGHT:
            return true;
        case B_BISHOP:
            return true;
        case B_KING:
            return true;
        case B_QUEEN:
            return true;
        default:
            return false;
    }
}
int GetPieceScore(enum Piece piece) {
    switch (piece) {
        case B_PAWN:
            return 1;
        case B_PAWN_EN:
            return 1;
        case B_ROOK:
            return 5;
        case B_KNIGHT:
            return 3;
        case B_BISHOP:
            return 3;
        case B_QUEEN:
            return 9;
        case B_KING:
            return 999;
        case W_PAWN:
            return 1;
        case W_PAWN_EN:
            return 1;
        case W_ROOK:
            return 5;
        case W_KNIGHT:
            return 3;
        case W_BISHOP:
            return 3;
        case W_QUEEN:
            return 9;
        case W_KING:
            return 999;
        default:
            return 0;

    }
}

float getEvalScorePiece(enum Piece *piece) {
    if (*piece == EMPTY) {
        return 0.0f;
    }

    if (isBlack(*piece)) {
        float Score = (float)GetPieceScore(*piece);
        return Score * -1.0f;
    }

    return (float)GetPieceScore(*piece);
}

float CountPieces(enum Piece *Board) {
    float result = 0.0f;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            result += getEvalScorePiece((Board + i * 8 + j));
        }
    }

    return result;
}

float Evaluate(enum Piece *Board) {

    float Evaluation = 0;

    Evaluation += CountPieces(Board);

    return Evaluation;
}
