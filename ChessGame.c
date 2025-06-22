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
            bool Valid = (NewCoord[0] - OldCoord[0] == 0 && abs(NewCoord[1] - OldCoord[1]) <= 2) || (abs(NewCoord[0] - OldCoord[0]) == 1 && abs(NewCoord[1] - OldCoord[1]) == 1);
            return Valid;
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

//Correct memory
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
//Correct memory
int FindPawnOnRank(enum Piece const *Board, int NewColummn, int Rank, bool FindWhite) {
    if (FindWhite) {
        if (*(Board + Rank + (NewColummn - 1) * 8) == W_PAWN) {
            return NewColummn - 1;
        }else if (*(Board + Rank + (NewColummn - 2) * 8) == W_PAWN && NewColummn - 2 == 1) {
            return NewColummn - 2;
        }else {
            return -999;
        }
    }else {
        if (*(Board + Rank + (NewColummn + 1) * 8) == B_PAWN) {
            return NewColummn + 1;
        }else if (*(Board + Rank + (NewColummn + 2) * 8) == B_PAWN && NewColummn + 2 == 6) {
            return NewColummn + 2;
        }else {
            return -999;
        }
    }
}

//Correct memory
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

bool PieceSwitch(enum Piece *Board, char move[4], bool WhitePlay) {

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
