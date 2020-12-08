#include "MultiPiece.h"

set<play> MultiPiece::get_castling(const Board &board, Color color, unsigned int turn) {
    set<play> plays;
    const auto &board_data = board.get_board();

    unsigned char initial_y = color ? 7 : 0;

    auto possible_king = board_data[4][initial_y];
    if ((possible_king == nullptr) || (possible_king->get_color() != color) ||
        (possible_king->get_representation() != "Kg") || (possible_king->moved()) ||
        (board.is_threatened({4, initial_y}, color, turn, true))) {
        return move(plays);
    }

    for (const auto &x : {0, 7}) {
        auto castling = board_data[x][initial_y];
        if ((castling == nullptr) || (castling->get_color() != color) ||
            (castling->get_representation() != "Rk") || (castling->moved())) {
            continue;
        }

        short int movement = (x > 4) ? 1 : -1;
        bool valid = true;
        for (short int current_x = 4 + movement; current_x != x; current_x += movement) {
            if ((board_data[current_x][initial_y] != nullptr) ||
                (((movement == 1) ? current_x <= 4 + 2 : current_x >= 4 - 2) &&
                 board.is_threatened({current_x, initial_y}, color, turn + 1, true))) {
                valid = false;
                break;
            }
        }

        if (valid) {
            King king(*board_data[4][initial_y]);
            Rook rock(*board_data[x][initial_y]);
            king.do_move(turn, Point(4 + (movement * 2), initial_y));
            rock.do_move(turn, Point(4 + movement, initial_y));
            plays.insert({{board_data[4][initial_y], make_shared<King>(king)},
                          {board_data[x][initial_y], make_shared<Rook>(rock)}});
        }
    }

    return move(plays);
}

set<play> MultiPiece::get_en_passant(const Board &board, Color color, unsigned int turn) {
    set<play> plays;
    const auto &board_data = board.get_board();

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((board_data[j][i] == nullptr) || (board_data[j][i]->get_color() != color) ||
                (board_data[j][i]->get_representation() != "Pn"))
                continue;
            for (const auto &x : {-1, 1}) {
                Point en_passant_point(j + x, i);
                if (!en_passant_point.in_positive_range(SIZE, SIZE)) continue;
                auto en_passant = board_data[en_passant_point.get_x()][en_passant_point.get_y()];
                if ((en_passant != nullptr) && (en_passant->get_color() != color) &&
                    (en_passant->get_representation() == "Pn") && (en_passant->count_moves() == 1) &&
                    (en_passant->get_move_turn() == turn - 1)) {
                    Pawn pawn(*board_data[j][i]);
                    pawn.do_move(turn, en_passant_point + Point(0, pawn.get_move_direction()));
                    plays.insert({{en_passant,       nullptr},
                                  {board_data[j][i], make_shared<Pawn>(pawn)}});
                }
            }
        }
    }

    return move(plays);
}

set<play> MultiPiece::get_plays(const Board &board, Color color, unsigned int turn, bool threatening) {
    set<play> plays, other;

    if (!threatening) {
        other = get_castling(board, color, turn);
        plays.insert(other.begin(), other.end());
    }
    other = get_en_passant(board, color, turn);
    plays.insert(other.begin(), other.end());

    return move(plays);
}

bool MultiPiece::is_promotion(const Board &board, const Point &destination) {
    const auto &board_data = board.get_board();
    auto piece = board_data[destination.get_x()][destination.get_y()];
    return ((piece->get_representation() == "Pn") && (destination.get_y() == (piece->get_color() ? 0 : 7)));
}

void MultiPiece::perform_promotion(Board &board, const Point &destination, const char promotion) {
    auto &board_data = board.get_mutable_board();
    if (promotion == 'q') {
        board_data[destination.get_x()][destination.get_y()] = make_shared<Queen>(
                *board_data[destination.get_x()][destination.get_y()]);
    } else if (promotion == 'r') {
        board_data[destination.get_x()][destination.get_y()] = make_shared<Rook>(
                *board_data[destination.get_x()][destination.get_y()]);
    } else if (promotion == 'b') {
        board_data[destination.get_x()][destination.get_y()] = make_shared<Bishop>(
                *board_data[destination.get_x()][destination.get_y()]);
    } else if (promotion == 'n') {
        board_data[destination.get_x()][destination.get_y()] = make_shared<Knight>(
                *board_data[destination.get_x()][destination.get_y()]);
    }
}
