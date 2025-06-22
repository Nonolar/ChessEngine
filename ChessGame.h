//
// Created by noel on 2025-06-22.
//

#ifndef CHESSGAME_H
#define CHESSGAME_H
#include <stdbool.h>
#include "EvalFunc.h"
#pragma once


bool ProcessMove(enum Piece *Board, char *move, bool WhitePlay);
//bool IsValidMove(enum Piece Board[8][8], char move[4], bool WhitePlay);
bool GameIsOver(enum Piece *Board);

#endif //CHESSGAME_H
