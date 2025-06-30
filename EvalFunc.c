#include <stdio.h>
#include <stdbool.h>
#include "EvalFunc.h"

#include "ChessGame.h"
#include "Pieces/Bishop.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"

bool SquareUnderAttack(enum Piece const *board, int const *Coord, bool CheckForWhite) {
    int *AttackingPieceCoord = FindRook(board, Coord, !CheckForWhite, NOT_FOUND);
    if (IsLegitCoordinate(AttackingPieceCoord)) {
        free(AttackingPieceCoord);
        return true;
    }
    free(AttackingPieceCoord);

    AttackingPieceCoord = FindKnight(board, Coord, !CheckForWhite, NOT_FOUND);
    if (IsLegitCoordinate(AttackingPieceCoord)) {
        free(AttackingPieceCoord);
        return true;
    }
    free(AttackingPieceCoord);

    AttackingPieceCoord = FindBishop(board, Coord, !CheckForWhite, NOT_FOUND);
    if (IsLegitCoordinate(AttackingPieceCoord)) {
        free(AttackingPieceCoord);
        return true;
    }
    free(AttackingPieceCoord);

    AttackingPieceCoord = FindKing(board, Coord, !CheckForWhite);
    if (IsLegitCoordinate(AttackingPieceCoord)) {
        free(AttackingPieceCoord);
        return true;
    }
    free(AttackingPieceCoord);

    AttackingPieceCoord = FindQueen(board, Coord, CheckForWhite ? B_QUEEN : W_QUEEN, NOT_FOUND);
    if (IsLegitCoordinate(AttackingPieceCoord)) {
        free(AttackingPieceCoord);
        return true;
    }
    free(AttackingPieceCoord);

    AttackingPieceCoord = malloc(sizeof(int) * 2);
    FindSinglePawnCapture(board, Coord, AttackingPieceCoord, !CheckForWhite);
    if (IsLegitCoordinate(AttackingPieceCoord)) {
        free(AttackingPieceCoord);
        return true;
    }
    free(AttackingPieceCoord);
    return false;
}

bool IsCheck(enum Piece const *board, bool const White) {
    enum Piece KingToFind = White ? W_KING : B_KING;
    enum Piece foundPiece = EMPTY;

    int Location = -1;

    for (int i = 0; i < SIDE_lENGHT * SIDE_lENGHT && Location == -1; i++) {
        foundPiece = *(board + i);
        Location = foundPiece == KingToFind ? i : Location;

    }

    if (Location == -1) {
        KingToFind = White ? W_KING_C : B_KING_C;
        for (int i = 0; i < SIDE_lENGHT * SIDE_lENGHT && Location == -1; i++) {
            foundPiece = *(board + i);
            Location = foundPiece == KingToFind ? i : Location;
        }
    }


    int Coord[2];
    Coord[0] = Location % 8;
    Coord[1] = (Location - Coord[0])/8;

    return  SquareUnderAttack(board, Coord, White);
}

bool isBlack(enum Piece piece) {
    switch (piece) {
        case B_PAWN:
            return true;
        case B_PAWN_EN:
            return true;
        case B_ROOK:
            return true;
        case B_ROOK_C:
            return true;
        case B_KNIGHT:
            return true;
        case B_BISHOP:
            return true;
        case B_KING:
            return true;
        case B_KING_C:
            return true;
        case B_QUEEN:
            return true;
        default:
            return false;
    }
}

int GetPieceScore(enum Piece piece) {
    switch (piece) {
        case B_PAWN:
            return 1;
        case B_PAWN_EN:
            return 1;
        case B_ROOK:
            return 5;
        case B_ROOK_C:
            return 5;
        case B_KNIGHT:
            return 3;
        case B_BISHOP:
            return 3;
        case B_QUEEN:
            return 9;
        case B_KING:
            return 999;
        case B_KING_C:
            return 1000;
        case W_PAWN:
            return 1;
        case W_PAWN_EN:
            return 1;
        case W_ROOK:
            return 5;
        case W_ROOK_C:
            return 5;
        case W_KNIGHT:
            return 3;
        case W_BISHOP:
            return 3;
        case W_QUEEN:
            return 9;
        case W_KING:
            return 999;
        case W_KING_C:
            return 1000;
        default:
            return 0;

    }
}

void GetMoveSwitchBox(enum Piece *board, int *Coord,  enum Piece foundPiece, char **FoundMoves, int *NumberOfMoves) {
    enum Types ActiveType = PieceToType(foundPiece);
    switch (ActiveType) {
        case BISHOP:
            GetBishopMoves(board, Coord, NumberOfMoves, FoundMoves);
            break;
        case ROOK:
            GetRookMoves(board, Coord, NumberOfMoves, FoundMoves);
            break;
        default:
            printf("Somethig wen't wrong\n");
    }
}

void GetAllMoves(enum Piece *BoardState, bool White) {
    char **moves = NULL;
    int NumberOfMoves = 0;
    //Loop through all pieces and get the moves for the corresponding piece
    for (int i = 0; i < SIDE_lENGHT * SIDE_lENGHT; i++) {
        enum Piece activePiece = *(BoardState + i);
        if (!SameColor(White, activePiece)) {
            continue;
        }

        int Coordinate[2] = {i % 8, (i - i % 8) / 8};

        GetMoveSwitchBox(BoardState, Coordinate, activePiece, moves, &NumberOfMoves);
    }

}

float getEvalScorePiece(enum Piece *piece) {
    if (*piece == EMPTY) {
        return 0.0f;
    }

    if (isBlack(*piece)) {
        float Score = (float)GetPieceScore(*piece);
        return Score * -1.0f;
    }

    return (float)GetPieceScore(*piece);
}

float CountPieces(enum Piece *Board) {
    float result = 0.0f;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            result += getEvalScorePiece((Board + i * 8 + j));
        }
    }

    return result;
}

float Evaluate(enum Piece *Board) {

    float Evaluation = 0;

    Evaluation += CountPieces(Board);

    return Evaluation;
}
