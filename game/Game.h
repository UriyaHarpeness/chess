#pragma once

#include "../board/Board.h"
#include "../multipiece/MultiPiece.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <termios.h>
#include <vector>

using namespace std;

enum GameStatus : char {
    ONGOING = 0,
    BLACK_RESIGN = 1,
    WHITE_RESIGN = 2,
    BLACK_WIN = 3,
    WHITE_WIN = 4
};

class Game {
public:
    static const set<char> promotion_options;

    /// Quit action identifiers.
    static const set<char> quit_actions;

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

    void turn(GameStatus &status, Color color);

    GameStatus play_game();

private:
    Board m_board;

    unsigned int m_turn;

    vector<tuple<Point, Point, char>> m_turns;
};
