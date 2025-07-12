#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EvalFunc.h"
#include "ChessGame.h"

#include <string.h>

#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include "Pieces/Bishop.h"
#include "Pieces/Queen.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"

enum Types PieceToType(enum Piece piece) {
    switch (piece) {
        case W_ROOK:
            return ROOK;
        case W_ROOK_C:
            return ROOK;
        case W_KNIGHT:
            return KNIGHT;
        case W_BISHOP:
            return BISHOP;
        case W_QUEEN:
            return QUEEN;
        case W_KING:
            return KING;
        case W_KING_C:
            return KING;
        case W_PAWN:
            return PAWN;
        case W_PAWN_EN:
            return PAWN;
        case B_ROOK:
            return ROOK;
        case B_ROOK_C:
            return ROOK;
        case B_KNIGHT:
            return KNIGHT;
        case B_BISHOP:
            return BISHOP;
        case B_QUEEN:
            return QUEEN;
        case B_KING:
            return KING;
        case B_KING_C:
            return KING;
        case B_PAWN:
            return PAWN;
        case B_PAWN_EN:
            return PAWN;
        default:
            return EMPTY_SQR;
    }
}

enum Piece LetterToPiece(char Letter, bool White) {
    if (White) {
        switch (Letter) {
            case 'R':
                return W_ROOK;
            case 'N':
                return W_KNIGHT;
            case 'B':
                return W_BISHOP;
            case 'Q':
                return W_QUEEN;
            default:
                return EMPTY;
        }
    }else {
        switch (Letter) {
            case 'R':
                return B_ROOK;
            case 'N':
                return B_KNIGHT;
            case 'B':
                return B_BISHOP;
            case 'Q':
                return B_QUEEN;
            default:
                return EMPTY;
        }
    }
}

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

bool SameColor(bool White, enum Piece Comparand) {
    if (Comparand == EMPTY) {
        return false;
    }

    if (White && !isBlack(Comparand)) {
        return true;
    }

    if (!White && isBlack(Comparand)) {
        return true;
    }

    return false;
}

bool IsLegitCoordinate(int const *Coordinate) {
    if (Coordinate[0] == INVALID_COORDINATE || Coordinate[1] == INVALID_COORDINATE) {
        return false;
    }

    if (Coordinate[0] < 0 || Coordinate[1] < 0 ) {
        return false;
    }

    if (Coordinate[0] > 7 || Coordinate[1] > 7) {
        return false;
    }

    return true;
}

bool IsTileLight(int Coordinate[]) {
    return Coordinate[0] % 2 != Coordinate[1] % 2;
}

bool CharIsPiece(char const C) {
    if (C != 'K' && C != 'Q' && C != 'R' && C != 'B' && C != 'N') {
        return false;
    }else {
        return true;
    }
}

