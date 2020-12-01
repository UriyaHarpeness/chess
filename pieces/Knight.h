#pragma once

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(Color color) : Piece(color) {};

    virtual const set<Point> &get_move_relative() override { return move_relative; };

    inline const string get_representation() const override { return "Kt"; };

private:
    static const set<Point> move_relative;
};
