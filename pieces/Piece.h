#pragma once

#include "../point/Point.h"

#include <memory>
#include <vector>
#include <set>
#include <string>

using namespace std;

#define SIZE 8

enum Color : bool {
    BLACK = false,
    WHITE = true
};

class Piece;

// todo: maybe unique_ptr would be better here.
typedef vector<vector<shared_ptr<Piece>>> BoardData;

typedef pair<shared_ptr<Piece>, shared_ptr<Piece>> change;

typedef vector<change> play;

class Piece {
public:
    Piece(Color color, const Point &position) : m_color(color), m_positions({{0, position}}) {};

    [[nodiscard]] virtual set<Point> get_possible_positions(const BoardData &board, unsigned int turn) const;

    [[nodiscard]] virtual const set<Point> &get_move_relative() const { return move_vector; };

    [[nodiscard]] virtual const set<Point> &get_move_vector() const { return move_relative; };

    [[nodiscard]] virtual inline string get_representation() const = 0;

    [[nodiscard]] virtual inline Color get_color() const { return m_color; };

    void do_move(unsigned int turn, const Point &position) { m_positions.emplace_back(turn, position); }

    void undo_last_move() { m_positions.pop_back(); }

    [[nodiscard]] inline bool moved() const { return m_positions.size() > 1; }

    [[nodiscard]] inline const Point &get_position() const { return prev(m_positions.end())->second; }

    [[nodiscard]] inline const unsigned int &get_move_turn() const { return prev(m_positions.end())->first; }

    [[nodiscard]] inline size_t count_moves() const { return size_t(m_positions.size()) - 1; }

protected:
    Color m_color;

    vector<pair<unsigned int, Point>> m_positions;

private:
    static const set<Point> move_vector;

    static const set<Point> move_relative;
};
