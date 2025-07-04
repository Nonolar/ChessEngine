#include "../ChessGame.h"
#include "../EvalFunc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include "Pawn.h"

bool CorrectMovement(int const NewCoord[], int const OldCoord[]) {
    return (NewCoord[0] - OldCoord[0] == 0 && abs(NewCoord[1] - OldCoord[1]) <= 2) || (abs(NewCoord[0] - OldCoord[0]) == 1 && abs(NewCoord[1] - OldCoord[1]) == 1);
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
            OldCoord[0] = INVALID_COORDINATE;
            OldCoord[1] = INVALID_COORDINATE;
        }
    }else {
        OldCoord[1] = NewCoord[1] + 1;
        if (*(Board + (NewCoord[0] - 1) + (NewCoord[1] + 1) * 8) == B_PAWN) {
            OldCoord[0] = NewCoord[0] - 1;
        }else if (*(Board + (NewCoord[0] +1) + (NewCoord[1] + 1) * 8) == B_PAWN) {
            OldCoord[0] = NewCoord[0] + 1;
        }else {
            OldCoord[0] = INVALID_COORDINATE;
            OldCoord[1] = INVALID_COORDINATE;
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
            return INVALID_COORDINATE;
        }
    }else {
        if (*(Board + Rank + (NewColummn + 1) * 8) == B_PAWN) {
            return NewColummn + 1;
        }else if (*(Board + Rank + (NewColummn + 2) * 8) == B_PAWN && NewColummn + 2 == 6 && *(Board + Rank + (NewColummn + 1) * 8) == EMPTY) {
            return NewColummn + 2;
        }else {
            return INVALID_COORDINATE;
        }
    }
}

bool Promote(enum Piece *board, int const *OldCoord, int const *NewCoord, char const PieceLetter, bool const isWhite) {
    enum Piece const newPiece = LetterToPiece(PieceLetter, isWhite);
    if (!CorrectMovement(NewCoord, OldCoord) || newPiece == EMPTY) {
        return false;
    }

    *(board + OldCoord[0] + OldCoord[1] * SIDE_lENGHT) = EMPTY;
    *(board + NewCoord[0] + NewCoord[1] * SIDE_lENGHT) = newPiece;

    return true;
}

void GetPawnMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char ***FoundMoves) {
    char const *Letters = "abcdefgh";
    char const BasicMove = Letters[Coord[0]];


    int ActiveCoord[2] = {Coord[0], Coord[1]};

    bool const IsWhite = !isBlack(*(BoardState + Coord[0] + Coord[1] * 8));

    int const DeltaRank = IsWhite ? 1 : -1;

    bool CanMove = *(BoardState + ActiveCoord[0] + (ActiveCoord[1] + DeltaRank) * SIDE_lENGHT) == EMPTY;
    bool CanMoveTwo = CanMove && (IsWhite ? Coord[1] == 1 : Coord[1] == 6) && *(BoardState + ActiveCoord[0] + (ActiveCoord[1] + DeltaRank * 2) * SIDE_lENGHT) == EMPTY ;

    enum Piece oppositeEnPassant = IsWhite ? B_PAWN_EN : W_PAWN_EN;

    bool EnPassantRight = *(BoardState + ActiveCoord[0] + 1 + ActiveCoord[1] * 8) == oppositeEnPassant;
    bool EnPassantLeft = *(BoardState + ActiveCoord[0] - 1 + ActiveCoord[1] * 8) == oppositeEnPassant;

    enum Piece UpLeft = *(BoardState + ActiveCoord[0] - 1 + (ActiveCoord[1] + DeltaRank) * 8);
    enum Piece UpRight = *(BoardState + ActiveCoord[0] + 1 + (ActiveCoord[1] + DeltaRank) * 8);

    bool CaptureRight = UpRight != EMPTY && !SameColor(IsWhite, UpRight);
    bool CaptureLeft = UpLeft != EMPTY && !SameColor(IsWhite, UpLeft);

    bool Promotion = (IsWhite && ActiveCoord[1] + DeltaRank == 7) || (!IsWhite && ActiveCoord[1] + DeltaRank == 0);

    int newMoves = CanMove + CanMoveTwo + EnPassantRight + EnPassantLeft + CaptureLeft + CaptureRight;
    newMoves = Promotion ? 4 * newMoves : newMoves;

    if (newMoves == 0) {
        return;
    }

    char **NewList = NULL;
    if (*FoundMoves == NULL) {
        *FoundMoves = malloc(sizeof(char*) * newMoves);
        *NumberOfMoves += newMoves;
        NewList = *FoundMoves;
    }else {
        *NumberOfMoves += newMoves;
        NewList = realloc(*FoundMoves, sizeof(char*) * (*NumberOfMoves));

    }

    if (NewList == NULL) {
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }

    *FoundMoves = NewList;

    //I don't feel like doing this dynamically so I will do it manually

    char const *PromotionPieces = "QRBN";

    if (CanMove && !Promotion) {
        (*FoundMoves)[*NumberOfMoves - newMoves] = malloc(sizeof(char) * 4);
        (*FoundMoves)[*NumberOfMoves - newMoves][0] = BasicMove;
        (*FoundMoves)[*NumberOfMoves - newMoves][1] = Letters[ActiveCoord[0]];
        (*FoundMoves)[*NumberOfMoves - newMoves][2] = (char)((ActiveCoord[1] + DeltaRank) + 0x31);
        (*FoundMoves)[*NumberOfMoves - newMoves][3] = '\0';
        newMoves--;
    }else if (CanMove && Promotion) {
        for (int i = 0; i < 4; i++) {
            (*FoundMoves)[*NumberOfMoves - newMoves] = malloc(sizeof(char) * 5);
            (*FoundMoves)[*NumberOfMoves - newMoves][0] = BasicMove;
            (*FoundMoves)[*NumberOfMoves - newMoves][1] = Letters[ActiveCoord[0]];
            (*FoundMoves)[*NumberOfMoves - newMoves][2] = (char)((ActiveCoord[1] + DeltaRank) + 0x31);
            (*FoundMoves)[*NumberOfMoves - newMoves][3] = PromotionPieces[i];
            (*FoundMoves)[*NumberOfMoves - newMoves][4] = '\0';
            newMoves--;
        }
    }

    if (CanMoveTwo) {
        (*FoundMoves)[*NumberOfMoves - newMoves] = malloc(sizeof(char) * 4);
        (*FoundMoves)[*NumberOfMoves - newMoves][0] = BasicMove;
        (*FoundMoves)[*NumberOfMoves - newMoves][1] = Letters[ActiveCoord[0]];
        (*FoundMoves)[*NumberOfMoves - newMoves][2] = (char)((ActiveCoord[1] +  DeltaRank * 2) + 0x31);
        (*FoundMoves)[*NumberOfMoves - newMoves][3] = '\0';
        newMoves--;
    }

    if (EnPassantLeft) {
        (*FoundMoves)[*NumberOfMoves - newMoves] = malloc(sizeof(char) * 4);
        (*FoundMoves)[*NumberOfMoves - newMoves][0] = BasicMove;
        (*FoundMoves)[*NumberOfMoves - newMoves][1] = Letters[ActiveCoord[0] - 1];
        (*FoundMoves)[*NumberOfMoves - newMoves][2] = (char)((ActiveCoord[1] + DeltaRank) + 0x31);
        (*FoundMoves)[*NumberOfMoves - newMoves][3] = '\0';
        newMoves--;
    }

    if (EnPassantRight) {
        (*FoundMoves)[*NumberOfMoves - newMoves] = malloc(sizeof(char) * 4);
        (*FoundMoves)[*NumberOfMoves - newMoves][0] = BasicMove;
        (*FoundMoves)[*NumberOfMoves - newMoves][1] = Letters[ActiveCoord[0] + 1];
        (*FoundMoves)[*NumberOfMoves - newMoves][2] = (char)((ActiveCoord[1] + DeltaRank) + 0x31);
        (*FoundMoves)[*NumberOfMoves - newMoves][3] = '\0';
        newMoves--;
    }

    if (CaptureLeft && !Promotion) {
        (*FoundMoves)[*NumberOfMoves - newMoves] = malloc(sizeof(char) * 4);
        (*FoundMoves)[*NumberOfMoves - newMoves][0] = BasicMove;
        (*FoundMoves)[*NumberOfMoves - newMoves][1] = Letters[ActiveCoord[0] - 1];
        (*FoundMoves)[*NumberOfMoves - newMoves][2] = (char)((ActiveCoord[1] + DeltaRank) + 0x31);
        (*FoundMoves)[*NumberOfMoves - newMoves][3] = '\0';
        newMoves--;
    }else if (CaptureLeft && Promotion) {
        for (int i = 0; i < 4; i++) {
            (*FoundMoves)[*NumberOfMoves - newMoves] = malloc(sizeof(char) * 5);
            (*FoundMoves)[*NumberOfMoves - newMoves][0] = BasicMove;
            (*FoundMoves)[*NumberOfMoves - newMoves][1] = Letters[ActiveCoord[0] - 1];
            (*FoundMoves)[*NumberOfMoves - newMoves][2] = (char)((ActiveCoord[1] + DeltaRank) + 0x31);
            (*FoundMoves)[*NumberOfMoves - newMoves][3] = PromotionPieces[i];
            (*FoundMoves)[*NumberOfMoves - newMoves][4] = '\0';
            newMoves--;
        }
    }

    if (CaptureRight && !Promotion) {
        (*FoundMoves)[*NumberOfMoves - newMoves] = malloc(sizeof(char) * 4);
        (*FoundMoves)[*NumberOfMoves - newMoves][0] = BasicMove;
        (*FoundMoves)[*NumberOfMoves - newMoves][1] = Letters[ActiveCoord[0] + 1];
        (*FoundMoves)[*NumberOfMoves - newMoves][2] = (char)((ActiveCoord[1] + DeltaRank) + 0x31);
        (*FoundMoves)[*NumberOfMoves - newMoves][3] = '\0';
        newMoves--;
    }else if (CaptureRight && Promotion) {
        for (int i = 0; i < 4; i++) {
            (*FoundMoves)[*NumberOfMoves - newMoves] = malloc(sizeof(char) * 5);
            (*FoundMoves)[*NumberOfMoves - newMoves][0] = BasicMove;
            (*FoundMoves)[*NumberOfMoves - newMoves][1] = Letters[ActiveCoord[0] + 1];
            (*FoundMoves)[*NumberOfMoves - newMoves][2] = (char)((ActiveCoord[1] + DeltaRank) + 0x31);
            (*FoundMoves)[*NumberOfMoves - newMoves][3] = PromotionPieces[i];
            (*FoundMoves)[*NumberOfMoves - newMoves][4] = '\0';
            newMoves--;
        }
    }
}

