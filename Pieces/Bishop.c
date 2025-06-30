#include "Bishop.h"
#include "../ChessGame.h"
#include "../EvalFunc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

bool ConfirmMove(int *OrigCoord, int *NewCoord) {
    int DiffRank = abs(OrigCoord[1] - NewCoord[1]);
    int DiffFile = abs(OrigCoord[0] - NewCoord[0]);
    return (DiffFile == DiffRank);
}

bool NoObstacle(int const *OrigCoord, int const *NewCoord, enum Piece const *Board) {
    int const Distance = abs(OrigCoord[0] - NewCoord[0]);
    int *move = malloc(sizeof(int) * 2);
    move[0] = NewCoord[0] - OrigCoord[0] < 0 ? -1 : 1;
    move[1] = NewCoord[1] - OrigCoord[1] < 0 ? -1 : 1;

    for (int i = 1; i < Distance; i++) {
        if (*(Board + (OrigCoord[0] + i * move[0]) + (OrigCoord[1] + i * move[1]) * 8) != EMPTY){
            free(move);
            return false;
        }
    }
    free(move);
    return true;
}


bool CompleteMove(enum Piece *Board, int *OrigCoord, int *NewCoord, enum Piece ActivePiece) {
    if (!ConfirmMove(OrigCoord, NewCoord) || !NoObstacle(OrigCoord, NewCoord, Board) || !IsLegitCoordinate(NewCoord)) {
        return false;
    }

    enum Piece const PreviousPiece = *(Board + NewCoord[0] + NewCoord[1] * SIDE_lENGHT);
    *(Board + OrigCoord[0] + OrigCoord[1] * SIDE_lENGHT) = EMPTY;
    *(Board + NewCoord[0] + NewCoord[1] * SIDE_lENGHT) = ActivePiece;

    if (IsCheck(Board, !isBlack(ActivePiece))) {
        *(Board + OrigCoord[0] + OrigCoord[1] * SIDE_lENGHT) = ActivePiece;
        *(Board + NewCoord[0] + NewCoord[1] * SIDE_lENGHT) = PreviousPiece;
        return false;
    }

    return true;
}

void SearchQuadrant(enum Piece const *Board, int const *NewCoord, enum Piece const ToFind, int const *changeInDir, int *SearchCoordinate) {

    SearchCoordinate[0] = NewCoord[0];
    SearchCoordinate[1] = NewCoord[1];

    //Start looking in the third quadrant and go counterclockwise from there
    for (int i = 0; IsLegitCoordinate(SearchCoordinate) && *(Board + SearchCoordinate[0] + SearchCoordinate[1] * SIDE_lENGHT) != ToFind; i++) {
        SearchCoordinate[0] += changeInDir[0];
        SearchCoordinate[1] += changeInDir[1];
        //Search Coordinate 2 is the number of steps taken
        SearchCoordinate[2] = i + 1;
    }
}

int *FindBishop(enum Piece const *Board, int const *NewCoord, bool const White, int const PriorityFile) {
    int *BishopCoord = malloc(sizeof(int) * 2);
    BishopCoord[0] = NOT_FOUND;
    BishopCoord[1] = NOT_FOUND;



    enum Piece ToFind = White ? W_BISHOP : B_BISHOP;

    //Count steps and select the closest bishop
    int StepToTake[8] = {-1, -1, 1, -1, 1, 1, -1, 1};
    int StepsTaken = 900;
    int *FoundBishop = malloc(sizeof(int) * 3);

    for (int i = 0; i < 4; i++) {
        SearchQuadrant(Board, NewCoord, ToFind, &StepToTake[i * 2], FoundBishop);
        if (IsLegitCoordinate(FoundBishop) && *(Board + FoundBishop[0] + FoundBishop[1] * 8) == ToFind && FoundBishop[2] < StepsTaken) {
            BishopCoord[0] = FoundBishop[0];
            BishopCoord[1] = FoundBishop[1];
            StepsTaken = FoundBishop[2];
        }

        if (PriorityFile == BishopCoord[0]) {
            free(FoundBishop);
            return BishopCoord;
        }
    }

    free(FoundBishop);
    return BishopCoord;
}

void GetBishopMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char **FoundMoves) {
    int const PossibleMoves[8] = {-1, -1, 1, -1, 1, 1, -1, 1};
    int const MovesSize = 4;
    GetMoves(BoardState, Coord, NumberOfMoves, 'B', PossibleMoves, false, MovesSize, FoundMoves);
}

bool DoBishopMove(enum Piece *Board, char Move[5], bool IsWhite) {
    int OrigCoord[2] = {INVALID_COORDINATE, INVALID_COORDINATE};
    int NewCoord[2] = {INVALID_COORDINATE, INVALID_COORDINATE};

    bool const Simple = isdigit(Move[2]) && !isdigit(Move[4]);
    bool const CompleteInfo = !Simple && !isdigit(Move[3]);

    if (Simple) {
        NewCoord[0] = LetterToCoordinate(Move[1]);
        NewCoord[1] = (int)(Move[2] - '0') - 1;
    }else if (!CompleteInfo) {
        OrigCoord[0] = LetterToCoordinate(Move[1]);
        NewCoord[0] = LetterToCoordinate(Move[2]);
        NewCoord[1] = (int)(Move[3] - '0') - 1;
    }else {
        OrigCoord[0] = LetterToCoordinate(Move[1]);
        OrigCoord[1] = (int)(Move[2] - '0') - 1;
        NewCoord[0] = LetterToCoordinate(Move[3]);
        NewCoord[1] = (int)(Move[4] - '0') - 1;
    }

    enum Piece ActivePiece = IsWhite ? W_BISHOP : B_BISHOP;

    bool const Capture = *(Board + NewCoord[0] + NewCoord[1] * SIDE_lENGHT) != EMPTY;
    if (Capture && SameColor(IsWhite, *(Board + NewCoord[0] + NewCoord[1] * SIDE_lENGHT))) {
        return false;
    }

    if (CompleteInfo) {
        return CompleteMove(Board, &OrigCoord[0], &NewCoord[0], ActivePiece);
    }

    int *FoundCoordinates = FindBishop(Board, NewCoord, IsWhite, OrigCoord[0]);
    OrigCoord[0] = FoundCoordinates[0];
    OrigCoord[1] = FoundCoordinates[1];
    free(FoundCoordinates);

    if (!IsLegitCoordinate(OrigCoord)) {
        return false;
    }

    return CompleteMove(Board, &OrigCoord[0], &NewCoord[0], ActivePiece);
}
