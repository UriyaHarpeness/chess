#pragma once

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(Color color) : Piece(color) {};

    virtual const set<Point> &get_move_vector() override { return move_vector; };

    inline const string get_representation() const override { return "Qn"; };

private:
    static const set<Point> move_vector;
};
