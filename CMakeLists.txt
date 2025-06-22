cmake_minimum_required(VERSION 3.31)
project(ChessEngine C)

set(CMAKE_C_STANDARD 11)

add_executable(ChessEngine main.c
        EvalFunc.h
        EvalFunc.c
        ChessGame.h
        ChessGame.c
)
