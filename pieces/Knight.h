#pragma once

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(Color color, const Point &position) : Piece(color, position) {}

    [[nodiscard]] const set<Point> &get_move_relative() const override { return move_relative; };

    [[nodiscard]] inline string get_representation() const override { return "Kt"; };

private:
    static const set<Point> move_relative;
};
