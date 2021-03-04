#pragma once

#include "../board/TwoDimensionalVector.h"
#include "../point/Point.h"

#include <memory>
#include <vector>
#include <set>
#include <string>

using namespace std;

/// Define the board size as 8 (8x8).
#define SIZE 8

/// Enum for the colors of players and pieces.
enum Color : bool {
    BLACK = false,
    WHITE = true
};

/// Forward declaration of the Piece class.
class Piece;

/// Define BoardData as a two dimensional vector of pieces.
typedef TwoDimensionalVector<shared_ptr<Piece>> BoardData;

/// Define change as a pair of pieces (before and after).
typedef pair<shared_ptr<Piece>, shared_ptr<Piece>> change;

/// Define play as a vector of changes, by order.
typedef vector<change> play;

/**
 * A base class for representing chess pieces.
 */
class Piece {
public:

    /**
     * Constructor.
     *
     * @param color     The color of the piece.
     * @param position  The initial position of the piece.
     */
    Piece(Color color, const Point &position) : m_color(color), m_positions({{0, position}}) {};

    /**
     * Get the possible normal moves.
     *
     * @param board The board.
     * @return  The possible normal moves.
     */
    [[nodiscard]] virtual set<Point> get_possible_positions(const BoardData &board) const;

    /**
     * Get the relative moves of the piece.
     *
     * Relative moves are the relative positions to the piece to which it can potentially move.
     *
     * @return  The relative moves.
     */
    [[nodiscard]] virtual const set<Point> &get_move_relative() const { return move_vector; };

    /**
     * Get the vector moves of the piece.
     *
     * Vector moves are the directions in which the piece can potentially move endlessly in a straight line.
     *
     * @return  The vector moves.
     */
    [[nodiscard]] virtual const set<Point> &get_move_vector() const { return move_relative; };

    /**
     * Get the string representation of the piece.
     *
     * @return  The string representation.
     */
    [[nodiscard]] virtual inline string get_representation() const = 0;

    /**
     * Get the color of the piece.
     *
     * @return  The color.
     */
    [[nodiscard]] virtual inline Color get_color() const { return m_color; };

    /**
     * Move the piece.
     *
     * @param turn      The turn number.
     * @param position  The new position of the piece.
     */
    void do_move(unsigned int turn, const Point &position) { m_positions.emplace_back(turn, position); }

    /**
     * Undo the last move.
     */
    void undo_last_move() { m_positions.pop_back(); }

    /**
     * Check if the piece was previously moved.
     *
     * @return  Was the piece previously moved.
     */
    [[nodiscard]] inline bool moved() const { return m_positions.size() > 1; }

    /**
     * Get the current piece's position.
     *
     * @return  The current piece's position.
     */
    [[nodiscard]] inline const Point &get_position() const { return prev(m_positions.end())->second; }

    /**
     * Get the turn in which the piece was lastly moved.
     *
     * @return  The last turn in which the piece moved.
     */
    [[nodiscard]] inline const unsigned int &get_move_turn() const { return prev(m_positions.end())->first; }

    /**
     * Get the number of moves the piece has done.
     *
     * @return  The number of moves the piece has done.
     */
    [[nodiscard]] inline size_t count_moves() const { return size_t(m_positions.size()) - 1; }


protected:

    /// The color of the piece.
    Color m_color;

    /// The positions in which the piece has been.
    vector<pair<unsigned int, Point>> m_positions;


private:

    /// The piece's vector moves.
    static const set<Point> move_vector;

    /// The piece's relative moves.
    static const set<Point> move_relative;
};
