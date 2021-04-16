#pragma once

#include "../board/Board.h"
#include "../pieces/Pawn.h"
#include "../pieces/Piece.h"

class Board;

/**
 * A namespace for handling moves that include multiple pieces, aka. play moves.
 *
 * The relevant moves are:
 * 1.   Castling.
 * 2.   En passant.
 * 3.   Promotion (not a multi-piece move, but not a normal move).
 */
namespace MultiPiece {

    /// A namespace for initial positions of pieces.
    namespace InitialPositions {
        static dimension white_king_y = 0;
        static dimension black_king_y = 7;
        static dimension king_x = 4;
        static dimension left_rook_x = 0;
        static dimension right_rook_x = 7;
    }

    /// A namespace for destination positions of pieces.
    namespace DestinationPositions {
        static dimension white_pawn_first_move_double_y = 4;
        static dimension black_pawn_first_move_double_y = 3;
    }

    /**
     * Get possible castling play moves.
     *
     * @see [Castling](https://en.wikipedia.org/wiki/Castling)
     *
     * @param board The board.
     * @param color The color whose turn it is.
     * @param turn  The turn number.
     * @return  The possible castling play moves.
     */
    set<play> get_castling(const Board &board, Color color, unsigned int turn);

    /**
     * Get possible en passant play moves.
     *
     * @see [En passant](https://en.wikipedia.org/wiki/En_passant)
     *
     * @param board The board.
     * @param color The color whose turn it is.
     * @param turn  The turn number.
     * @return  The possible en passant play moves.
     */
    set<play> get_en_passant(const Board &board, Color color, unsigned int turn);

    /**
     * Get possible play moves.
     *
     * @param board         The board.
     * @param color         The color whose turn it is.
     * @param turn          The turn number.
     * @param threatening   Whether to get only threatening moves, for example: castling is not threatening, while en
     *                      passant is.
     * @return  The possible play moves.
     */
    set<play> get_plays(const Board &board, Color color, unsigned int turn, bool threatening);

    /**
     * Check if a move is a promotion.
     *
     * @see [Promotion](https://en.wikipedia.org/wiki/Promotion_(chess))
     *
     * @param board         The board.
     * @param source        The source of the move.
     * @param destination   The destination of the move.
     * @return  Is the move a promotion.
     */
    bool is_promotion(const Board &board, const Point &source, const Point &destination);

    /**
     * Perform a promotion.
     *
     * @param board         The board.
     * @param destination   The destination of the promotion move.
     * @param promotion     The piece the promotion is to.
     */
    void perform_promotion(Board &board, const Point &destination, char promotion);
};
