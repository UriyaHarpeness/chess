#pragma once

#include "../pieces/King.h"
#include "../pieces/Queen.h"
#include "../pieces/Rock.h"
#include "../pieces/Knight.h"
#include "../pieces/Bishop.h"
#include "../pieces/Pawn.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>

using namespace std;

class Board {
public:
    static const unsigned char black = 0;
    static const unsigned char white = 255;
    static const unsigned char dark_gray = 243;
    static const unsigned char light_gray = 245;
    static const unsigned char dark_green = 28;
    static const unsigned char light_green = 34;

    Board();

    static string zfill(string str, unsigned int length, char filler);

    static void print_colorful(const string &text, unsigned char foreground, unsigned char background);

    static void print_index(dimension i);

    static void print_char_index(char i);

    void draw_board(const map<Point, set<Point>> &possible_moves) const;

    void draw_board(const set<Point> &possible_moves, const Point &chosen) const;

    [[nodiscard]] map<Point, set<Point>> get_possible_moves(Color color) const;

    void do_move(const Point &source, const Point &destination);

private:
    BoardData m_board;
};
