#pragma once

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

class Board {
public:
    Board();

    static string zfill(string str, unsigned int length, char filler);

    static void print_colorful(const string &text, unsigned char foreground, unsigned char background);

    static void print_index(dimension i);

    static void print_char_index(char i);

    void draw_board(const set<Point> &options, Color color, unsigned int turn) const;

    void draw_board(const set<Point> &possible_moves, const map<Point, play> &possible_plays, const Point &chosen,
                    unsigned int turn);

    [[nodiscard]] map<Point, set<Point>> get_possible_moves(Color color, unsigned int turn) const;

    void do_move(unsigned int turn, const Point &source, const Point &destination);

    void undo_move(const Point &source, const Point &destination, shared_ptr<Piece> original);

    [[nodiscard]] map<Point, shared_ptr<Piece>> get_endangered_pieces(const play &single_play) const;

    void do_move(const play &single_play);

    void undo_move(const map<Point, shared_ptr<Piece>> &endangered_pieces, const play &single_play);

    [[nodiscard]] set<Point>
    get_threatened(const set<Point> &possible_moves, const map<Point, play> &possible_plays, const Point &chosen,
                   unsigned int turn);

    [[nodiscard]] bool is_threatened(const Point &position, Color color, unsigned int turn, bool threatening) const;

    [[nodiscard]] bool is_threatened(const shared_ptr<Piece> &piece, unsigned int turn, bool threatening) const;

    [[nodiscard]] set<Point> get_threatened(Color color, unsigned int turn) const;

    void
    filter_illegal_moves(map<Point, set<Point>> &possible_moves, map<Point, map<Point, play>> &possible_play_moves,
                         Color color, unsigned int turn);

    [[nodiscard]] const BoardData &get_board() const { return m_board; };

    [[nodiscard]] BoardData &get_mutable_board() { return m_board; };

    [[nodiscard]] Point find_king(Color color) const;

private:
    BoardData m_board;
};
