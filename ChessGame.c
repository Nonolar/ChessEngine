//
// Created by noel on 2025-06-22.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EvalFunc.h"
#include "ChessGame.h"

#include <ctype.h>

enum Types {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
};

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


void FindSinglePawnCapture(enum Piece const *Board, int const *NewCoord, int *OldCoord, bool IsWhite) {
    int NewCoord0 = *NewCoord;
    int NewCoord1 = *(NewCoord + 1);
    if (IsWhite) {
        OldCoord[1] = NewCoord[1] - 1;
        if (*(Board + (NewCoord0 - 1) + (NewCoord1 - 1) * 8) == W_PAWN) {
            OldCoord[0] = NewCoord[0] - 1;
        }else if (*(Board + (NewCoord0 + 1)+ (NewCoord1 - 1) * 8) == W_PAWN) {
            OldCoord[0] = NewCoord[0] + 1;
        }else {
            OldCoord[0] = - 999;
            OldCoord[1] = -999;
        }
    }else {
        OldCoord[1] = NewCoord[1] + 1;
        if (*(Board + (NewCoord[0] - 1) + (NewCoord[1] + 1) * 8) == B_PAWN) {
            OldCoord[0] = NewCoord[0] - 1;
        }else if (*(Board + (NewCoord[0] +1) + (NewCoord[1] + 1) * 8) == B_PAWN) {
            OldCoord[0] = NewCoord[0] + 1;
        }else {
            OldCoord[0] = -999;
            OldCoord[1] = -999;
        }
    }
}


bool IsEnPassant(enum Piece const *Board, int const *NewCoord, bool IsWhite) {
    if (IsWhite) {
        return *(Board + NewCoord[0] + (NewCoord[1] - 1 ) * 8) == B_PAWN_EN;
    }else {
        return *(Board + NewCoord[0] + (NewCoord[1] + 1) * 8) == W_PAWN_EN;
    }
}

int FindPawnOnRank(enum Piece const *Board, int NewColummn, int Rank, bool FindWhite) {
    if (FindWhite) {
        if (*(Board + Rank + (NewColummn - 1) * 8) == W_PAWN) {
            return NewColummn - 1;
        }else if (*(Board + Rank + (NewColummn - 2) * 8) == W_PAWN && NewColummn - 2 == 1 && *(Board + Rank + (NewColummn - 1) * 8) == EMPTY) {
            return NewColummn - 2;
        }else {
            return -999;
        }
    }else {
        if (*(Board + Rank + (NewColummn + 1) * 8) == B_PAWN) {
            return NewColummn + 1;
        }else if (*(Board + Rank + (NewColummn + 2) * 8) == B_PAWN && NewColummn + 2 == 6 && *(Board + Rank + (NewColummn + 1) * 8) == EMPTY) {
            return NewColummn + 2;
        }else {
            return -999;
        }
    }
}


bool PawnMove(enum Piece *Board, char move[4], bool WhitePlay) {
    int OrigCoord[2] = {-999, -999};
    int NewCoord[2] = {-999, -999};
    bool const Simple = isdigit(move[1]);
    if (Simple) {
        //In this case we have something like "e4" e.g. the pawn on the e lane to e4
        NewCoord[0] = LetterToCoordinate(move[0]);
        NewCoord[1] = (int)(move[1] - '0') - 1;
    }else {
        //In this case we have something like "de5" e.g. d pawn to e5
        NewCoord[0] = LetterToCoordinate(move[1]);
        NewCoord[1] = (int)(move[2] - '0') - 1;
    }

    //Find pawn in rank
    enum Piece ToFind = W_PAWN;
    if (!WhitePlay) {
        ToFind = B_PAWN;
    }
    enum Piece NewTile = *(Board + NewCoord[0] + NewCoord[1] * 8);
    bool Capture = NewTile != EMPTY;
    //Check so that we don't capture our own piece
    if ( Capture && SameColor(WhitePlay, NewTile)) {
        return false;
    }

    bool EnPassant = IsEnPassant(Board, NewCoord, WhitePlay);

    if (Capture && Simple) {
        FindSinglePawnCapture(Board, NewCoord, OrigCoord, WhitePlay);
    }else if (Capture && WhitePlay) {
        OrigCoord[0] = LetterToCoordinate(move[0]);
        OrigCoord[1] = NewCoord[1] - 1;
    }else if (Capture) {
        OrigCoord[0] = LetterToCoordinate(move[0]);
        OrigCoord[1] = NewCoord[1] + 1;
    }else if (EnPassant) {
        FindSinglePawnCapture(Board, NewCoord, OrigCoord, WhitePlay);
    }else {
        OrigCoord[0] = LetterToCoordinate(move[0]);
        OrigCoord[1] = FindPawnOnRank(Board, NewCoord[1], OrigCoord[0], WhitePlay);
    }

    if (!CheckFinalValidity(NewCoord, OrigCoord, PAWN)) {
        return false;
    }

    if (abs(OrigCoord[1] - NewCoord[1]) == 2) {
        if (WhitePlay) {
            ToFind = W_PAWN_EN;
        }else {
            ToFind = B_PAWN_EN;
        }
    }

    *(Board + OrigCoord[0] + OrigCoord[1] * 8) = EMPTY;
    *(Board + NewCoord[0] + NewCoord[1] * 8) = ToFind;
    if (EnPassant && WhitePlay) {
        *(Board + NewCoord[0] + OrigCoord[1] * 8) = EMPTY;
    }

    return true;
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
    return -999;
}

