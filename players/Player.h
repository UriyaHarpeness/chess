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

    template<typename T>
    static set<Point> get_keys(const map<Point, T> &points_map);

    static string turn_to_string(const turn_t &turn);

    static turn_t string_to_turn(const string &turn);

    virtual vector<turn_t> get_turns() = 0;

    virtual void forward_turns(const vector<turn_t> &turns) = 0;

    virtual turn_t
    get_turn(Board &board, const vector<tuple<Point, Point, char>> &turns, map<Point, set<Point>> possible_moves,
             map<Point, map<Point, play>> possible_play_moves, Color color, unsigned int turn) = 0;

    virtual void forward_turn(const turn_t &turn) = 0;
};

template<typename T>
set<Point> Player::get_keys(const map<Point, T> &points_map) {
    set<Point> points;

    for_each(points_map.begin(), points_map.end(), [&points](const auto &p) { points.insert(p.first); });

    return move(points);
}