#include "Piece.h"

const set<Point> Piece::move_vector;

const set<Point> Piece::move_relative;

set<Point> Piece::get_possible_positions(const BoardData &board, unsigned int turn) const {
    set<Point> possible_positions;

    Point relative;
    for (const auto &move : get_move_vector()) {
        for (relative = get_position(), relative += move; relative.in_positive_range(SIZE, SIZE) &&
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
        relative = get_position() + move;
        if (relative.in_positive_range(SIZE, SIZE) && ((board[relative.get_x()][relative.get_y()] == nullptr) ||
                                                       (board[relative.get_x()][relative.get_y()]->get_color() !=
                                                        m_color))) {
            possible_positions.insert(relative);
        }
    }

    return move(possible_positions);
}

set<Point> Piece::get_threatening_positions(const BoardData &board, unsigned int turn) const {
    return move(get_possible_positions(board, turn));
}

