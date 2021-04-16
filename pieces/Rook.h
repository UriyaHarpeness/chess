#pragma once

#include "Piece.h"

/**
 * A class representing the Rook piece.
 *
 * @see [Rook](https://en.wikipedia.org/wiki/Rook_(chess))
 */
class Rook : public Piece {
public:

    /**
     * Copy constructor.
     *
     * @param other The piece to copy.
     */
    Rook(const Piece &other) : Piece(other) {}

    /**
     * Constructor.
     *
     * @overload
     */
    Rook(Color color, const Point &position) : Piece(color, position) {}

    /**
     * Get the vector moves of the piece.
     *
     * @overload
     */
    [[nodiscard]] const set<Point> &get_move_vector() const override { return move_vector; };

    /**
     * Get the string representation of the piece.
     *
     * Rook is represented with "Rk".
     *
     * @overload
     */
    [[nodiscard]] inline string get_representation() const override { return "Rk"; };


private:

    /// The piece's vector moves.
    static const set<Point> move_vector;
};
