#pragma once

#include "Piece.h"

/**
 * A class representing the Pawn piece.
 *
 * @see [Pawn](https://en.wikipedia.org/wiki/Pawn_(chess))
 */
class Pawn : public Piece {
public:

    /**
     * Copy constructor.
     *
     * @param other The piece to copy.
     */
    Pawn(const Piece &other) : Piece(other) {}

    /**
     * Constructor.
     *
     * @overload
     */
    Pawn(Color color, const Point &position) : Piece(color, position) {}

    /**
     * Get the possible normal moves.
     *
     * @overload
     */
    [[nodiscard]] set<Point> get_possible_positions(const BoardData &board) const override;

    /**
     * Get the string representation of the piece.
     *
     * Pawn is represented with "Pn".
     *
     * @overload
     */
    [[nodiscard]] inline string get_representation() const override { return "Pn"; };

    /**
     * Get the direction in which the pawn can move.
     *
     * @return  The direction in which the pawn can move.
     */
    [[nodiscard]] inline short int get_move_direction() const { return m_color ? 1 : -1; }
};
