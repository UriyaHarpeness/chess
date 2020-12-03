#include "MultiPiece.h"

set<play> MultiPiece::get_en_passant(const BoardData &board, Color color, unsigned int turn) {
    set<play> plays;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((board[j][i] == nullptr) || (board[j][i]->get_color() != color) ||
                (board[j][i]->get_representation() != "Pn"))
                continue;
            for (const auto &x : {-1, 1}) {
                Point en_passant_point(j + x, i);
                if (!en_passant_point.in_positive_range(SIZE, SIZE)) continue;
                auto en_passant = board[en_passant_point.get_x()][en_passant_point.get_y()];
                if ((en_passant != nullptr) && (en_passant->get_color() != color) &&
                    (en_passant->get_representation() == "Pn") && (en_passant->get_move_turn() == turn - 1)) {
                    Pawn pawn(*board[j][i]);
                    pawn.do_move(turn, en_passant_point + Point(0, pawn.get_move_direction()));
                    plays.insert({{en_passant,  nullptr},
                                  {board[j][i], make_shared<Pawn>(pawn)}});
                }
            }
        }
    }

    return move(plays);
}

set<play> MultiPiece::get_plays(const BoardData &board, Color color, unsigned int turn) {
    set<play> plays, other;

    other = get_en_passant(board, color, turn);
    plays.insert(other.begin(), other.end());

    return move(plays);
}
