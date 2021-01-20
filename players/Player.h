#pragma once

#include "../board/Board.h"
#include "../point/Point.h"

#include <map>
#include <set>
#include <tuple>

typedef tuple<Point, Point, char> turn_t;

class Player {
public:
    static const turn_t quit_turn;

    virtual ~Player() = default;

    /*
     * todo: try to use the template function.
     * template<class T>
        static set<Point> get_keys(const map<Point, T> &mapping);
     */
    static set<Point> get_keys(const map<Point, set<Point>> &possible_moves);

    static set<Point> get_keys(const map<Point, map<Point, play>> &possible_play_moves);

    static string turn_to_string(const turn_t &turn);

    static turn_t string_to_turn(const string &turn);

    virtual vector<turn_t> get_turns() = 0;

    virtual void forward_turns(const vector<turn_t> &turns) = 0;

    virtual turn_t
    get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
             map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) = 0;

    virtual void forward_turn(const turn_t &turn) = 0;
};
