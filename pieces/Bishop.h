#pragma once

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(Color color) : Piece(color) {};

    virtual const set<Point> &get_move_vector() override { return move_vector; };

    inline const string get_representation() const override { return "Bp"; };

private:
    static const set<Point> move_vector;
};
