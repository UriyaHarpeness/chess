#include "Pawn.h"

set<Point> Pawn::get_possible_positions(const BoardData &board) const {
    set<Point> possible_positions;

    // Normal move.
    auto relative = get_position() + Point(0, get_move_direction());
    if (board.contains(relative) && (board[relative] == nullptr)) {
        possible_positions.insert(relative);
    }

    // First move.
    if (!moved() && !possible_positions.empty()) {
        relative += Point(0, get_move_direction());
        if (board.contains(relative) && (board[relative] == nullptr)) {
            possible_positions.insert(relative);
        }
    }

    // Capture move.
    for (const auto &x : {-1, 1}) {
        relative = get_position() + Point(x, get_move_direction());
        if (board.contains(relative) && (board[relative] != nullptr) &&
            (board[relative]->get_color() != m_color)) {
            possible_positions.insert(relative);
        }
    }

    return move(possible_positions);
}
