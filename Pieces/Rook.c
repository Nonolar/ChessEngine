#include "Rook.h"
#include "../ChessGame.h"
#include "../EvalFunc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>


bool confirmMove(int NewCoord[], int OldCoord[]) {
    return (NewCoord[0] - OldCoord[0] == 0 || NewCoord[1] - OldCoord[1] == 0);
}

bool RookMoveNoObstacle(enum Piece *Board, int *NewCoord, int *OldCoord) {
    if (NewCoord[0] == OldCoord[0]) {
        int Direction = NewCoord[1] - OldCoord[1];
        Direction = Direction > 0 ? 1 : -1;
        for (int i = OldCoord[1] + Direction; i != NewCoord[1]; i += Direction) {
            if (*(Board + OldCoord[0] + i * 8) != EMPTY) {

                return false;
            }
        }
    }else {
        int Direction = NewCoord[0] - OldCoord[0];
        Direction = Direction > 0 ? 1 : -1;
        for (int i = OldCoord[0] + Direction; i != NewCoord[0]; i += Direction) {
            if (*(Board + OldCoord[1] * 8+ i) != EMPTY) {
                return false;
            }
        }
    }

    return true;
}

int SearchFileForRook(enum Piece *Board, int File, bool SearchWhite) {
    enum Piece ToFind = SearchWhite ? W_ROOK : B_ROOK;
    for (int i = 0; i < 8; i++) {
        if (*(Board + File + i * 8) == ToFind) {
            return i;
        }
    }
    return INVALID_COORDINATE;
}

int SearchRankForRook(enum Piece *Board, int Rank, bool SearchWhite) {
    enum Piece ToFind = SearchWhite ? W_ROOK : B_ROOK;
    for (int i = 0; i < 8; i++) {
        if (*(Board + Rank  * 8 + i) == ToFind) {
            return i;
        }
    }
    return INVALID_COORDINATE;
}

bool DoRookMove(enum Piece *Board, char *move, bool WhitePlay) {
    int OrigCoord[2] = {INVALID_COORDINATE, INVALID_COORDINATE};
    int NewCoord[2] = {INVALID_COORDINATE, INVALID_COORDINATE};

    bool const Simple = isdigit(move[2]) && !isdigit(move[4]);
    bool const CompleteInfo = !Simple && !isdigit(move[3]);

    if (Simple) {
        NewCoord[0] = LetterToCoordinate(move[1]);
        NewCoord[1] = (int)(move[2] - '0') - 1;
    }else if (isdigit(move[3])) {
        OrigCoord[0] = LetterToCoordinate(move[1]);
        NewCoord[0] = LetterToCoordinate(move[2]);
        NewCoord[1] = (int)(move[3] - '0') - 1;
    }else {
        OrigCoord[0] = LetterToCoordinate(move[1]);
        OrigCoord[1] = (int)(move[2] - '0') - 1;
        NewCoord[0] = LetterToCoordinate(move[3]);
        NewCoord[1] = (int)(move[4] - '0') - 1;
    }

    enum Piece ActivePiece = W_ROOK;
    if (!WhitePlay) {
        ActivePiece = B_ROOK;
    }

    bool const Capture = *(Board + NewCoord[0] + NewCoord[1] * 8) != EMPTY;
    if (Capture && SameColor(WhitePlay,* (Board + NewCoord[0] + NewCoord[1] * 8) )) {
        return false;
    }

    if (CompleteInfo && confirmMove(NewCoord, OrigCoord) && RookMoveNoObstacle(Board, &NewCoord[0], &OrigCoord[0])) {
        *(Board + NewCoord[0] + NewCoord[1] * 8) = ActivePiece;
        *(Board + OrigCoord[0] + OrigCoord[1] * 8) = EMPTY;
        return true;
    }else if (CompleteInfo) {
        return false;
    }

    if (!Simple) {
        OrigCoord[1] = SearchFileForRook(Board, OrigCoord[0], WhitePlay);
    }else if (SearchRankForRook(Board, NewCoord[1], WhitePlay) == INVALID_COORDINATE) {
        OrigCoord[1] = SearchFileForRook(Board, NewCoord[0], WhitePlay);
        OrigCoord[0] = NewCoord[0];
    }else {
        OrigCoord[1] = NewCoord[1];
        OrigCoord[0] = SearchRankForRook(Board, NewCoord[1], WhitePlay);
    }

    if (!confirmMove(NewCoord, OrigCoord) || !RookMoveNoObstacle(Board, &NewCoord[0], &OrigCoord[0])) {
        return false;
    }

    *(Board + NewCoord[0] + NewCoord[1] * 8) = ActivePiece;
    *(Board + OrigCoord[0] + OrigCoord[1] * 8) = EMPTY;

    return true;
}