//This should work for every piece except for pawns
void GetMoves(enum Piece const *BoardState, int const *Coord, int *NumberOfMoves, char const Piece, int const *StepsToTake, bool const OneMove, int MovesSize, char ***Moves) {

    char const *Letters = "abcdefgh";
    char BasicMove[4];

    BasicMove[0] = Piece;
    BasicMove[1] = Letters[Coord[0]];
    BasicMove[2] = (char)(Coord[1] + 0x31);
    BasicMove[3] = '\0';

    int ActiveCoord[2] = {Coord[0], Coord[1]};

    bool const IsWhite = !isBlack(*(BoardState + Coord[0] + Coord[1] * 8));

    enum Piece* placeHolderBoard = malloc(sizeof(enum Piece) * 64);
    enum Piece const PieceUsed = *(BoardState + Coord[0] + Coord[1] * 8);

    for (int i = 0; i < MovesSize; i++) {
        ActiveCoord[0] = Coord[0];
        ActiveCoord[1] = Coord[1];
        int steps = 1;
        ActiveCoord[0] += StepsToTake[i * 2 + 0];
        ActiveCoord[1] += StepsToTake[i * 2 + 1];

        while (!OneMove && IsLegitCoordinate(ActiveCoord) && *(BoardState + ActiveCoord[0] + ActiveCoord[1] * 8) == EMPTY) {
            ActiveCoord[0] += StepsToTake[i * 2 + 0];
            ActiveCoord[1] += StepsToTake[i * 2 + 1];
            steps++;
        }

        if (!IsLegitCoordinate(ActiveCoord) || SameColor(IsWhite, *(BoardState + ActiveCoord[0] + ActiveCoord[1] * SIDE_lENGHT))) {
            ActiveCoord[0] -= StepsToTake[i * 2 + 0];
            ActiveCoord[1] -= StepsToTake[i * 2 + 1];
            steps--;
        }



        if (steps == 0) {
            continue;
        }

        char **NewMoves = malloc(sizeof(char*) * steps);
        bool *IsValidMove = malloc(sizeof(bool) * steps);
        int CurrentCoord[2] = {ActiveCoord[0], ActiveCoord[1]};
        for (int j = 0; j < steps; j++) {
            memcpy(placeHolderBoard, BoardState, sizeof(enum Piece) * 64);
            NewMoves[j] = malloc(sizeof(char) * 6);
            NewMoves[j][0] = BasicMove[0];
            NewMoves[j][1] = BasicMove[1];
            NewMoves[j][2] = BasicMove[2];
            NewMoves[j][3] = Letters[CurrentCoord[0]];
            NewMoves[j][4] = (char)(CurrentCoord[1] + 0x31);
            NewMoves[j][5] = '\0';

            CurrentCoord[0] = Coord[0] + (j + 1) * StepsToTake[i * 2 + 0];
            CurrentCoord[1] = Coord[1] + (j + 1) * StepsToTake[i * 2 + 1];

            IsValidMove[j] = ProcessMove(placeHolderBoard, NewMoves[j], IsWhite);
        }


        int NumberOfNewMoves = 0;
        for (int j = 0; j < steps; j++) {
            NumberOfNewMoves += IsValidMove[j];
        }

        if (NumberOfNewMoves == 0) {
            free(IsValidMove);
            free(NewMoves);
            continue;
        }

        char **NewList = NULL;
        if (*Moves == NULL) {
            *Moves = malloc(sizeof(char*) * NumberOfNewMoves);
            *NumberOfMoves += NumberOfNewMoves;
            NewList = *Moves;
        }else {
            *NumberOfMoves += NumberOfNewMoves;
            NewList = realloc(*Moves, sizeof(char*) * (*NumberOfMoves));

        }

        if (NewList == NULL) {
            fprintf(stderr, "Memory allocation failed %s");
            exit(1);
        }

        *Moves = NewList;


        int iteration = 0;
        while (NumberOfNewMoves > 0) {
            if (IsValidMove[iteration]) {
                (*Moves)[*NumberOfMoves - NumberOfNewMoves] = NewMoves[iteration];
                NumberOfNewMoves--;
            }else {
                free(NewMoves[iteration]);
            }

            //printf("\n%s\n", (*Moves)[*NumberOfMoves - steps]);

            ActiveCoord[0] -= StepsToTake[i * 2 + 0];
            ActiveCoord[1] -= StepsToTake[i * 2 + 1];
            iteration++;
        }
        free(NewMoves);
        free(IsValidMove);
    }

    free(placeHolderBoard);
}


bool PieceSwitch(enum Piece *Board, char move[5], bool WhitePlay) {

    switch (*move) {
        case 'R':
            return DoRookMove(Board, move, WhitePlay);
        case 'B':
            return DoBishopMove(Board, move, WhitePlay);
        case 'Q':
            return DoQueenMove(Board, move, WhitePlay);
        case 'K':
            return DoKingMove(Board, move, WhitePlay);
        case 'N':
            return DoKnightMove(Board, move, WhitePlay);
    }
}

bool ProcessMove(enum Piece *Board, char *move, bool WhitePlay) {
    if (move == NULL) {
        return false;
    }
    if (CharIsPiece(*move)) {
        return PieceSwitch(Board, move, WhitePlay);
    }else if (*move == 'O') {
        return Castle(Board, move, WhitePlay);
    }else {
        return DoPawnMove(Board, move, WhitePlay);
    }
}


