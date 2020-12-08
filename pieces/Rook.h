#pragma once

#include "Piece.h"

class Rook : public Piece {
public:
    Rook(const Piece &other) : Piece(other) {}

    Rook(Color color, const Point &position) : Piece(color, position) {}

    [[nodiscard]] const set<Point> &get_move_vector() const override { return move_vector; };

    [[nodiscard]] inline string get_representation() const override { return "Rk"; };

private:
    static const set<Point> move_vector;
};
