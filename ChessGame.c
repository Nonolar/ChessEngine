#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EvalFunc.h"
#include "ChessGame.h"
#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include <ctype.h>




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

bool CheckFinalValidity(int NewCoord[], int OldCoord[], enum Types PieceType) {
    switch (PieceType) {
        case PAWN:
            bool ValidPawn = (NewCoord[0] - OldCoord[0] == 0 && abs(NewCoord[1] - OldCoord[1]) <= 2) || (abs(NewCoord[0] - OldCoord[0]) == 1 && abs(NewCoord[1] - OldCoord[1]) == 1);
            return ValidPawn;
        case ROOK:
            bool ValidRook = (NewCoord[0] - OldCoord[0] == 0 || NewCoord[1] - OldCoord[1] == 0);
            return ValidRook;
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
