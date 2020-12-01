#include "Board.h"

Board::Board() : m_board(8, vector<shared_ptr<Piece>>(8, nullptr)) {
    // Kings.
    m_board[4][0] = make_shared<King>(BLACK);
    m_board[4][SIZE - 1] = make_shared<King>(WHITE);

    // Queens.
    m_board[3][0] = make_shared<Queen>(BLACK);
    m_board[3][SIZE - 1] = make_shared<Queen>(WHITE);

    // Rocks.
    m_board[0][0] = make_shared<Rock>(BLACK);
    m_board[0][SIZE - 1] = make_shared<Rock>(WHITE);
    m_board[SIZE - 1][0] = make_shared<Rock>(BLACK);
    m_board[SIZE - 1][SIZE - 1] = make_shared<Rock>(WHITE);

    // Knights.
    m_board[1][0] = make_shared<Knight>(BLACK);
    m_board[1][SIZE - 1] = make_shared<Knight>(WHITE);
    m_board[SIZE - 2][0] = make_shared<Knight>(BLACK);
    m_board[SIZE - 2][SIZE - 1] = make_shared<Knight>(WHITE);

    // Bishops.
    m_board[2][0] = make_shared<Bishop>(BLACK);
    m_board[2][SIZE - 1] = make_shared<Bishop>(WHITE);
    m_board[SIZE - 3][0] = make_shared<Bishop>(BLACK);
    m_board[SIZE - 3][SIZE - 1] = make_shared<Bishop>(WHITE);

    // Pawns.
    for (int i = 0; i < SIZE; i++) {
        m_board[i][1] = make_shared<Pawn>(BLACK);
        m_board[i][SIZE - 2] = make_shared<Pawn>(WHITE);
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

void Board::draw_board(const map<Point, set<Point>> &possible_moves) const {
    cout << "  ";
    for (dimension y = 0; y < SIZE; y++) print_char_index('A' + y);
    cout << endl;
    unsigned char background;

    for (int i = 0; i < SIZE; i++) {
        print_index(i + 1);
        for (int j = 0; j < SIZE; j++) {
            if (m_board[j][i] == nullptr) {
                print_colorful("  ", 0, ((i + j) % 2 ? dark_gray : light_gray));
                continue;
            }
            if ((possible_moves.find(Point(j, i)) == possible_moves.end())) {
                background = (i + j) % 2 ? dark_gray : light_gray;
            } else {
                background = (i + j) % 2 ? dark_green : light_green;
            }
            print_colorful(m_board[j][i]->get_representation(), (m_board[j][i]->get_color()) ? white : black,
                           background);
        }
        print_index(i + 1);
        cout << endl;
    }

    cout << "  ";
    for (dimension y = 0; y < SIZE; y++) print_char_index('A' + y);
    cout << endl;
}

void Board::draw_board(const set<Point> &possible_moves, const Point &chosen) const {
    cout << "  ";
    for (dimension y = 0; y < SIZE; y++) print_char_index('A' + y);
    cout << endl;
    unsigned char background;

    for (int i = 0; i < SIZE; i++) {
        print_index(i + 1);
        for (int j = 0; j < SIZE; j++) {
            if (Point(j, i) == chosen) {
                background = 220;
            } else if ((possible_moves.find(Point(j, i)) == possible_moves.end())) {
                background = (i + j) % 2 ? dark_gray : light_gray;
            } else {
                background = (i + j) % 2 ? dark_green : light_green;
            }

            if (m_board[j][i] == nullptr) {
                print_colorful("  ", black, background);
            } else {
                print_colorful(m_board[j][i]->get_representation(), (m_board[j][i]->get_color()) ? white : black,
                               background);
            }
        }
        print_index(i + 1);
        cout << endl;
    }

    cout << "  ";
    for (dimension y = 0; y < SIZE; y++) print_char_index('A' + y);
    cout << endl;
}

map<Point, set<Point>> Board::get_possible_moves(Color color) const {
    map<Point, set<Point>> possible_moves;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((m_board[j][i] == nullptr) || (m_board[j][i]->get_color() != color)) continue;
            auto piece_possible_moves = m_board[j][i]->get_possible_positions(Point(j, i), m_board);
            if (!piece_possible_moves.empty()) {
                possible_moves[Point(j, i)] = piece_possible_moves;
            }
        }
    }

    return move(possible_moves);
}

void Board::do_move(const Point &source, const Point &destination) {
    m_board[destination.get_x()][destination.get_y()] = move(m_board[source.get_x()][source.get_y()]);
    m_board[destination.get_x()][destination.get_y()]->set_moved();
}
