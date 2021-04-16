#include "Piece.h"

const set<Point> Piece::move_vector;

const set<Point> Piece::move_relative;

set<Point> Piece::get_possible_positions(const BoardData &board) const {
    set<Point> possible_positions;

    Point relative;

    // Get vector positions.
    for (const auto &move : get_move_vector()) {
        // Empty squares.
        for (relative = get_position(), relative += move;
             board.contains(relative) && (board[relative] == nullptr); relative += move) {
            possible_positions.insert(relative);
        }
        // Squares containing the opponent's color.
        if (board.contains(relative) && (board[relative] != nullptr) &&
            (board[relative]->get_color() != m_color)) {
            possible_positions.insert(relative);
        }
    }

    // Get relative positions.
    for (const auto &move : get_move_relative()) {
        relative = get_position() + move;
        // Empty squares or squares containing the opponent's color.
        if (board.contains(relative) && ((board[relative] == nullptr) || (board[relative]->get_color() != m_color))) {
            possible_positions.insert(relative);
        }
    }

    return move(possible_positions);
}
