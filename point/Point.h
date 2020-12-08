#pragma once

#include <tuple>

using namespace std;

typedef short int dimension;

class Point {
public:
    Point() : m_x(0), m_y(0) {}

    Point(const Point &other);

    Point(dimension x, dimension y);

    Point(pair<dimension, dimension> point);

    Point operator+(const Point &other) const;

    Point operator+=(const Point &other);

    bool operator==(const Point &other) const;

    bool operator<(const Point &other) const;

    bool in_positive_range(dimension x, dimension y);

    [[nodiscard]] dimension get_x() const { return m_x; }

    [[nodiscard]] dimension get_y() const { return m_y; }

private:
    dimension m_x;
    dimension m_y;
};
