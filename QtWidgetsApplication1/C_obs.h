#ifndef C_OBS_H
#define C_OBS_H
#include "structs.h"

class C_obs {
private:
    std::set<segment> obs;

public:
    inline void add_obstacle(const segment& s, const int r);
    inline bool is_visible(int r, const vertex& v, const vertex& u) const;
    inline std::set<vertex> get_vertices() const;
    inline std::set<vertex> get_visible_vertices(int r, const vertex& v, const vertex& goal) const;
};

inline
void C_obs::add_obstacle(const segment& s, const int r) {
    int x1 = s.v1.x - r;
    int y1 = s.v1.y - r;
    int x2 = s.v4.x + r;
    int y2 = s.v4.y + r;



    segment s_real(x1, y1, x2, y2);

    obs.insert(s_real);
}

inline
std::set<vertex> C_obs::get_vertices() const {
    std::set<vertex> vertices;
    for (auto s : obs) {
        vertices.insert(s.v1);
        vertices.insert(s.v2);
        vertices.insert(s.v3);
        vertices.insert(s.v4);
    }
    return vertices;
}

inline
double dot(vertex A, vertex B, vertex C, vertex D) {
    vertex AB(B.x - A.x, B.y - A.y);
    vertex CD(D.x - C.x, D.y - C.y);

    return AB.x * CD.x + AB.y * CD.y;
}

inline
double cross(vertex A, vertex B, vertex C, vertex D) {
    vertex AB(B.x - A.x, B.y - A.y);    //radius vector AB
    vertex CD(D.x - C.x, D.y - C.y);	//rad vector CD

    return AB.x * CD.y - CD.x * AB.y;
}

inline
bool intersect(vertex A, vertex B, vertex C, vertex D) {
    if (A == C && B == D) {
        return false;
    }
    auto denom = cross(A, B, C, D);
    auto alpha = cross(A, C, A, B);
    auto beta = cross(A, C, C, D);
    if (denom == 0) {
        if (alpha != 0)
            return false;	//parallel lines

        auto t0 = dot(A, C, A, B) / dot(A, B, A, B);
        auto t1 = t0 + dot(C, D, A, B) / dot(A, B, A, B);
        if (t0 > t1) {
            auto tmp = t0;
            t0 = t1;
            t1 = tmp;
        }
        if (t0 < 1.0 || t1 > 0) {
            return true;
        }

    }
    double ad = alpha / denom,
        bd = beta / denom;

    if (0 < ad && ad < 1 && 0 < bd && bd < 1) {
        return true;
    }

    return false;
}

inline
bool C_obs::is_visible(int r, const vertex& v, const vertex& u) const {
    for (auto s : obs) {
        if (s.v1.x < u.x && u.x < s.v4.x && s.v1.y < u.y && u.y < s.v4.y) {
            return false;
        }
        if (s.v1.x < v.x && v.x < s.v4.x && s.v1.y < v.y && v.y < s.v4.y) {
            return false;
        }
        if (640 - r <= u.x || u.x <= 0 + r || 380 - r <= u.y || u.y <= 0 + r) {
            return false;
        }

        if (640 - r <= v.x || v.x <= 0 + r || 380 - r <= v.y || v.y <= 0 + r) {
            return false;
        }

        if (intersect(s.v1, s.v2, v, u) || intersect(s.v2, s.v3, v, u) ||
            intersect(s.v3, s.v4, v, u) || intersect(s.v4, s.v1, v, u) ||
            intersect(s.v1, s.v4, v, u) || intersect(s.v2, s.v3, v, u)) {
            return false;
        }
    }
    return true;
}

inline
std::set<vertex> C_obs::get_visible_vertices(int r, const vertex& v, const vertex& goal) const {
    std::set<vertex> visible_vertices = {};
    std::set<vertex> vertices = this->get_vertices();
    vertices.insert(goal);

    for (auto u : vertices) {
        if (is_visible(r, v, u)) {
            visible_vertices.insert(u);
        }
    }

    return visible_vertices;
}
#endif // C_OBS_H
