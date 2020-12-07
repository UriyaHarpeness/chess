#include "Pawn.h"

set<Point> Pawn::get_possible_positions(const BoardData &board, unsigned int turn) const {
    set<Point> possible_positions;

    if (get_position() == Point(2, 5)) {
        int x = 0;
    }

    // Normal move.
    auto relative = get_position() + Point(0, get_move_direction());
    if (relative.in_positive_range(SIZE, SIZE) && (board[relative.get_x()][relative.get_y()] == nullptr)) {
        possible_positions.insert(relative);
    }

    // First move.
    if (!moved() && !possible_positions.empty()) {
        relative += Point(0, get_move_direction());
        if (relative.in_positive_range(SIZE, SIZE) && (board[relative.get_x()][relative.get_y()] == nullptr)) {
            possible_positions.insert(relative);
        }
    }

    // Capture move.
    for (const auto &x : {-1, 1}) {
        relative = get_position() + Point(x, get_move_direction());
        if (relative.in_positive_range(SIZE, SIZE) && (board[relative.get_x()][relative.get_y()] != nullptr) &&
            (board[relative.get_x()][relative.get_y()]->get_color() != m_color)) {
            possible_positions.insert(relative);
        }
    }

    return move(possible_positions);
}
