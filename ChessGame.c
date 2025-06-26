#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EvalFunc.h"
#include "ChessGame.h"
#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include "Pieces/Bishop.h"
#include "Pieces/Queen.h"
#include "Pieces/King.h"




int LetterToCoordinate(char Letter) {
    switch (Letter) {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
        default:
            return -1;
    }
}

bool SameColor(bool White, enum Piece Comparand) {
    if (White && !isBlack(Comparand)) {
        return true;
    }

    if (!White && isBlack(Comparand)) {
        return true;
    }

    return false;
}

bool IsLegitCoordinate(int *Coordinate) {
    if (Coordinate[0] == INVALID_COORDINATE || Coordinate[1] == INVALID_COORDINATE) {
        return false;
    }

    if (Coordinate[0] < 0 || Coordinate[1] < 0 ) {
        return false;
    }

    if (Coordinate[0] > 7 || Coordinate[1] > 7) {
        return false;
    }

    return true;
}

bool IsTileLight(int Coordinate[]) {
    return !(Coordinate[0] % 2 == Coordinate[1] % 2);
}

bool CharIsPiece(char const C) {
    if (C != 'K' && C != 'Q' && C != 'R' && C != 'B' && C != 'N') {
        return false;
    }else {
        return true;
    }
}

bool PieceSwitch(enum Piece *Board, char move[5], bool WhitePlay) {

    switch (*move) {
        case 'R':
            return DoRookMove(Board, move, WhitePlay);
        case 'B':
            return DoBishopMove(Board, move, WhitePlay);
        case 'Q':
            return DoQueenMove(Board, move, WhitePlay);
        case 'K':
            return DoKingMove(Board, move, WhitePlay);
    }
}

bool ProcessMove(enum Piece *Board, char *move, bool WhitePlay) {

    if (CharIsPiece(*move)) {
        return PieceSwitch(Board, move, WhitePlay);
    }else {
        return DoPawnMove(Board, move, WhitePlay);
    }
}


bool GameIsOver(enum Piece *Board) {
 return false;
}
