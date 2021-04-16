#pragma once

#include <tuple>

using namespace std;

/// Define dimension as a short int,
typedef short int dimension;

class Point {
public:

    /**
     * Constructor.
     */
    Point() : m_x(0), m_y(0) {}

    /**
     * Copy constructor.
     *
     * @param other The point to copy.
     */
    Point(const Point &other) = default;

    /**
     * Constructor.
     *
     * @param x X axis value.
     * @param y Y axis value.
     */
    Point(dimension x, dimension y);

    /**
     * Constructor.
     *
     * @param point The X and Y axis values.
     */
    Point(pair<dimension, dimension> point);

    /**
     * Addition operator.
     *
     * @param other The point to add.
     * @return  The sum of the points.
     */
    Point operator+(const Point &other) const;

    /**
     * Addition assignment operator.
     *
     * @param other The point to add.
     * @return  The sum of the points.
     */
    Point operator+=(const Point &other);

    /**
     * Equal operator.
     *
     * @param other The point to compare with.
     * @return  Are the points equal.
     */
    bool operator==(const Point &other) const;

    /**
     * Less than operator.
     *
     * @param other The point to compare with.
     * @return  Is the point less than the given point.
     */
    bool operator<(const Point &other) const;

    /**
     * Get the point's X axis value.
     *
     * @return  The point's X axis value.
     */
    [[nodiscard]] dimension get_x() const { return m_x; }

    /**
     * Get the point's Y axis value.
     *
     * @return  The point's Y axis value.
     */
    [[nodiscard]] dimension get_y() const { return m_y; }


private:

    /// The X axis value.
    dimension m_x;

    /// The Y axis value.
    dimension m_y;
};
