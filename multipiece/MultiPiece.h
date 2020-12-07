#pragma once

#include "../board/Board.h"
#include "../pieces/Pawn.h"
#include "../pieces/Piece.h"

class Board;

class MultiPiece {
public:
    static set<play> get_castling(const Board &board, Color color, unsigned int turn);

    static set<play> get_en_passant(const Board &board, Color color, unsigned int turn);

    static set<play> get_plays(const Board &board, Color color, unsigned int turn, bool threatening);
};
