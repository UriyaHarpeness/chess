#include "Piece.h"

const set<Point> Piece::move_vector;

const set<Point> Piece::move_relative;

set<Point> Piece::get_possible_positions(const Point &current, const BoardData &board) {
    set<Point> possible_positions;

    Point relative;
    for (const auto &move : get_move_vector()) {
        relative = current;

        for (relative = current, relative += move; relative.in_positive_range(SIZE, SIZE) &&
                                                   (board[relative.get_x()][relative.get_y()] ==
                                                    nullptr); relative += move) {
            possible_positions.insert(relative);
        }
        if (relative.in_positive_range(SIZE, SIZE) && (board[relative.get_x()][relative.get_y()] != nullptr) &&
            (board[relative.get_x()][relative.get_y()]->get_color() != m_color)) {
            possible_positions.insert(relative);
        }
    }

    for (const auto &move : get_move_relative()) {
        relative = current + move;
        if (relative.in_positive_range(SIZE, SIZE) && ((board[relative.get_x()][relative.get_y()] == nullptr) ||
                                                       (board[relative.get_x()][relative.get_y()]->get_color() !=
                                                        m_color))) {
            possible_positions.insert(relative);
        }
    }

    return move(possible_positions);
}
