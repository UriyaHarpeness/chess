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

typedef vector<vector<shared_ptr<Piece>>> BoardData;

class Piece {
public:
    explicit Piece(Color color) : m_color(color), m_moved(false) {};

    virtual set<Point> get_possible_positions(const Point &current, const BoardData &board);

    virtual const set<Point> &get_move_vector() { return move_vector; };

    virtual const set<Point> &get_move_relative() { return move_relative; };

    [[nodiscard]] virtual inline const string get_representation() const = 0;

    [[nodiscard]] virtual inline Color get_color() const { return m_color; };

    void set_moved() { m_moved = true; }

protected:
    Color m_color;

    bool m_moved;

private:
    static const set<Point> move_vector;

    static const set<Point> move_relative;
};
