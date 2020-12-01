#include "Pawn.h"

set<Point> Pawn::get_possible_positions(const Point &current, const BoardData &board) const {
    set<Point> possible_positions;

    // Normal move.
    auto relative = current + Point(0, get_move_direction());
    if (relative.in_positive_range(SIZE, SIZE) && (board[relative.get_x()][relative.get_y()] == nullptr)) {
        possible_positions.insert(relative);
    }

    if (!moved() && !possible_positions.empty()) {
        relative += Point(0, get_move_direction());
        if (relative.in_positive_range(SIZE, SIZE) && (board[relative.get_x()][relative.get_y()] == nullptr)) {
            possible_positions.insert(relative);
        }
    }

    // Capture move.
    for (const auto &move : {Point(-1, get_move_direction()), Point(1, get_move_direction())}) {
        relative = current + move;
        if (relative.in_positive_range(SIZE, SIZE) && (board[relative.get_x()][relative.get_y()] != nullptr) &&
            (board[relative.get_x()][relative.get_y()]->get_color() != m_color)) {
            possible_positions.insert(relative);
        }
    }

    return move(possible_positions);
}

set<Point> Pawn::get_threatening_positions(const Point &current, const BoardData &board) const {
    set<Point> threatening_positions;
    Point relative;

    for (const auto &move : {Point(-1, get_move_direction()), Point(1, get_move_direction())}) {
        relative = current + move;
        if (relative.in_positive_range(SIZE, SIZE)) {
            threatening_positions.insert(relative);
        }
    }

    return move(threatening_positions);
}
