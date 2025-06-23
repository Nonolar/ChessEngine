#ifndef BISHOP_H
#define BISHOP_H
#define NOT_FOUND -900
#include <stdbool.h>
#include <stdlib.h>
#include "../EvalFunc.h"
#pragma once

bool DoBishopMove(enum Piece *Board, char Move[5], bool IsWhite);

#endif //BISHOP_H
