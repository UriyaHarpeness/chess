#pragma once

#include "../point/Point.h"

#include <vector>

using namespace std;

/**
 * A class for storing two dimensional data.
 *
 * @tparam T    Type of data stored in the two dimensional vector.
 */
template<typename T>
class TwoDimensionalVector {
public:

    /**
     * Constructor.
     *
     * Initialize a two dimensional, fixed size vector (of vectors), with empty data.
     *
     * @param x Size of the first dimension.
     * @param y Size of the second dimension.
     */
    TwoDimensionalVector(size_t x, size_t y);

    /**
     * Subscript operator.
     *
     * @param point The index of the element to get.
     * @return  The element in the given index.
     */
    T &operator[](const Point &point);

    /**
     * Subscript operator.
     *
     * @param point The index of the element to get.
     * @return  The element in the given index.
     */
    const T &operator[](const Point &point) const;

    /**
     * Check if an index is inside the data's boundaries.
     *
     * @param point The index to check.
     * @return  Is the index inside the boundaries.
     */
    [[nodiscard]] bool contains(const Point &point) const;


private:

    /// The two dimensional data.
    vector<vector<T>> m_data;
};

template<typename T>
TwoDimensionalVector<T>::TwoDimensionalVector(size_t x, size_t y) : m_data(x, vector<T>(y, nullptr)) {}

template<typename T>
T &TwoDimensionalVector<T>::operator[](const Point &point) {
    return m_data[point.get_x()][point.get_y()];
}

template<typename T>
const T &TwoDimensionalVector<T>::operator[](const Point &point) const {
    return m_data[point.get_x()][point.get_y()];
}

template<typename T>
bool TwoDimensionalVector<T>::contains(const Point &point) const {
    return ((point.get_x() >= 0) && (point.get_y() >= 0) && (point.get_x() < m_data.size()) &&
            (point.get_y() < m_data[0].size()));
}