int SearchRankForRook(enum Piece *Board, int Rank, bool SearchWhite) {
    enum Piece ToFind = SearchWhite ? W_ROOK : B_ROOK;
    for (int i = 0; i < 8; i++) {
        if (*(Board + Rank  * 8 + i) == ToFind) {
            return i;
        }
    }
    return -999;
}

bool RookMove(enum Piece *Board, char move[5], bool WhitePlay) {
    int OrigCoord[2] = {-999, -999};
    int NewCoord[2] = {-999, -999};

    bool const Simple = isdigit(move[2]) && !isdigit(move[4]);
    bool CompleteInfo = !Simple && !isdigit(move[3]);

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

    bool Capture = *(Board + NewCoord[0] + NewCoord[1] * 8) != EMPTY;
    if (Capture && SameColor(WhitePlay,* (Board + NewCoord[0] + NewCoord[1] * 8) )) {
        return false;
    }

    if (CompleteInfo && CheckFinalValidity(NewCoord, OrigCoord, ROOK) && RookMoveNoObstacle(Board, &NewCoord[0], &OrigCoord[0])) {
        *(Board + NewCoord[0] + NewCoord[1] * 8) = ActivePiece;
        *(Board + OrigCoord[0] + OrigCoord[1] * 8) = EMPTY;
        return true;
    }else if (CompleteInfo) {
        return false;
    }

    if (!Simple) {
        OrigCoord[1] = SearchFileForRook(Board, OrigCoord[0], WhitePlay);
    }else if (SearchRankForRook(Board, NewCoord[1], WhitePlay) == -999) {
        OrigCoord[1] = SearchFileForRook(Board, NewCoord[0], WhitePlay);
        OrigCoord[0] = NewCoord[0];
    }else {
        OrigCoord[1] = NewCoord[1];
        OrigCoord[0] = SearchRankForRook(Board, NewCoord[1], WhitePlay);
    }

    if (!CheckFinalValidity(NewCoord, OrigCoord, ROOK) || !RookMoveNoObstacle(Board, &NewCoord[0], &OrigCoord[0])) {
        return false;
    }

    *(Board + NewCoord[0] + NewCoord[1] * 8) = ActivePiece;
    *(Board + OrigCoord[0] + OrigCoord[1] * 8) = EMPTY;

    return true;
}

bool PieceSwitch(enum Piece *Board, char move[5], bool WhitePlay) {
    switch (*move) {
        case 'R':
            return RookMove(Board, move, WhitePlay);
    }
}



bool ProcessMove(enum Piece *Board, char *move, bool WhitePlay) {

    if (CharIsPiece(*move)) {
        return PieceSwitch(Board, move, WhitePlay);
    }else {
        return PawnMove(Board, move, WhitePlay);
    }
}

//bool IsValidMove(enum Piece Board[8][8], char move[4], bool WhitePlay) {
//    return true;
//}

bool GameIsOver(enum Piece *Board) {
 return false;
}
