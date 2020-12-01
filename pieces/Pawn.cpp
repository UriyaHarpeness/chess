#include "Pawn.h"

set<Point> Pawn::get_possible_positions(const Point &current, const BoardData &board) {
    set<Point> possible_positions;

    auto relative = current + Point(0, m_color ? -1 : 1);
    if (relative.in_positive_range(SIZE, SIZE) && ((board[relative.get_x()][relative.get_y()] == nullptr) ||
                                                   (board[relative.get_x()][relative.get_y()]->get_color() !=
                                                    m_color))) {
        possible_positions.insert(relative);
    }

    if (!m_moved && !possible_positions.empty()) {
        relative += Point(0, m_color ? -1 : 1);
        if (relative.in_positive_range(SIZE, SIZE) && ((board[relative.get_x()][relative.get_y()] == nullptr) ||
                                                       (board[relative.get_x()][relative.get_y()]->get_color() !=
                                                        m_color))) {
            possible_positions.insert(relative);
        }
    }

    return move(possible_positions);
}
