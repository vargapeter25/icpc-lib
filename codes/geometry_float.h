/*
source of some codes: https://cp-algorithms.com/
*/

#include <bits/stdc++.h>
using namespace std;

const long double EPS = 1e-9; 

struct point{
    long double x, y;
    point operator+(const point& p) const { return point{x + p.x, y + p.y}; }
    point operator-(const point& p) const { return point{x - p.x, y - p.y}; }
    point operator*(long double t) const { return point{x * t, y * t}; }
    long double len() const { return hypot(x, y); }
    point normalized() const { return (*this) * (1.0 / len()); }
    bool operator<(const point& p) const { return x < p.x - EPS || (abs(x - p.x) < EPS && y < p.y - EPS); }
};

inline long double dot(const point& a, const point& b) { return a.x * b.x + a.y * b.y; }
inline long double cross(const point& a, const point& b) { return a.x * b.y - a.y * b.x; }
inline long double det(long double a, long double b, long double c, long double d) { return a * c - b * d; }
inline long double sqr(long double x) { return x*x; }
inline int sgn(auto x) { return (x > 0 ) - (x < 0); }
inline int dir(const point& a, const point& b, const point& c) { return sgn(cross(b - a, c - a)); }

struct line{ // a * x + b * y + c = 0, normalizáltnak kell lennie
    long double a, b, c;
    line(long double a_, long double b_, long double c_) : a(a_), b(b_), c(c_) {
        long double len = hypot(a, b);
        if(len > EPS) a /= len, b /= len, c /= len;
    }
    line(const point& p1, const point& p2) {
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        c = -a * p1.x - b * p1.y;
        long double len = hypot(a, b);
        if(len > EPS) a /= len, b /= len, c /= len;
    }

    long double dist(const point& p) { return a * p.x + b * p.y + c; }
};

inline bool paralell(const line& l1, const line& l2) { return abs(det(l1.a, l1.b, l2.a, l2.b)) < EPS; }

inline bool equivalent(const line& l1, const line& l2) {
    return abs(det(l1.a, l1.b, l2.a, l2.b)) < EPS
        && abs(det(l1.a, l1.c, l2.a, l2.c)) < EPS
        && abs(det(l1.b, l1.c, l2.b, l2.c)) < EPS;
}

inline bool intersect1(long double a1, long double a2, long double b1, long double b2){
    return max(min(a1, a2), min(b1, b2)) <= min(max(a1, a2), max(b1, b2)) + EPS;
}

inline bool betw(double l, double r, double x) {
    return min(l, r) <= x + EPS && x <= max(l, r) + EPS;
}

bool intersect(const line& l1, const line& l2, point& res) {
    long double zn = det(l1.a, l1.b, l2.a, l2.b);
    if (abs(zn) < EPS) return false; // párhuzamos
    res.x = -det(l1.c, l1.b, l2.c, l2.b) / zn;
    res.y = -det(l1.a, l1.c, l2.a, l2.c) / zn;
    return true;
}

bool intersect(point a, point b, point c, point d, point& left, point& right) { // ellenőrzi a metszést, metszés esetén a [left, right] szakasz a metszet
    if (!intersect1(a.x, b.x, c.x, d.x) || !intersect1(a.y, b.y, c.y, d.y))
        return false;
    line m(a, b);
    line n(c, d);
    long double zn = det(m.a, m.b, n.a, n.b);
    if (abs(zn) < EPS) {
        if (abs(m.dist(c)) > EPS || abs(n.dist(a)) > EPS)
            return false;
        if (b < a)
            swap(a, b);
        if (d < c)
            swap(c, d);
        left = max(a, c);
        right = min(b, d);
        return true;
    } else {
        left.x = right.x = -det(m.c, m.b, n.c, n.b) / zn;
        left.y = right.y = -det(m.a, m.c, n.a, n.c) / zn;
        return betw(a.x, b.x, left.x) && betw(a.y, b.y, left.y) &&
               betw(c.x, d.x, left.x) && betw(c.y, d.y, left.y);
    }
}

struct circle{
    point p;
    long double r;
};

vector<point> intersection(const circle& circ, const line& l){ // kör-egyenes metszéspontok
    long double r = circ.r, a = l.a, b = l.b, c = l.c - l.a * circ.p.x - l.b * circ.p.y;
    double x0 = -a*c/(a*a+b*b), y0 = -b*c/(a*a+b*b);
    if (c*c > r*r*(a*a+b*b)+EPS) return {};
    if (abs (c*c - r*r*(a*a+b*b)) < EPS) return {point{x0, y0} + circ.p};
    double d = r*r - c*c/(a*a+b*b);
    double mult = sqrt (d / (a*a+b*b));
    double ax, ay, bx, by;
    ax = x0 + b * mult;
    bx = x0 - b * mult;
    ay = y0 - a * mult;
    by = y0 + a * mult;
    return {point{ax, ay} + circ.p, point{bx, by} + circ.p};
}

vector<point> intersection(circle circ1, circle circ2){ // kör-kör metszéspontok
    point origo = circ1.p;
    circ2.p = circ2.p - origo;
    circ1.p = {0, 0};
    line l(-2 * circ2.p.x, -2 * circ2.p.y, sqr(circ2.p.x) + sqr(circ2.p.y) + sqr(circ1.r) - sqr(circ2.r));
    auto tmp = intersection(circ1, l);
    for(auto &p : tmp) p = p + origo;
    return tmp;
}

void tangents (point c, double r1, double r2, vector<line> & ans) {
    double r = r2 - r1;
    double z = sqr(c.x) + sqr(c.y);
    double d = z - sqr(r);
    if (d < -EPS)  return;
    d = sqrt (abs (d));
    line l(0, 0, 0);
    l.a = (c.x * r + c.y * d) / z;
    l.b = (c.y * r - c.x * d) / z;
    l.c = r1;
    ans.push_back (l);
}

vector<line> tangents (circle a, circle b) { // 2 kör közös érintői
    vector<line> ans;
    for (int i=-1; i<=1; i+=2)
        for (int j=-1; j<=1; j+=2)
            tangents (b.p-a.p, a.r*i, b.r*j, ans);
    for (size_t i=0; i<ans.size(); ++i)
        ans[i].c -= ans[i].a * a.p.x + ans[i].b * a.p.y;
    return ans;
}