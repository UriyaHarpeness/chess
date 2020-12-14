#include "Player.h"

const turn_t Player::quit_turn = {{0, 0}, {0, 0}, 0};

// todo: make this work as template function.
/*
template<class T>
set<Point> Player::get_keys(const map<Point, T> &mapping) {
    set<Point> points;

    for_each(mapping.begin(), mapping.end(), [&points](const auto &p) { points.insert(p.first); });

    return move(points);
}*/

set<Point> Player::get_keys(const map<Point, set<Point>> &possible_moves) {
    set<Point> points;

    for_each(possible_moves.begin(), possible_moves.end(), [&points](const auto &p) { points.insert(p.first); });

    return move(points);
}

set<Point> Player::get_keys(const map<Point, map<Point, play>> &possible_play_moves) {
    set<Point> points;

    for_each(possible_play_moves.begin(), possible_play_moves.end(),
             [&points](const auto &p) { points.insert(p.first); });

    return move(points);
}

