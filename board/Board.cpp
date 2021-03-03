#include "Board.h"

Board::Board() : m_board(SIZE, SIZE) {
    // Kings.
    m_board[{4, 0}] = make_shared<King>(WHITE, Point{4, 0});
    m_board[{4, SIZE - 1}] = make_shared<King>(BLACK, Point{4, SIZE - 1});

    // Queens.
    m_board[{3, 0}] = make_shared<Queen>(WHITE, Point{3, 0});
    m_board[{3, SIZE - 1}] = make_shared<Queen>(BLACK, Point{3, SIZE - 1});

    // Rooks.
    m_board[{0, 0}] = make_shared<Rook>(WHITE, Point{0, 0});
    m_board[{0, SIZE - 1}] = make_shared<Rook>(BLACK, Point{0, SIZE - 1});
    m_board[{SIZE - 1, 0}] = make_shared<Rook>(WHITE, Point{SIZE - 1, 0});
    m_board[{SIZE - 1, SIZE - 1}] = make_shared<Rook>(BLACK, Point{SIZE - 1, SIZE - 1});

    // Knights.
    m_board[{1, 0}] = make_shared<Knight>(WHITE, Point{1, 0});
    m_board[{1, SIZE - 1}] = make_shared<Knight>(BLACK, Point{1, SIZE - 1});
    m_board[{SIZE - 2, 0}] = make_shared<Knight>(WHITE, Point{SIZE - 2, 0});
    m_board[{SIZE - 2, SIZE - 1}] = make_shared<Knight>(BLACK, Point{SIZE - 2, SIZE - 1});

    // Bishops.
    m_board[{2, 0}] = make_shared<Bishop>(WHITE, Point{2, 0});
    m_board[{2, SIZE - 1}] = make_shared<Bishop>(BLACK, Point{2, SIZE - 1});
    m_board[{SIZE - 3, 0}] = make_shared<Bishop>(WHITE, Point{SIZE - 3, 0});
    m_board[{SIZE - 3, SIZE - 1}] = make_shared<Bishop>(BLACK, Point{SIZE - 3, SIZE - 1});

    // Pawns.
    for (dimension i = 0; i < SIZE; i++) {
        m_board[{i, 1}] = make_shared<Pawn>(WHITE, Point{i, 1});
        m_board[{i, SIZE - 2}] = make_shared<Pawn>(BLACK, Point{i, SIZE - 2});
    }
}

string Board::zfill(string str, unsigned int length, char filler) {
    if (str.length() < length) {
        str.insert(0, string(length - str.length(), filler));
    }
    return str;
}

void Board::print_colorful(const string &text, unsigned char foreground, unsigned char background) {
    cout << "\u001b[48;5;" << (int) background << ";38;5;" << (int) foreground << "m" << text << "\u001b[0m";
}

void Board::print_index(const dimension i) {
    print_colorful(zfill(to_string(min((int) i, 99)), 2, '0'), (i % 2) ? 250 : 237, (i % 2) ? 237 : 250);
}

void Board::print_char_index(const char i) {
    print_colorful({i, ' '}, (i % 2) ? 250 : 237, (i % 2) ? 237 : 250);
}

void Board::draw_board(const set<Point> &options, Color color, unsigned int turn) const {
    cout << "  ";
    for (dimension y = 0; y < SIZE; y++) print_char_index((char) ('A' + y));
    cout << endl;
    unsigned char background;

#if DISPLAY_THREATENED
    auto threatened = get_threatened(color, turn);
#endif // DISPLAY_THREATENED

    for (dimension i = SIZE - 1; i >= 0; i--) {
        print_index(i + 1);
        for (dimension j = 0; j < SIZE; j++) {
            if (m_board[{j, i}] == nullptr) {
                print_colorful("  ", 0, ((i + j) % 2 ? colors::light_gray : colors::dark_gray));
                continue;
            }
#if DISPLAY_HINTS
            if ((options.find({j, i}) != options.end())) {
#if DISPLAY_THREATENED
                if (threatened.find({j, i}) != threatened.end()) {
                    background = (i + j) % 2 ? colors::light_red : colors::dark_red;
                } else {
#endif // DISPLAY_THREATENED
#endif // DISPLAY_HINTS
                    background = (i + j) % 2 ? colors::light_green : colors::dark_green;
#if DISPLAY_THREATENED
                }
#endif // DISPLAY_THREATENED
#if DISPLAY_HINTS
            } else {
                background = (i + j) % 2 ? colors::light_gray : colors::dark_gray;
            }
#endif // DISPLAY_HINTS
            print_colorful(m_board[{j, i}]->get_representation(),
                           (m_board[{j, i}]->get_color()) ? colors::white : colors::black, background);
        }
        print_index(i + 1);
        cout << endl;
    }

    cout << "  ";
    for (dimension y = 0; y < SIZE; y++) print_char_index((char) ('A' + y));
    cout << endl;
}

