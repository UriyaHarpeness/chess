#pragma once

#include "../board/Board.h"
#include "../multipiece/MultiPiece.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <termios.h>
#include <vector>

using namespace std;

class Game {
public:

    static const char undo_action = 'u';

    Game();

    template<class T>
    static set<Point> get_keys(const map<Point, T> &mapping);

    static Point moves_fast_match(const set<Point> &options, char input, bool get = false);

    /**
     * Get single keyboard input without enter.
     *
     * A linux implementation for the windows `getch()` function.
     * Based on https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux.
     *
     * @return  The input character.
     */
    static char getch();

    void turn(bool &quit, Color color);

    bool play_game();

private:
    Board m_board;

    unsigned int m_turn;

    vector<pair<Point, Point>> m_turns;
};
