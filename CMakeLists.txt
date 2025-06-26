cmake_minimum_required(VERSION 3.31)
project(ChessEngine C)

set(CMAKE_C_STANDARD 11)

add_executable(ChessEngine main.c
        EvalFunc.h
        EvalFunc.c
        ChessGame.h
        ChessGame.c
        Pieces/Pawn.h
        Pieces/Pawn.c
        Pieces/Rook.h
        Pieces/Rook.c
        Pieces/Bishop.h
        Pieces/Bishop.c
        Pieces/Queen.h
        Pieces/Queen.c
        Pieces/King.h
        Pieces/King.c
)