void Board::draw_board(const set<Point> &possible_moves, const map<Point, play> &possible_plays, const Point &chosen,
                       unsigned int turn) {
    cout << "  ";
    for (dimension y = 0; y < SIZE; y++) print_char_index((char) ('A' + y));
    cout << endl;
    unsigned char background;

#if DISPLAY_THREATENED
    auto threatened = get_threatened(possible_moves, possible_plays, chosen, turn);
#endif // DISPLAY_THREATENED

    for (dimension i = SIZE - 1; i >= 0; i--) {
        print_index(i + 1);
        for (dimension j = 0; j < SIZE; j++) {
            if (Point(j, i) == chosen) {
                background = 220;
#if DISPLAY_HINTS
            } else if ((possible_moves.find({j, i}) == possible_moves.end())) {
#else
                }
#endif // DISPLAY_HINTS
                background = (i + j) % 2 ? colors::light_gray : colors::dark_gray;
#if DISPLAY_HINTS
            } else {
#if DISPLAY_THREATENED
                if (threatened.find({j, i}) != threatened.end()) {
                    background = (i + j) % 2 ? colors::light_red : colors::dark_red;
                } else {
#endif // DISPLAY_THREATENED
                    background = (i + j) % 2 ? colors::light_green : colors::dark_green;
#if DISPLAY_THREATENED
                }
#endif // DISPLAY_THREATENED
            }
#endif // DISPLAY_HINTS

            if (m_board[{j, i}] == nullptr) {
                print_colorful("  ", colors::black, background);
            } else {
                print_colorful(m_board[{j, i}]->get_representation(),
                               (m_board[{j, i}]->get_color()) ? colors::white : colors::black, background);
            }
        }
        print_index(i + 1);
        cout << endl;
    }

    cout << "  ";
    for (dimension y = 0; y < SIZE; y++) print_char_index((char) ('A' + y));
    cout << endl;
}

map<Point, set<Point>> Board::get_possible_moves(Color color) const {
    map<Point, set<Point>> possible_moves;

    for (dimension i = 0; i < SIZE; i++) {
        for (dimension j = 0; j < SIZE; j++) {
            if ((m_board[{j, i}] == nullptr) || (m_board[{j, i}]->get_color() != color)) continue;
            auto piece_possible_moves = m_board[{j, i}]->get_possible_positions(m_board);
            if (!piece_possible_moves.empty()) {
                possible_moves[{j, i}] = piece_possible_moves;
            }
        }
    }

    return move(possible_moves);
}

void Board::do_move(unsigned int turn, const Point &source, const Point &destination) {
    m_board[destination] = move(m_board[source]);
    m_board[destination]->do_move(turn, destination);
}

void Board::undo_move(const Point &source, const Point &destination, shared_ptr<Piece> original) {
    m_board[destination]->undo_last_move();
    m_board[source] = move(m_board[destination]);
    // Revive the previous piece if such exists.
    m_board[destination] = move(original);
}

map<Point, shared_ptr<Piece>> Board::get_endangered_pieces(const play &single_play) const {
    map<Point, shared_ptr<Piece>> endangered_pieces;
    for (const auto &move : single_play) {
        if (move.second != nullptr) {
            endangered_pieces[move.second->get_position()] = m_board[move.second->get_position()];
        }
        endangered_pieces[move.first->get_position()] = m_board[move.first->get_position()];
    }
    return move(endangered_pieces);
}

void Board::do_move(const play &single_play) {
    for (const auto &move : single_play) {
        if (move.second != nullptr) {
            m_board[move.second->get_position()] = move.second;
        }
        m_board[move.first->get_position()] = nullptr;
    }
}

void Board::undo_move(const map<Point, shared_ptr<Piece>> &endangered_pieces, const play &single_play) {
    for (const auto &move : single_play) {
        if (move.second != nullptr) {
            m_board[move.second->get_position()] = endangered_pieces.at(move.second->get_position());
        }
        m_board[move.first->get_position()] = endangered_pieces.at(move.first->get_position());
    }
}

