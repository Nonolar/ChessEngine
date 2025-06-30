#include "Rook.h"
#include "../ChessGame.h"
#include "../EvalFunc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>


bool confirmMove(int const NewCoord[], int const OldCoord[]) {
    return (NewCoord[0] - OldCoord[0] == 0 || NewCoord[1] - OldCoord[1] == 0);
}

bool RookMoveNoObstacle(enum Piece const *Board, int const *NewCoord, int const *OldCoord) {
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
    enum Piece const AlternativeToFind = SearchWhite ? W_ROOK_C : B_ROOK_C;
    for (int i = 0; i < 8; i++) {
        if (*(Board + File * 8 + i ) == ToFind || *(Board + File * 8 + i ) == AlternativeToFind) {
            return i;
        }
    }
    return INVALID_COORDINATE;
}

int SearchRankForRook(enum Piece *Board, int Rank, bool SearchWhite) {
    enum Piece ToFind = SearchWhite ? W_ROOK : B_ROOK;
    enum Piece const AlternativeToFind = SearchWhite ? W_ROOK_C : B_ROOK_C;
    for (int i = 0; i < 8; i++) {
        if (*(Board + Rank + i  * 8) == ToFind || *(Board + Rank + i * 8) == AlternativeToFind) {
            return i;
        }
    }
    return INVALID_COORDINATE;
}

int *FindRook(enum Piece const *Board, int const *Coord, bool const SearchWhite, int const PreferredRank) {
    int *FoundRook = malloc(sizeof(int) * 2);
    FoundRook[0] = INVALID_COORDINATE;
    FoundRook[1] = INVALID_COORDINATE;

    if (!IsLegitCoordinate(Coord)) {
        return FoundRook;
    }

    if (PreferredRank != INVALID_COORDINATE && PreferredRank != NOT_FOUND) {
        FoundRook[0] = PreferredRank;
        FoundRook[1] = SearchRankForRook(Board, PreferredRank, SearchWhite);
        return FoundRook;
    }

    //Try searching rank first, if not succes search File and return
    FoundRook[0] = Coord[0];
    FoundRook[1] = SearchRankForRook(Board, Coord[0], SearchWhite);

    if (FoundRook[1] != INVALID_COORDINATE) {
        return FoundRook;
    }

    FoundRook[1] = Coord[1];
    FoundRook[0] = SearchFileForRook(Board, Coord[1], SearchWhite);

    return FoundRook;
}

void GetRookMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char ***FoundMoves) {
    int const PossibleMoves[8] = {-1, 0, 0, -1, 1, 0, 0, 1};
    int const MovesSize = 4;
    GetMoves(BoardState, Coord, NumberOfMoves, 'R', PossibleMoves, false, MovesSize, FoundMoves);
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

    if (!IsLegitCoordinate(NewCoord)) {
        return false;
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

    int *FoundRook = FindRook(Board, NewCoord, WhitePlay, OrigCoord[0]);
    OrigCoord[0] = FoundRook[0];
    OrigCoord[1] = FoundRook[1];
    free(FoundRook);

    if (!IsLegitCoordinate(OrigCoord)) {
        return false;
    }

    if (!confirmMove(NewCoord, OrigCoord) || !RookMoveNoObstacle(Board, &NewCoord[0], &OrigCoord[0])) {
        return false;
    }

    enum Piece previousPiece = *(Board + NewCoord[0] + NewCoord[1] * 8);

    *(Board + NewCoord[0] + NewCoord[1] * 8) = ActivePiece;
    *(Board + OrigCoord[0] + OrigCoord[1] * 8) = EMPTY;

    if (IsCheck(Board, WhitePlay)) {
        *(Board + NewCoord[0] + NewCoord[1] * 8) = previousPiece;
        *(Board + OrigCoord[0] + OrigCoord[1] * 8) = ActivePiece;
        return false;
    }

    return true;
}