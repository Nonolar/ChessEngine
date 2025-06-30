#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../EvalFunc.h"
#include "../ChessGame.h"

bool ValidMove(int const *origCoord, int const *NewCoord) {
    int DeltaX = abs(origCoord[0] - NewCoord[0]);
    int DeltaY = abs(origCoord[1] - NewCoord[1]);
    return  (DeltaX == 2 && DeltaY == 1) || (DeltaX == 1 && DeltaY == 2);
}

bool CompleteKnightMove(enum Piece *board, int *OrigCoord, int *NewCoord, bool isWhite) {
    if (!ValidMove(OrigCoord, NewCoord)) {
        return false;
    }

    enum Piece const activePiece = isWhite ? W_KNIGHT : B_KNIGHT;
    enum Piece const PreviousPiece = *(board + NewCoord[0] + NewCoord[1] * SIDE_lENGHT);

    *(board + NewCoord[0] + NewCoord[1] * SIDE_lENGHT) = activePiece;
    *(board + OrigCoord[0] + OrigCoord[1] * SIDE_lENGHT) = EMPTY;

    if (IsCheck(board, isWhite)) {
        *(board + NewCoord[0] + NewCoord[1] * SIDE_lENGHT) = PreviousPiece;
        *(board + OrigCoord[0] + OrigCoord[1] * SIDE_lENGHT) = activePiece;
        return false;
    }

    return true;
}

int *FindKnight(enum Piece const *board, int const *NewCoord, bool const isWhite, int const priorityFile) {
    int Search[] = {NOT_FOUND, NOT_FOUND};
    int *KnightCoord = malloc(sizeof(int) * 2);
    KnightCoord[0] = INVALID_COORDINATE;
    KnightCoord[1] = INVALID_COORDINATE;

    enum Piece ToFind = isWhite ? W_KNIGHT : B_KNIGHT;

    int const SearchPatterns[16] = {-2, -1, -1, -2, 1, -2, 2, -1, 2, 1, 1, 2, -1, 2, -2, 1};

    for (int i = 0; i < 8; i++) {
        Search[0] = NewCoord[0] + SearchPatterns[i * 2];
        Search[1] = NewCoord[1] + SearchPatterns[i * 2 + 1];
        if (IsLegitCoordinate(Search) && *(board + Search[0] + Search[1] * SIDE_lENGHT) == ToFind) {
            KnightCoord[0] = Search[0];
            KnightCoord[1] = Search[1];
        }

        if (KnightCoord[0] == priorityFile) {
            return KnightCoord;
        }
    }

    return KnightCoord;
}
void GetKnightMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char ***FoundMoves) {
    int const PossibleMoves[16] = {-2, -1, -1, -2, 1, -2, 2, -1, 2, 1, 1, 2, -1, 2, -2, 1};
    int const MovesSize = 8;
    GetMoves(BoardState, Coord, NumberOfMoves, 'N', PossibleMoves, true, MovesSize, FoundMoves);
}
bool DoKnightMove(enum Piece *board, char const *move, bool const isWhite) {
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
        return CompleteKnightMove(board, OrigCoord, NewCoord, isWhite);
    }

    int *FoundKnight = FindKnight(board, NewCoord, isWhite, OrigCoord[0]);
    OrigCoord[0] = FoundKnight[0];
    OrigCoord[1] = FoundKnight[1];
    free(FoundKnight);

    if (!IsLegitCoordinate(OrigCoord)) {
        return false;
    }

    return CompleteKnightMove(board, OrigCoord, NewCoord, isWhite);
}
