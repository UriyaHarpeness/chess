#pragma once

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(const Piece &other) : Piece(other) {}

    Pawn(Color color, const Point &position) : Piece(color, position) {}

    [[nodiscard]] set<Point> get_possible_positions(const BoardData &board, unsigned int turn) const override;

    [[nodiscard]] inline string get_representation() const override { return "Pn"; };

    [[nodiscard]] inline short int get_move_direction() const { return m_color ? 1 : -1; }
};