bool DoPawnMove(enum Piece *Board, char move[5], bool WhiteTurn) {
    int OrigCoord[2] = {INVALID_COORDINATE, INVALID_COORDINATE};
    int NewCoord[2] = {INVALID_COORDINATE, INVALID_COORDINATE};
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
    if (!WhiteTurn) {
        ToFind = B_PAWN;
    }
    enum Piece NewTile = *(Board + NewCoord[0] + NewCoord[1] * 8);
    bool Capture = NewTile != EMPTY;

    if (!IsLegitCoordinate(NewCoord)) {
        return false;
    }
    //Check so that we don't capture our own piece
    if ( Capture && SameColor(WhiteTurn, NewTile)) {
        return false;
    }

    bool EnPassant = IsEnPassant(Board, NewCoord, WhiteTurn);

    if (Capture && Simple) {
        FindSinglePawnCapture(Board, NewCoord, OrigCoord, WhiteTurn);
    }else if (Capture && WhiteTurn) {
        OrigCoord[0] = LetterToCoordinate(move[0]);
        OrigCoord[1] = NewCoord[1] - 1;
    }else if (Capture) {
        OrigCoord[0] = LetterToCoordinate(move[0]);
        OrigCoord[1] = NewCoord[1] + 1;
    }else if (EnPassant) {
        FindSinglePawnCapture(Board, NewCoord, OrigCoord, WhiteTurn);
    }else {
        OrigCoord[0] = LetterToCoordinate(move[0]);
        OrigCoord[1] = FindPawnOnRank(Board, NewCoord[1], OrigCoord[0], WhiteTurn);
    }

    if ((WhiteTurn && NewCoord[1] == 7) || (!WhiteTurn && NewCoord[1] == 0)) {
        char Letter = Simple ? move[2] : move[3];
        return Promote(Board, OrigCoord, NewCoord, Letter, WhiteTurn);
    }

    if (!CorrectMovement(NewCoord, OrigCoord)) {
        return false;
    }

    if (abs(OrigCoord[1] - NewCoord[1]) == 2) {
        if (WhiteTurn) {
            ToFind = W_PAWN_EN;
        }else {
            ToFind = B_PAWN_EN;
        }
    }

    enum Piece const previousPiece = *(Board + NewCoord[0] + NewCoord[1] * 8);
    enum Piece const EnPassantPiece = *(Board + NewCoord[0] + OrigCoord[1] * 8);

    *(Board + OrigCoord[0] + OrigCoord[1] * 8) = EMPTY;
    *(Board + NewCoord[0] + NewCoord[1] * 8) = ToFind;
    if (EnPassant) {
        *(Board + NewCoord[0] + OrigCoord[1] * 8) = EMPTY;
    }

    if (IsCheck(Board, !isBlack(ToFind))) {
        *(Board + OrigCoord[0] + OrigCoord[1] * 8) = ToFind;
        *(Board + NewCoord[0] + NewCoord[1] * 8) = previousPiece;
        *(Board + NewCoord[0] + OrigCoord[1] * 8) = EnPassantPiece;
        return false;
    }

    return true;

}
