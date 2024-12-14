#include <bits/stdc++.h>
using namespace std;

const long double eps = 1e-9, inf = 1e9;
struct point{
    long double x, y;
    point(long double x = 0, long double y = 0) : x(x), y(y) {}
    point operator-(const point& p) const  { return point(x - p.x, y - p.y); }
    point operator+(const point& p) const  { return point(x + p.x, y + p.y); }
    point operator*(long double t)  const { return point(x * t, y * t); }
};

long double dot(const point& a, const point& b) { return a.x * b.x + a.y * b.y; }
long double cross(const point& a, const point& b) { return a.x * b.y - a.y * b.x; }

struct halfplane{
    point p, pq;
    long double angle;
    halfplane() {}
    halfplane(const point& a, const point& b) : p(a), pq(b-a) { angle = atan2l(pq.y , pq.x); }
    bool out(const point& r) { return cross(pq, r - p) < -eps; }
    bool operator<(const halfplane hp) const { return angle < hp.angle; }
};

point inter(const halfplane& hp1, const halfplane &hp2){
    long double alpha = cross((hp2.p - hp1.p), hp2.pq) / cross(hp1.pq, hp2.pq);
    return hp1.p + (hp2.pq * alpha);
}

vector<point> hp_intersect(vector<halfplane> &H){
    point box[4] = {
        point{inf, inf},
        point{-inf, inf},
        point{-inf, -inf},
        point{inf, -inf},
    };
    for(int i = 0; i < 4; i++){
        halfplane aux(box[i], box[(i + 1) % 4]);
        H.push_back(aux);
    }

    sort(H.begin(), H.end());
    deque<halfplane> dq;
    int len = 0;
    for(int i = 0; i < int(H.size()); i++){
        while(len > 1 && H[i].out(inter(dq[len-1], dq[len-2]))) {dq.pop_back(); --len; }
        while(len > 1 && H[i].out(inter(dq[0], dq[1]))) {dq.pop_front(); --len; }
        if(len > 0 && fabsl(cross(H[i].pq, dq[len-1].pq)) < eps) {
            if(dot(H[i].pq, dq[len-1].pq) < 0.0) return vector<point>{};
            if(H[i].out(dq[len-1].p)) {dq.pop_back(); --len;}
            else continue; 
        }
        dq.push_back(H[i]); ++len;
    }
    while(len > 2 && dq[0].out(inter(dq[len-1], dq[len-2]))) { dq.pop_back(), --len; }
    while(len > 2 && dq[len-1].out(inter(dq[0], dq[1]))) { dq.pop_front(), --len; }
    if(len < 3) return vector<point>{};
    vector<point> ret(len);
    for(int i = 0; i + 1 < len; i++) { ret[i] = inter(dq[i], dq[i+1]);}
    ret.back() = inter(dq[len-1], dq[0]);
    return ret;
}