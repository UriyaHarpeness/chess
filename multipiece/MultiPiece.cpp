#include "MultiPiece.h"

set<play> MultiPiece::get_castling(const Board &board, Color color, unsigned int turn) {
    set<play> plays;
    const auto &board_data = board.get_board();

    unsigned char initial_y = color ? InitialPositions::white_king_y : InitialPositions::black_king_y;

    // Castling cannot be done if the king is threatened, or it has been moved.
    auto possible_king = board_data[{InitialPositions::king_x, initial_y}];
    if ((possible_king == nullptr) || (possible_king->get_color() != color) ||
        (possible_king->get_representation() != "Kg") || (possible_king->moved()) ||
        (board.is_threatened({4, initial_y}, color, turn, true))) {
        return move(plays);
    }

    // Check option of castling for both rooks.
    for (const auto &x : {InitialPositions::left_rook_x, InitialPositions::right_rook_x}) {
        // Castling cannot involve a rook that has been moved.
        auto possible_rook = board_data[{x, initial_y}];
        if ((possible_rook == nullptr) || (possible_rook->get_color() != color) ||
            (possible_rook->get_representation() != "Rk") || (possible_rook->moved())) {
            continue;
        }

        // In castling, the space between the king and the rook - meaning the route of the king, cannot be threatened.
        short int movement = (x > InitialPositions::king_x) ? 1 : -1;
        bool valid = true;
        for (auto current_x = (short int) (InitialPositions::king_x + movement);
             current_x != x; current_x += movement) {
            if ((board_data[{current_x, initial_y}] != nullptr) ||
                (((movement == 1) ? current_x <= InitialPositions::king_x + 2 : current_x >=
                                                                                InitialPositions::king_x - 2) &&
                 board.is_threatened({current_x, initial_y}, color, turn + 1, true))) {
                valid = false;
                break;
            }
        }

        // Create the castling play move if it is valid.
        if (valid) {
            King king(*board_data[{InitialPositions::king_x, initial_y}]);
            Rook rock(*board_data[{x, initial_y}]);
            king.do_move(turn, Point(InitialPositions::king_x + (movement * 2), initial_y));
            rock.do_move(turn, Point(InitialPositions::king_x + movement, initial_y));
            plays.insert({{board_data[{InitialPositions::king_x, initial_y}], make_shared<King>(king)},
                          {board_data[{x, initial_y}],                        make_shared<Rook>(rock)}});
        }
    }

    return move(plays);
}

set<play> MultiPiece::get_en_passant(const Board &board, Color color, unsigned int turn) {
    set<play> plays;
    const auto &board_data = board.get_board();

    unsigned char pawn_first_move_double_y = color ? DestinationPositions::white_pawn_first_move_double_y
                                                   : DestinationPositions::black_pawn_first_move_double_y;

    // Scan the row..
    for (dimension x = 0; x < SIZE; x++) {
        // Only a pawn can perform an en passant.
        if ((board_data[{x, pawn_first_move_double_y}] == nullptr) ||
            (board_data[{x, pawn_first_move_double_y}]->get_color() != color) ||
            (board_data[{x, pawn_first_move_double_y}]->get_representation() != "Pn"))
            continue;

        // Check for both sides.
        for (const auto &relative_x : {-1, 1}) {
            Point en_passant_point(x + relative_x, pawn_first_move_double_y);
            if (!board_data.contains(en_passant_point)) continue;

            // En passant can be only done on pawns immediately after their first double move.
            auto en_passant = board_data[en_passant_point];
            if ((en_passant != nullptr) && (en_passant->get_color() != color) &&
                (en_passant->get_representation() == "Pn") && (en_passant->count_moves() == 1) &&
                (en_passant->get_move_turn() == turn - 1)) {
                // Create the en passant move.
                Pawn pawn(*board_data[{x, pawn_first_move_double_y}]);
                pawn.do_move(turn, en_passant_point + Point(0, pawn.get_move_direction()));
                plays.insert({{en_passant,                                nullptr},
                              {board_data[{x, pawn_first_move_double_y}], make_shared<Pawn>(pawn)}});
            }
        }
    }

    return move(plays);
}

set<play> MultiPiece::get_plays(const Board &board, Color color, unsigned int turn, bool threatening) {
    set<play> plays, other;

    // Skip castling moves if only searching for threatening play moves.
    if (!threatening) {
        // Get castling play moves.
        other = get_castling(board, color, turn);
        plays.insert(other.begin(), other.end());
    }

    // Get en passant play moves.
    other = get_en_passant(board, color, turn);
    plays.insert(other.begin(), other.end());

    return move(plays);
}

bool MultiPiece::is_promotion(const Board &board, const Point &source, const Point &destination) {
    const auto &piece = board.get_board()[source];
    return ((piece->get_representation() == "Pn") && (destination.get_y() == (piece->get_color() ? 0 : 7)));
}

void MultiPiece::perform_promotion(Board &board, const Point &destination, const char promotion) {
    auto &board_data = board.get_mutable_board();
    if (promotion == 'q') {
        // Chose queen.
        board_data[destination] = make_shared<Queen>(*board_data[destination]);
    } else if (promotion == 'r') {
        // Chose rook.
        board_data[destination] = make_shared<Rook>(*board_data[destination]);
    } else if (promotion == 'b') {
        // Chose bishop.
        board_data[destination] = make_shared<Bishop>(*board_data[destination]);
    } else if (promotion == 'n') {
        // Chose knight.
        board_data[destination] = make_shared<Knight>(*board_data[destination]);
    }
}
