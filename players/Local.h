#pragma once

#include "Player.h"
#include "../config.h"

#include <algorithm>
#include <iostream>
#include <termios.h>

using namespace std;

class Local : public Player {
public:
    static const set<char> quit_actions;

    static const set<char> promotion_options;

    static const char reselect_action = 'u';

    Local() = default;

    ~Local() = default;

    Point moves_fast_match(const set<Point> &options, char input, bool &reselect, bool get = false);

    /**
     * Get single keyboard input without enter.
     *
     * A linux implementation for the windows `getch()` function.
     * Based on https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux.
     *
     * @return  The input character.
     */
    static char getch();

    turn_t
    get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
             map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) override;

    void forward_turn(const turn_t &turn) override;
};
