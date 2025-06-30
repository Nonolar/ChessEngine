#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../ChessGame.h"
#include  "../EvalFunc.h"

bool IsValidMove(int *OrigCoord, int *NewCoord) {
    return abs(OrigCoord[0] - NewCoord[0]) <= 1 && abs(OrigCoord[1] - NewCoord[1]) <= 1;
}

bool FinishMove(enum Piece *board, int *origCoord, int *NewCoord, bool isWhite) {
    if (!IsValidMove(origCoord, NewCoord) || SquareUnderAttack(board, NewCoord, isWhite)) {
        return false;
    }

    enum Piece ActivePiece = isWhite ? W_KING : B_KING;

    *(board + NewCoord[0] + NewCoord[1] * 8) = ActivePiece;
    *(board + origCoord[0] + origCoord[1] * 8) = EMPTY;

    return true;
}

int *FindKing(enum Piece *board, int *NewCoord, bool isWhite) {
    int *KingCoordinate = malloc(sizeof(int) * 2);
    KingCoordinate[0] = INVALID_COORDINATE;
    KingCoordinate[1] = INVALID_COORDINATE;

    enum Piece ToFind = isWhite ? W_KING : B_KING;
    enum Piece AlternativeToFind = isWhite ? W_KING_C : B_KING_C;

    int const SearchPatterns[16] = {-1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0};

    for (int i = 0; i < 8; i++) {
        KingCoordinate[0] = NewCoord[0] + SearchPatterns[i * 2];
        KingCoordinate[1] = NewCoord[1] + SearchPatterns[i * 2 + 1];
        if (IsLegitCoordinate(KingCoordinate) && (*(board + KingCoordinate[0] + KingCoordinate[1] * 8) == ToFind || (*(board + KingCoordinate[0] + KingCoordinate[1] * 8) == AlternativeToFind))) {
            return KingCoordinate;
        }
    }

    KingCoordinate[0] = INVALID_COORDINATE;
    KingCoordinate[1] = INVALID_COORDINATE;

    return KingCoordinate;
}



bool DoKingMove(enum Piece *board, char const *move, bool const isWhite) {
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
        return FinishMove(board, OrigCoord, NewCoord, isWhite);
    }

    int *KingCoordinate = FindKing(board, NewCoord, isWhite);
    OrigCoord[0] = KingCoordinate[0];
    OrigCoord[1] = KingCoordinate[1];
    free(KingCoordinate);

    if (!IsLegitCoordinate(OrigCoord)) {
        return false;
    }

    return FinishMove(board, OrigCoord, NewCoord, isWhite);
}

bool QueenSideCastle(enum Piece *board, bool isWhite) {
    int const Rank = isWhite ? 0 : 7;
    int const CoordRank = Rank * SIDE_lENGHT;

    int Coord[2] = {0, Rank};

    for (int i = 0; i < 3; i++) {
        Coord[0] = i + 1;
        if (*(board + Coord[0] + CoordRank) != EMPTY || SquareUnderAttack(board, Coord, isWhite)) {
            return false;
        }
    }

    enum Piece const ROOK = isWhite ? W_ROOK_C : B_ROOK_C;
    enum Piece const NEW_ROOK = isWhite ? W_ROOK : B_ROOK;
    enum Piece const KING = isWhite ? W_KING : B_KING;

    if (*(board + 0 + CoordRank) != ROOK) {
        return false;
    }

    *(board + 0 + CoordRank) = EMPTY;
    *(board + 2 + CoordRank) = KING;
    *(board + 3 + CoordRank) = NEW_ROOK;
    *(board + 4 + CoordRank) = EMPTY;

    return true;
}

bool KingSideCastle(enum Piece *board, bool isWhite) {
    int const Rank = isWhite ? 0 : 7;
    int const CoordRank = Rank * SIDE_lENGHT;

    int Coord[2] = {0, Rank};

    for (int i = 0; i < 2; i++) {
        Coord[0] = i + 5;
        if (*(board + Coord[0] + CoordRank) != EMPTY || SquareUnderAttack(board, Coord, isWhite)) {
            return false;
        }
    }

    enum Piece const ROOK = isWhite ? W_ROOK_C : B_ROOK_C;
    enum Piece const NEW_ROOK = isWhite ? W_ROOK : B_ROOK;
    enum Piece const KING = isWhite ? W_KING : B_KING;

    if (*(board + 7 + CoordRank) != ROOK) {
        return false;
    }

    *(board + 7 + CoordRank) = EMPTY;
    *(board + 6 + CoordRank) = KING;
    *(board + 5 + CoordRank) = NEW_ROOK;
    *(board + 4 + CoordRank) = EMPTY;

    return true;
}

bool Castle(enum Piece *board, char *move, bool isWhite) {
    int const LocationKing = isWhite ? 4 + 0 * SIDE_lENGHT : 4 + 7 * SIDE_lENGHT;
    enum Piece const ActivePiece = isWhite ? W_KING_C : B_KING_C;
    if (*(board + LocationKing) != ActivePiece) {
        return false;
    }
    if (move[4] == 'O') {
        return QueenSideCastle(board, isWhite);
    }

    if (move[4] != 'O') {
        return KingSideCastle(board, isWhite);
    }

    return false;
}