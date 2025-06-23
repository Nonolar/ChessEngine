#ifndef ROOK_H
#define ROOK_H
#include <stdbool.h>
#include <stdlib.h>
#include "../EvalFunc.h"
#pragma once

bool DoRookMove(enum Piece *Board, char *move, bool WhitePlay);

#endif //ROOK_H
