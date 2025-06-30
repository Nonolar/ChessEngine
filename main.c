#include <stdio.h>
#include <stdlib.h>

#include "ChessGame.h"
#include "EvalFunc.h"
#include "Pieces/Bishop.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define DEPTH 5
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

void InitiateBoard(enum Piece *Board) {
    *Board = W_ROOK_C;
    *(Board + 1) = W_KNIGHT;
    *(Board + 2) = W_BISHOP;
    *(Board + 3) = W_QUEEN;
    *(Board + 4) = W_KING_C;
    *(Board + 5) = W_BISHOP;
    *(Board + 6) = W_KNIGHT;
    *(Board + 7) = W_ROOK_C;

    *(Board + 56) = B_ROOK_C;
    *(Board + 57) = B_KNIGHT;
    *(Board + 58) = B_BISHOP;
    *(Board + 59) = B_QUEEN;
    *(Board + 60) = B_KING_C;
    *(Board + 61) = B_BISHOP;
    *(Board + 62) = B_KNIGHT;
    *(Board + 63) = B_ROOK_C;


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
            return 'P';
        case B_ROOK:
            return 'R';
        case B_ROOK_C:
            return 'R';
        case B_KNIGHT:
            return 'N';
        case B_BISHOP:
            return 'B';
        case B_KING:
            return 'K';
        case B_KING_C:
            return 'K';
        case B_QUEEN:
            return 'Q';
        case W_PAWN:
            return 'p';
        case W_PAWN_EN:
            return 'p';
        case W_ROOK:
            return 'r';
        case W_ROOK_C:
            return 'r';
        case W_KNIGHT:
            return 'n';
        case W_BISHOP:
            return 'b';
        case W_KING:
            return 'k';
        case W_KING_C:
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
                #ifdef _WIN32
                    printf("|%c", GetCharOfPiece(*(Board + i * 8 + j)));
                #else
                                printf("|" ANSI_COLOR_RED "%c" ANSI_COLOR_RESET , GetCharOfPiece(*(Board + i * 8 + j)));
                #endif

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


    char Move[5];
    scanf("%5s", Move);
    //system("clear");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (!ProcessMove(Board, Move, WhiteRound)) {
        printf("Invalid move %s\n", Move);
        return false;
    }

    return true;
}

int main(void) {
    int sleepTime = 10;
#ifdef WIN32
    sleepTime = 1000;
#else
    sleepTime = 1;
#endif

    setbuf(stdout, 0);
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


    bool whiteBot = false;
    bool blackBot = true;

    bool DoWhiteRound = true;
    bool DoBlackRound = true;
    while (!GameIsOver(Board)) {
        if (DoWhiteRound && !whiteBot) {
            RemoveEnPassant(Board, true);
            DoBlackRound = PlayOneRound(Board, true);
            RenderBoard(Board);
            evauluation = Evaluate(Board);
            printf("Evaluation: %f\n", evauluation);
        }else if (DoWhiteRound) {
            RemoveEnPassant(Board, true);
            char *MoveToDO = Minimax(Board, DEPTH, true, -10000000.0f, 10000000.0f).Move;
            DoBlackRound = ProcessMove(Board, MoveToDO, true);
            printf("Move made: %s\n", MoveToDO);
            RenderBoard(Board);
            evauluation = Evaluate(Board);
            printf("Evaluation: %f\n", evauluation);
        }
        //char ch = getchar();
        if (DoBlackRound && !blackBot) {
            RemoveEnPassant(Board, false);
            DoWhiteRound =  PlayOneRound(Board, false);
            RenderBoard(Board);
            evauluation = Evaluate(Board);
            printf("Evaluation: %f\n", evauluation);
        }else if (DoBlackRound) {
            RemoveEnPassant(Board, false);
            char *MoveToDO = Minimax(Board, DEPTH, false, -10000000.0f, 10000000.0f).Move;
            DoWhiteRound = ProcessMove(Board, MoveToDO, false);
            printf("Move made: %s\n", MoveToDO);
            RenderBoard(Board);
            evauluation = Evaluate(Board);
            printf("Evaluation: %f\n", evauluation);
        }


    }

    return 0;
}