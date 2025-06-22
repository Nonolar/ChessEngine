#include <stdio.h>
#include <stdlib.h>

#include "ChessGame.h"
#include "EvalFunc.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

void InitiateBoard(enum Piece *Board) {
    *Board = W_ROOK;
    *(Board + 1) = W_KNIGHT;
    *(Board + 2) = W_BISHOP;
    *(Board + 3) = W_QUEEN;
    *(Board + 4) = W_KING;
    *(Board + 5) = W_BISHOP;
    *(Board + 6) = W_KNIGHT;
    *(Board + 7) = W_ROOK;

    *(Board + 56) = B_ROOK;
    *(Board + 57) = B_KNIGHT;
    *(Board + 58) = B_BISHOP;
    *(Board + 59) = B_QUEEN;
    *(Board + 60) = B_KING;
    *(Board + 61) = B_BISHOP;
    *(Board + 62) = B_KNIGHT;
    *(Board + 63) = B_ROOK;

    for (int i = 0; i < 8; i++) {
        *(Board + i + 8) = W_PAWN;
        *(Board + i + 48) = B_PAWN;

    }



}

char GetCharOfPiece(enum Piece piece) {
    switch (piece) {
        case B_PAWN:
            return 'P';
        case B_PAWN_EN:
            return 'E';
        case B_ROOK:
            return 'R';
        case B_KNIGHT:
            return 'N';
        case B_BISHOP:
            return 'B';
        case B_KING:
            return 'K';
        case B_QUEEN:
            return 'Q';
        case W_PAWN:
            return 'p';
        case W_PAWN_EN:
            return 'e';
        case W_ROOK:
            return 'r';
        case W_KNIGHT:
            return 'n';
        case W_BISHOP:
            return 'b';
        case W_KING:
            return 'k';
        case W_QUEEN:
            return 'q';
        case EMPTY:
            return ' ';
        default:
            return '#';


    }
}

void RemoveEnPassant(enum Piece *Board, bool White) {
    for (int i = 0; i < 64; i++) {
        if (Board[i] == B_PAWN_EN && !White) {
            Board[i] = B_PAWN;
        }else if (Board[i] == W_PAWN_EN && White) {
            Board[i] = W_PAWN;
        }
    }
}

void RenderBoard(enum Piece *Board) {

    for (int i = 7; i >= 0; i--) {
        printf("+-+-+-+-+-+-+-+-+\n");
        for (int j = 0; j < 8; j++) {
            if (isBlack(*(Board + i * 8 + j))) {
                printf("|" ANSI_COLOR_RED "%c" ANSI_COLOR_RESET, GetCharOfPiece(*(Board + i * 8 + j)));
            }else {
                printf("|%c", GetCharOfPiece(*(Board + i * 8 + j)));
            }

        }
        printf("| %d\n", i + 1);
    }

    printf("+-+-+-+-+-+-+-+-+\n a b c d e f g h\n");
}

bool PlayOneRound(enum Piece *Board, bool WhiteRound) {
    if (WhiteRound) {
        printf("Input white move: ");
    }else {
        printf("Input black move: ");
    }


    char Move[4];
    scanf("%4s", Move);
    system("clear");

    if (!ProcessMove(Board, Move, WhiteRound)) {
        printf("Invalid move %s\n", Move);
        return false;
    }

    return true;
}

int main(void) {
    enum Piece *Board = malloc(sizeof(enum Piece) * 64);
    //malloc(64*sizeof(enum Piece))
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            *(Board + i*8 + j) = EMPTY;
        }
    }
    RenderBoard(Board);
    InitiateBoard(Board);
    printf("\n\n\n");
    RenderBoard(Board);
    float evauluation = Evaluate(Board);
    printf("Evaluation: %f\n", evauluation);

    bool DoWhiteRound = true;
    bool DoBlackRound = true;
    while (!GameIsOver(Board)) {
        if (DoWhiteRound) {
            RemoveEnPassant(Board, true);
            DoBlackRound = PlayOneRound(Board, true);
            RenderBoard(Board);
            evauluation = Evaluate(Board);
            printf("Evaluation: %f\n", evauluation);
        }
        if (DoBlackRound) {
            RemoveEnPassant(Board, false);
            DoWhiteRound =  PlayOneRound(Board, false);
            RenderBoard(Board);
            evauluation = Evaluate(Board);
            printf("Evaluation: %f\n", evauluation);
        }


    }

    return 0;
}