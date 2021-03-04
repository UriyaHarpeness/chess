#pragma once

#include "Piece.h"

/**
 * A class representing the Knight piece.
 *
 * @see [Knight](https://en.wikipedia.org/wiki/Knight_(chess))
 */
class Knight : public Piece {
public:

    /**
     * Copy constructor.
     *
     * @param other The piece to copy.
     */
    Knight(const Piece &other) : Piece(other) {}

    /**
     * Constructor.
     *
     * @overload
     */
    Knight(Color color, const Point &position) : Piece(color, position) {}

    /**
     * Get the relative moves of the piece.
     *
     * @overload
     */
    [[nodiscard]] const set<Point> &get_move_relative() const override { return move_relative; };

    /**
     * Get the string representation of the piece.
     *
     * Knight is represented with "Kt".
     *
     * @overload
     */
    [[nodiscard]] inline string get_representation() const override { return "Kt"; };


private:

    /// The piece's relative moves.
    static const set<Point> move_relative;
};
