#pragma once

#include "../board/Board.h"
#include "../pieces/Pawn.h"
#include "../pieces/Piece.h"

class Board;

namespace MultiPiece {
    set<play> get_castling(const Board &board, Color color, unsigned int turn);

    set<play> get_en_passant(const Board &board, Color color, unsigned int turn);

    set<play> get_plays(const Board &board, Color color, unsigned int turn, bool threatening);

    bool is_promotion(const Board &board, const Point &destination);

    void perform_promotion(Board &board, const Point &destination, const char promotion);
};
