#pragma once

#include "TwoDimensionalVector.h"
#include "../config.h"
#include "../multipiece/MultiPiece.h"
#include "../pieces/King.h"
#include "../pieces/Queen.h"
#include "../pieces/Rook.h"
#include "../pieces/Knight.h"
#include "../pieces/Bishop.h"
#include "../pieces/Pawn.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

using namespace std;

/// A namespace containing the values for different colors in the terminal.
namespace colors {
    static const unsigned char black = 0;
    static const unsigned char white = 255;
    static const unsigned char dark_gray = 243;
    static const unsigned char light_gray = 245;
    static const unsigned char dark_green = 28;
    static const unsigned char light_green = 34;
    static const unsigned char dark_red = 124;
    static const unsigned char light_red = 160;
}

/**
 * A class that manages the board throughout the game.
 */
class Board {
public:

    /**
     * Constructor.
     *
     * Initializes the board with the pieces in their initial positions.
     */
    Board();

    /**
     * Right padding string.
     *
     * @param str       The string to pad.
     * @param length    The desired final length.
     * @param filler    The character used to fill up to the desired length.
     * @return  The padded string.
     */
    static string zfill(string str, unsigned int length, char filler);

    /**
     * Print a string with specified foreground and background colors.
     *
     * @param text          The text to print.
     * @param foreground    The foreground color for the text.
     * @param background    The background color for the text.
     */
    static void print_colorful(const string &text, unsigned char foreground, unsigned char background);

    /**
     * Print a number index.
     *
     * Prints the given number after padding it to a fixed size with colors.
     *
     * @param i The index value to print.
     */
    static void print_index(dimension i);

    /**
     * Print a character index.
     *
     * Prints the given character after padding it to a fixed size with colors.
     *
     * @param i The index value to print.
     */
    static void print_char_index(char i);

    /**
     * Draw the board before choosing a piece to move.
     *
     * @param options   The options for pieces to choose from.
     * @param color     The color whose turn it currently is.
     * @param turn      The turn number.
     */
    void draw_board(const set<Point> &options, Color color, unsigned int turn) const;

    /**
     * Draw the board after choosing a piece to move.
     *
     * @param possible_moves    The possible normal moves to choose from.
     * @param possible_plays    The possible play moves to choose from.
     * @param chosen            The piece chosen to move.
     * @param turn              The turn number.
     */
    void draw_board(const set<Point> &possible_moves, const map<Point, play> &possible_plays, const Point &chosen,
                    unsigned int turn);

    /**
     * Get all the possible normal moves for the given color.
     *
     * @param color The color to get possible normal moves for.
     * @return  All the possible normal moves for the color.
     */
    [[nodiscard]] map<Point, set<Point>> get_possible_moves(Color color) const;

    /**
     * Perform a normal move.
     *
     * @param turn          The turn number.
     * @param source        The moving piece's source.
     * @param destination   The moving piece's destination.
     */
    void do_move(unsigned int turn, const Point &source, const Point &destination);

    /**
     * Revert a normal move.
     *
     * @param source        The moving piece's source.
     * @param destination   The moving piece's destination.
     * @param original      The piece previously in the move's destination, if such exists.
     */
    void undo_move(const Point &source, const Point &destination, shared_ptr<Piece> original);

    /**
     * Get all the pieces endangered by a play move.
     *
     * @param single_play   The play move to get endangered pieces for.
     * @return  A mapping of positions to pieces endangered by the given move.
     */
    [[nodiscard]] map<Point, shared_ptr<Piece>> get_endangered_pieces(const play &single_play) const;

    /**
     * Perform a play move.
     *
     * @param single_play   The play move to perform.
     */
    void do_move(const play &single_play);

    /**
     * Revert a play move.
     *
     * @param endangered_pieces The pieces previously endangered by the play move.
     * @param single_play       The play move to revert.
     */
    void undo_move(const map<Point, shared_ptr<Piece>> &endangered_pieces, const play &single_play);

    /**
     * Get threatened positions of all the given normal and play moves.
     *
     * Checks if the piece chosen to move is threatened in its final position for each of the given moves.
     *
     * @param possible_moves    The possible normal moves.
     * @param possible_plays    The possible play moves.
     * @param chosen            The source position chosen to move.
     * @param turn              The turn number.
     * @return  All the positions threatened in the given moves.
     */
    [[nodiscard]] set<Point>
    get_threatened(const set<Point> &possible_moves, const map<Point, play> &possible_plays, const Point &chosen,
                   unsigned int turn);

    /**
     * Check if the given position is threatened.
     *
     * @param position      The position to check.
     * @param color         The color whose turn it is.
     * @param turn          The turn number.
     * @param threatening   Whether to check only threatening moves.
     * @return  Is the given position threatened.
     */
    [[nodiscard]] bool is_threatened(const Point &position, Color color, unsigned int turn, bool threatening) const;

    /**
     * Check if the given piece is threatened.
     *
     * @param piece      The piece to check.
     * @param turn          The turn number.
     * @param threatening   Whether to check only threatening moves.
     * @return  Is the given piece threatened.
     */
    [[nodiscard]] bool is_threatened(const shared_ptr<Piece> &piece, unsigned int turn, bool threatening) const;

    /**
     * Get all the threatened positions.
     *
     * @param color The color to get threatened positions for.
     * @param turn  The turn number.
     * @return  All the positions threatened.
     */
    [[nodiscard]] set<Point> get_threatened(Color color, unsigned int turn) const;

    /**
     * Filter moves that are illegal to perform.
     *
     * A move that exposes the king is considered illegal.
     *
     * @param possible_moves        Possible normal moves.
     * @param possible_play_moves   Possible play moves.
     * @param color                 The color whose turn it is.
     * @param turn                  The turn number.
     */
    void
    filter_illegal_moves(map<Point, set<Point>> &possible_moves, map<Point, map<Point, play>> &possible_play_moves,
                         Color color, unsigned int turn);

    /**
     * Get the board data.
     *
     * @return  The board data.
     */
    [[nodiscard]] const BoardData &get_board() const { return m_board; };

    /**
     * Get the board data.
     *
     * @return  The board data.
     */
    [[nodiscard]] BoardData &get_mutable_board() { return m_board; };

    /**
     * Find the king's position.
     *
     * @param color The color to find the king for.
     * @return  The king's position.
     */
    [[nodiscard]] Point find_king(Color color) const;


private:

    /// The board data.
    BoardData m_board;
};
