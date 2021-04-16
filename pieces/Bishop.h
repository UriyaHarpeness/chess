#pragma once

#include "Piece.h"

/**
 * A class representing the Bishop piece.
 *
 * @see [Bishop](https://en.wikipedia.org/wiki/Bishop_(chess))
 */
class Bishop : public Piece {
public:

    /**
     * Copy constructor.
     *
     * @param other The piece to copy.
     */
    Bishop(const Piece &other) : Piece(other) {}

    /**
     * Constructor.
     *
     * @overload
     */
    Bishop(Color color, const Point &position) : Piece(color, position) {}

    /**
     * Get the vector moves of the piece.
     *
     * @overload
     */
    [[nodiscard]] const set<Point> &get_move_vector() const override { return move_vector; };

    /**
     * Get the string representation of the piece.
     *
     * Bishop is represented with "Bp".
     *
     * @overload
     */
    [[nodiscard]] inline string get_representation() const override { return "Bp"; };


private:

    /// The piece's vector moves.
    static const set<Point> move_vector;
};
