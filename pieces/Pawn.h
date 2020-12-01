#pragma once

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(Color color) : Piece(color) {};

    set<Point> get_possible_positions(const Point &current, const BoardData &board);

    inline const string get_representation() const override { return "Pn"; };
};
