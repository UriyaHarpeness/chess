#pragma once

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(Color color, const Point &position) : Piece(color, position) {}

    [[nodiscard]] const set<Point> &get_move_vector() const override { return move_vector; };

    [[nodiscard]] inline string get_representation() const override { return "Qn"; };

private:
    static const set<Point> move_vector;
};
