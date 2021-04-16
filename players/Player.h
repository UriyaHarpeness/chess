#pragma once

#include "../board/Board.h"
#include "../point/Point.h"

#include <map>
#include <set>
#include <tuple>

/// Define turn_t as a tuple of two points and a character - source, destination, and optionally a promotion.
typedef tuple<Point, Point, char> turn_t;

/**
 * A base class for players.
 */
class Player {
public:

    /// Quit turn definition.
    static const turn_t quit_turn;

    /**
     * Destructor.
     */
    virtual ~Player() = default;

    /**
     * Get the keys of a map of points.
     *
     * @tparam T            Type of the map's values.
     * @param points_map    The map to get keys from.
     * @return  The keys of the map.
     */
    template<typename T>
    static set<Point> get_keys(const map<Point, T> &points_map);

    /**
     * Convert a turn to its string representation.
     *
     * @param turn  The turn to convert.
     * @return  The string representation of the turn.
     */
    static string turn_to_string(const turn_t &turn);

    /**
     * Convert a string representation to a turn.
     *
     * @param turn  The string representation to convert.
     * @return  The turn.
     */
    static turn_t string_to_turn(const string &turn);

    /**
     * Get the turns performed by the player.
     *
     * @return  The turns performed by the player.
     */
    virtual vector<turn_t> get_turns() = 0;

    /**
     * Forward turns performed by a player.
     *
     * @param turns Turns performed by a player.
     */
    virtual void forward_turns(const vector<turn_t> &turns) = 0;

    /**
     * Get the player's turn.
     *
     * @param board                 The board.
     * @param turns                 The turns performed up to this point.
     * @param possible_moves        The possible normal moves.
     * @param possible_play_moves   The possible play moves.
     * @param color                 The player's color.
     * @param turn                  The turn number.
     * @return  The player's turn.
     */
    virtual turn_t
    get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
             map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) = 0;

    /**
     * Forward a turn performed by a player.
     *
     * @param turn  A turn performed by a player.
     */
    virtual void forward_turn(const turn_t &turn) = 0;
};

template<typename T>
set<Point> Player::get_keys(const map<Point, T> &points_map) {
    set<Point> points;

    for_each(points_map.begin(), points_map.end(), [&points](const auto &p) { points.insert(p.first); });

    return move(points);
}
