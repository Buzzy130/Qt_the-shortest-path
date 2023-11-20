#ifndef STRUCTS_H
#define STRUCTS_H
#pragma once
#include <set>
#include <utility>
#include <algorithm>

struct vertex {
    int x, y;

    vertex() {};

    vertex(int x1, int y1) {
        x = x1;
        y = y1;
    }

    bool operator == (vertex const& u) const {
        return (this->x == u.x && this->y == u.y);
    }
};



struct segment {
    vertex v1, v2, v3, v4;

    segment(int x1, int y1, int x2, int y2) {
        v1 = { std::min(x1, x2), std::min(y1, y2) };
        v2 = { std::min(x1, x2), std::max(y1, y2) };
        v3 = { std::max(x1, x2), std::min(y1, y2) };
        v4 = { std::max(x1, x2), std::max(y1, y2) };
    }
};

inline
const bool operator < (vertex const& v, vertex const& u) {
    if (v.x == u.x) {
        return (v.y < u.y);
    }
    else {
        return (v.x < u.x);
    }
}

inline
const bool operator < (segment const& s, segment const& s1) {
    if (s1.v1.x == s.v1.x) {
        return (s.v1.y < s1.v1.y);
    }
    else {
        return (s.v1.x < s1.v1.x);
    }
}
#endif // STRUCTS_H
