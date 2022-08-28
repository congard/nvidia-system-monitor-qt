#ifndef QNVSM_POINT_H
#define QNVSM_POINT_H

template<typename T = float>
class Point {
public:
    Point(T x, T y): x(x), y(y) {}

public:
    T x; // [0; 1]
    T y; // [0; 1]
};

using PointF = Point<float>;
using PointI = Point<int>;

#endif //QNVSM_POINT_H