set<Point>
Board::get_threatened(const set<Point> &possible_moves, const map<Point, play> &possible_plays, const Point &chosen,
                      unsigned int turn) {
    set<Point> threatened;

    for (const auto &move : possible_moves) {
        shared_ptr<Piece> original = m_board[move];
        do_move(turn, chosen, move);
        if (is_threatened(m_board[move], turn, true)) {
            threatened.insert(move);
        }
        undo_move(chosen, move, original);
    }

    for (const auto &single_play : possible_plays) {
        auto move = single_play.first;
        const auto endangered_pieces = get_endangered_pieces(single_play.second);
        do_move(single_play.second);
        if (is_threatened(m_board[move], turn, true)) {
            threatened.insert(move);
        }
        undo_move(endangered_pieces, single_play.second);
    }

    return move(threatened);
}

bool Board::is_threatened(const Point &position, Color color, unsigned int turn, bool threatening) const {
    const map<Point, set<Point>> possible_moves = get_possible_moves(color ? BLACK : WHITE);
    if (any_of(possible_moves.begin(), possible_moves.end(), [&position](const auto &moves) {
        return any_of(moves.second.begin(), moves.second.end(),
                      [&position](const auto &destination) { return destination == position; });
    })) {
        return true;
    }

    map<Point, map<Point, play>> possible_play_moves;
    const auto possible_plays = MultiPiece::get_plays(*this, color ? BLACK : WHITE, turn + 1, threatening);
    for (const auto &possible_play : possible_plays) {
        for (const auto &single_change : possible_play) {
            if ((single_change.first != nullptr) && (single_change.second == nullptr) &&
                (single_change.first->get_position() == position)) {
                return true;
            }
        }
    }

    return false;
}

bool Board::is_threatened(const shared_ptr<Piece> &piece, unsigned int turn, bool threatening) const {
    return is_threatened(piece->get_position(), piece->get_color(), turn, threatening);
}

set<Point> Board::get_threatened(Color color, unsigned int turn) const {
    set<Point> threatened;
    const map<Point, set<Point>> possible_moves = get_possible_moves(color ? BLACK : WHITE);
    for_each(possible_moves.begin(), possible_moves.end(),
             [&threatened](const auto &moves) { threatened.insert(moves.second.begin(), moves.second.end()); });

    map<Point, map<Point, play>> possible_play_moves;
    const auto possible_plays = MultiPiece::get_plays(*this, color ? BLACK : WHITE, turn + 1, true);
    for (const auto &possible_play : possible_plays) {
        for (const auto &single_change : possible_play) {
            if ((single_change.first != nullptr) && (single_change.second == nullptr)) {
                threatened.insert(single_change.first->get_position());
            }
        }
    }

    return move(threatened);
}

void
Board::filter_illegal_moves(map<Point, set<Point>> &possible_moves, map<Point, map<Point, play>> &possible_play_moves,
                            Color color, unsigned int turn) {
    for (auto moves = possible_moves.begin(); moves != possible_moves.end();) {
        for (auto move = moves->second.begin(); move != moves->second.end();) {
            auto source = moves->first, destination = *move;
            shared_ptr<Piece> original = m_board[destination];
            do_move(turn, source, destination);
            const auto king_position = find_king(color);
            bool king_threatened = is_threatened(king_position, color, turn, true);
            undo_move(source, destination, original);

            if (king_threatened) {
                move = moves->second.erase(move);
            } else {
                move++;
            }
        }

        if (moves->second.empty()) {
            moves = possible_moves.erase(moves);
        } else {
            moves++;
        }
    }

    for (auto possible_plays = possible_play_moves.begin(); possible_plays != possible_play_moves.end();) {
        for (auto possible_play = possible_plays->second.begin(); possible_play != possible_plays->second.end();) {
            const auto endangered_pieces = get_endangered_pieces(possible_play->second);
            do_move(possible_play->second);
            const auto king_position = find_king(color);
            bool king_threatened = is_threatened(king_position, color, turn, true);
            undo_move(endangered_pieces, possible_play->second);

            if (king_threatened) {
                possible_play = possible_plays->second.erase(possible_play);
            } else {
                possible_play++;
            }
        }

        if (possible_plays->second.empty()) {
            possible_plays = possible_play_moves.erase(possible_plays);
        } else {
            possible_plays++;
        }
    }
}

Point Board::find_king(Color color) const {
    for (dimension i = 0; i < SIZE; i++) {
        for (dimension j = 0; j < SIZE; j++) {
            if ((m_board[{j, i}] != nullptr) && (m_board[{j, i}]->get_representation() == "Kg") &&
                (m_board[{j, i}]->get_color() == color)) {
                return Point(j, i);
            }
        }
    }

    return Point(-1, -1);
}
