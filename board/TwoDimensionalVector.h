#pragma once

#include "../point/Point.h"

#include <vector>

using namespace std;

template<typename T>
class TwoDimensionalVector {
public:
    TwoDimensionalVector(size_t x, size_t y);

    T &operator[](const Point &point);

    const T &operator[](const Point &point) const;

    [[nodiscard]] bool contains(const Point &point) const;

private:
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
