#pragma once

#include "Player.h"
#include "../config.h"

#include <algorithm>
#include <iostream>
#include <termios.h>

using namespace std;

/**
 * A local player.
 * Gets the turns from the local player's input.
 */
class Local : public Player {
public:

    /// Inputs that cause a resignation.
    static const set<char> quit_actions;

    /// Options to promote a pawn to.
    static const set<char> promotion_options;

    /// Input meaning reselection of the turn's move.
    static const char reselect_action = 'u';

    /**
     * Constructor.
     */
    Local() = default;

    /**
     * Destructor.
     */
    ~Local() = default;

    /**
     * Match partial input to given options.
     *
     * @param options   The options to try and match to.
     * @param input     The input to match.
     * @param reselect  Whether the input is reselection.
     * @param get       Whether to get input from console.
     * @return
     */
    static Point moves_fast_match(const set<Point> &options, char input, bool &reselect, bool get = false);

    /**
     * Get single keyboard input without enter.
     *
     * A linux implementation for the windows `getch()` function.
     * Based on https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux.
     *
     * @return  The input character.
     */
    static char getch();

    /**
     * Get the turns performed by the player.
     *
     * @overload
     */
    vector<turn_t> get_turns() override;

    /**
     * Forward turns performed by a player.
     *
     * @overload
     */
    void forward_turns(const vector<turn_t> &turns) override;

    /**
    * Get the player's turn.
     *
     * @overload
    */
    turn_t
    get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
             map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) override;

    /**
     * Forward a turn performed by a player.
     *
     * @overload
     */
    void forward_turn(const turn_t &turn) override;
};
