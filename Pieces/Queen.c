#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../EvalFunc.h"
#include "../ChessGame.h"

bool IsCorrectMove(int const *NewCoord, int const *OldCoord) {
    bool isCorrect = false;
    isCorrect = abs(OldCoord[1] - NewCoord[1]) == abs(OldCoord[0] - NewCoord[0]);
    isCorrect = isCorrect || (NewCoord[0] - OldCoord[0] == 0 || NewCoord[1] - OldCoord[1] == 0);
    return isCorrect;
}

bool FindObstacle(int const *OldCoord, int const *NewCoord, enum Piece const *board) {
    int Xmov = (NewCoord[0] - OldCoord[0]) < 0 ? -1 : 1;
    int Ymov = (NewCoord[1] - OldCoord[1]) < 0 ? -1 : 1;

    int SearchCoord[] = {OldCoord[0] + Xmov, OldCoord[1] + Ymov};
    for (int i = 2; IsLegitCoordinate(&SearchCoord[0]) && (SearchCoord[0] != *NewCoord || SearchCoord[1] != NewCoord[1]); i++) {


        if (*(board + SearchCoord[0] + SearchCoord[1] * SIDE_lENGHT) != EMPTY) {
            return true;
        }
        SearchCoord[0] = OldCoord[0] + Xmov * i;
        SearchCoord[1] = OldCoord[1] + Ymov * i;
    }

    return false;
}

bool FinalizeMove(enum Piece *board, int const *OldCoord, int const *NewCoord, bool const isWhite) {
    if (!IsCorrectMove(NewCoord, OldCoord) || FindObstacle(OldCoord, NewCoord, board)) {
        return false;
    }

    enum Piece activePiece = isWhite ? W_QUEEN : B_QUEEN;

    *(board + NewCoord[0] + NewCoord[1] * 8) = activePiece;
    *(board + OldCoord[0] + OldCoord[1] * 8) = EMPTY;

    return true;
}

void SearchPart(enum Piece const *Board, int const *NewCoord, enum Piece const ToFind, int const *changeInDir, int *SearchCoord) {
    SearchCoord[0] = NewCoord[0];
    SearchCoord[1] = NewCoord[1];

    for (int i = 0; IsLegitCoordinate(SearchCoord) && *(Board + SearchCoord[0] + SearchCoord[1] * SIDE_lENGHT) != ToFind; i++) {
        SearchCoord[0] += changeInDir[0];
        SearchCoord[1] += changeInDir[1];
        SearchCoord[2] = i + 1;
    }
}

int *FindQueen(enum Piece const *board, int const *NewCoord, enum Piece const ToFind, int PriorityFile) {
    int *FoundLocation = malloc(sizeof(int) * 2);
    FoundLocation[0] = INVALID_COORDINATE;
    FoundLocation[1] = INVALID_COORDINATE;

    int SearchLocation[] = {NewCoord[0], NewCoord[1], 0};


    int StepsTaken = 900;

    int const SearchPatterns[16] = {-1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0};

    for (int i = 0; i < 8; i++) {
        SearchPart(board, NewCoord, ToFind, &SearchPatterns[i * 2], &SearchLocation[0]);
        if (IsLegitCoordinate(SearchLocation) && *(board + SearchLocation[0] + SearchLocation[1] * 8) == ToFind && SearchLocation[2] < StepsTaken) {
            FoundLocation[0] = SearchLocation[0];
            FoundLocation[1] = SearchLocation[1];
            StepsTaken = SearchLocation[2];
        }

        if (FoundLocation[0] == PriorityFile) {
            return FoundLocation;
        }
    }

    return FoundLocation;
}

bool DoQueenMove(enum Piece *board, char const *move, bool const isWhite){
    int OrigCoord[2] = {NOT_FOUND, NOT_FOUND};
    int NewCoord[2] = {INVALID_COORDINATE, INVALID_COORDINATE};

    bool const Simple = isdigit(move[2]) && !isdigit(move[4]);
    bool const CompleteInfo = !Simple && !isdigit(move[3]);

    if (Simple) {
        NewCoord[0] = LetterToCoordinate(move[1]);
        NewCoord[1] = (int)(move[2] - '0') - 1;
    }else if (!CompleteInfo) {
        OrigCoord[0] = LetterToCoordinate(move[1]);
        NewCoord[0] = LetterToCoordinate(move[2]);
        NewCoord[1] = (int)(move[3] - '0') - 1;
    }else {
        OrigCoord[0] = LetterToCoordinate(move[1]);
        OrigCoord[1] = (int)(move[2] - '0') - 1;
        NewCoord[0] = LetterToCoordinate(move[3]);
        NewCoord[1] = (int)(move[4] - '0') - 1;
    }

    if (!IsLegitCoordinate(NewCoord)) {
        return false;
    }

    bool const Capture = *(board + NewCoord[0] + NewCoord[1] * 8) != EMPTY;

    if (Capture && SameColor(isWhite, *(board + NewCoord[0] + NewCoord[1] * 8))) {
        return false;
    }

    if (CompleteInfo) {
        return FinalizeMove(board, &OrigCoord[0], &NewCoord[0], isWhite);
    }

    enum Piece ToFind = isWhite ? W_QUEEN : B_QUEEN;

    int *Location = FindQueen(board, &NewCoord[0], ToFind, OrigCoord[0]);
    OrigCoord[0] = Location[0];
    OrigCoord[1] = Location[1];
    free(Location);

    if (!IsLegitCoordinate(&OrigCoord[0])) {
        return false;
    }

    return FinalizeMove(board, &OrigCoord[0], &NewCoord[0], isWhite);
}