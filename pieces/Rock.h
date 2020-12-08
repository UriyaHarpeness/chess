#pragma once

#include "Piece.h"

class Rock : public Piece {
public:
    Rock(const Piece &other) : Piece(other) {}

    Rock(Color color, const Point &position) : Piece(color, position) {}

    [[nodiscard]] const set<Point> &get_move_vector() const override { return move_vector; };

    [[nodiscard]] inline string get_representation() const override { return "Rk"; };

private:
    static const set<Point> move_vector;
};
