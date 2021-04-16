#pragma once

#include "Piece.h"

/**
 * A class representing the King piece.
 *
 * @see [King](https://en.wikipedia.org/wiki/King_(chess))
 */
class King : public Piece {
public:

    /**
     * Copy constructor.
     *
     * @param other The piece to copy.
     */
    King(const Piece &other) : Piece(other) {}

    /**
     * Constructor.
     *
     * @overload
     */
    King(Color color, const Point &position) : Piece(color, position) {}

    /**
     * Get the relative moves of the piece.
     *
     * @overload
     */
    [[nodiscard]] const set<Point> &get_move_relative() const override { return move_relative; };

    /**
     * Get the string representation of the piece.
     *
     * King is represented with "Kg".
     *
     * @overload
     */
    [[nodiscard]] inline string get_representation() const override { return "Kg"; };


private:

    /// The piece's relative moves.
    static const set<Point> move_relative;
};
