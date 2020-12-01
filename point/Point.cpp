#include "Point.h"

Point::Point(const Point &other) : m_x(other.m_x), m_y(other.m_y) {}

Point::Point(dimension x, dimension y) : m_x(x), m_y(y) {}

Point Point::operator+(const Point &other) const {
    Point tmp = *this;
    tmp.m_x += other.m_x;
    tmp.m_y += other.m_y;
    return tmp;
}

Point Point::operator+=(const Point &other) {
    m_x += other.m_x;
    m_y += other.m_y;
    return *this;
}

bool Point::operator==(const Point &other) const {
    return tie(m_x, m_y) == tie(other.m_x, other.m_y);
}

bool Point::operator<(const Point &other) const {
    return tie(m_x, m_y) < tie(other.m_x, other.m_y);
}

bool Point::in_positive_range(dimension x, dimension y) {
    return ((m_x >= 0) && (m_y >= 0) && (m_x < x) && (m_y < y));
}
