#include <bits/stdc++.h>
using namespace std;

struct point{
    long long x, y;
    point operator+(const point& p) const { return {x + p.x, y + p.y}; }
    point operator-(const point& p) const { return {x - p.x, y - p.y}; }
    point operator*(long long t) const { return {x * t, y * t}; }
    bool operator==(const point& p) const { return x == p.x && y == p.y; }
    long long len() const { return x * x + y * y; }

    void print() { cout << "(" << x << ", " << y << ")"; }
};

void print(const vector<point>& v) {
    for(point p : v) {
        p.print();
        cout << ' ';
    }
    cout << '\n';
}

inline long long cross(const point& a, const point& b) {
    return a.x * b.y - b.x * a.y;
}

inline int sgn(long long x) { return (x > 0) - (x < 0); }

inline int dir(const point& a, const point& b, const point& c) {
    return sgn(cross(b - a, c - a));
}

struct adat{
    vector<point> hull1, hull2;
    bool operator<(const adat& d) const {
        return hull1.size() + hull2.size() > d.hull1.size() + d.hull2.size();
    }
};

vector<vector<int>> g;
vector<adat> hulls;

inline int next(const vector<point>& v, int i) { return i == (int)v.size() - 1 ? 0 : i + 1; }
inline int prev(const vector<point>& v, int i) { return i == 0 ? (int)v.size() - 1 : i - 1; }

bool comp_args(const point& a, const point& b){
    bool fst_a = a.y > 0 || (a.y == 0 && a.x >= 0); 
    bool fst_b = b.y > 0 || (b.y == 0 && b.x >= 0);
    if(fst_a != fst_b) return fst_a;
    long long c = cross(a, b);
    return c != 0 ? c > 0 : a.len() < b.len();
}

vector<point> minkowski_sum(vector<point> a, vector<point> b){
    if(a.empty() || b.empty()) return {};
    auto comp = [](const point& a, const point& b) { return a.y < b.y || (a.y == b.y && a.x < b.x); };
    int min_a = min_element(a.begin(), a.end(), comp) - a.begin();
    int min_b = min_element(b.begin(), b.end(), comp) - b.begin();
    rotate(a.begin(), a.begin() + min_a, a.end());
    rotate(b.begin(), b.begin() + min_b, b.end());
    a.push_back(a[0]);
    a.push_back(a[1]);
    b.push_back(b[0]);
    b.push_back(b[1]);
    vector<point> hull;
    int i = 0, j = 0;
    while(i < a.size() - 2 || j < b.size() - 2) {
        hull.push_back(a[i] + b[j]);
        long long c = cross(a[i + 1] - a[i], b[j + 1] - b[j]);
        if(c >= 0 && i < a.size() - 2)
            ++i;
        if(c <= 0 && j < b.size() - 2)
            ++j;
    }
    return hull;
}

vector<point> convex_hull(vector<point> a){
    if(a.empty()) return {};
    int pos = min_element(a.begin(), a.end(), [](const point& a, const point& b) { return a.x < b.x || (a.x == b.x && a.y < b.y); }) - a.begin();
    swap(a[0], a[pos]);
    sort(a.begin() + 1, a.end(), [o = a[0]](const point& a, const point& b) { int d = dir(o, a, b); return d == 1 || (d == 0 && (a-o).len() < (b-o).len()); });
    vector<point> hull;
    for(const point &p : a){
        while(hull.size() > 1 && dir(hull[hull.size() - 2], hull[hull.size() - 1], p) != 1) hull.pop_back();
        hull.push_back(p);
    }
    int j = (int)hull.size() - 2;
    while(j > 0 && dir(hull[j], hull[j+1], hull[0]) != 1) {
        hull.pop_back();
        j--;
    }

    if(hull.size() == 2 && hull[0] == hull[1]) hull.pop_back();

    return hull;
}

vector<point> merge_convex(vector<point> a, vector<point> b){
    a.insert(a.end(), b.begin(), b.end());
    return convex_hull(a);
}

void dfs(int x){
    priority_queue<adat> pq;
    vector<point> ps;
    for(int y : g[x]){
        dfs(y);
        pq.push(adat{hulls[y]});
    }
    if(pq.empty()) return;
    while(pq.size() > 1){
        adat d1 = pq.top();
        pq.pop();
        adat d2 = pq.top();
        pq.pop();

        adat nd;
        nd.hull1 = minkowski_sum(d1.hull1, d2.hull1);
        nd.hull2 = merge_convex(minkowski_sum(d1.hull2, d2.hull1), minkowski_sum(d1.hull1, d2.hull2));
        pq.push(nd);
    }
    hulls[x] = pq.top();

    hulls[x].hull1 = hulls[x].hull2;
    for(point &p : hulls[x].hull1) p = p * (-1);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin>>n;

    g.resize(n);
    hulls.resize(n);
    for(int i = 0; i < n; i++){
        int k;
        cin>>k;
        if(k == 0){
            point p;
            cin>>p.x>>p.y;
            hulls[i] = adat{vector<point>{p * (-1)}, vector<point>{p}};
            continue;
        }
        for(int j = 0; j < k; j++){
            int a;
            cin>>a;
            --a;
            g[i].push_back(a);
        }
    }

    dfs(0);

    long long ans = 0;
    for(const point& p : hulls[0].hull1){
        ans = max(ans, p.x * p.x + p.y * p.y);
    }

    cout << ans << '\n';

    return 0;
}