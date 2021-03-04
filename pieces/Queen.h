#pragma once

#include "Piece.h"

/**
 * A class representing the Queen piece.
 *
 * @see [Queen](https://en.wikipedia.org/wiki/Queen_(chess))
 */
class Queen : public Piece {
public:

    /**
     * Copy constructor.
     *
     * @param other The piece to copy.
     */
    Queen(const Piece &other) : Piece(other) {}

    /**
     * Constructor.
     *
     * @overload
     */
    Queen(Color color, const Point &position) : Piece(color, position) {}

    /**
     * Get the vector moves of the piece.
     *
     * @overload
     */
    [[nodiscard]] const set<Point> &get_move_vector() const override { return move_vector; };

    /**
     * Get the string representation of the piece.
     *
     * Queen is represented with "Qn".
     *
     * @overload
     */
    [[nodiscard]] inline string get_representation() const override { return "Qn"; };


private:

    /// The piece's vector moves.
    static const set<Point> move_vector;
};
