#pragma once

#include "Piece.h"

class King : public Piece {
public:
    King(Color color) : Piece(color) {};

    virtual const set<Point> &get_move_relative() override { return move_relative; };

    inline const string get_representation() const override { return "Kg"; };

private:
    static const set<Point> move_relative;
};
