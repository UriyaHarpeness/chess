#pragma once

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(Color color, const Point &position) : Piece(color, position) {}

    [[nodiscard]] set<Point> get_possible_positions(const Point &current, const BoardData &board) const override;

    [[nodiscard]] set<Point> get_threatening_positions(const Point &current, const BoardData &board) const override;

    [[nodiscard]] inline string get_representation() const override { return "Pn"; };

private:
    [[nodiscard]] inline short int get_move_direction() const { return m_color ? -1 : 1; }
};
