#include <bits/stdc++.h>
using namespace std;

struct inter{
    mutable int x, y;
    int get(int p) const { return y + abs(x - p); }
    bool operator<(const inter& i) const { return x < i.x; }
};

set<inter> s;

inline bool contains(const inter& a, const inter& b) { 
    // cout << "contains: " << a.x << ' ' << a.y << " | " << b.x << ' ' << b.y << '\n';
    return a.get(b.x) <= b.y; 
}

void add(const inter& a){
    // cout << "add: " << a.x << ' ' << a.y << '\n';
    auto it = s.insert(a).first;
    it->y = min(it->y, a.y);
    if((it != s.begin() && contains(*prev(it), *it)) || (next(it) != s.end() && contains(*next(it), *it))){
        s.erase(it);
        return;
    }
    // cout << "begin: " << (it == s.begin()) << '\n';
    while(it != s.begin() && contains(*it, *prev(it))) s.erase(prev(it));
    while(next(it) != s.end() && contains(*it, *next(it))) s.erase(next(it));
}

bool rem(int x){
    auto it = s.lower_bound(inter{x, 0});
    if(it != s.end() && it->x == x) {
        s.erase(it);
        return true;
    }
    return false;
}

int get_val(int x, int n){
    assert(!s.empty());
    auto it = s.lower_bound(inter{x, 0});
    int res = it == s.end() ? (int)1e9 : it->get(x);
    if(it != s.begin()) res = min(res, prev(it)->get(x));
    return min({res, s.begin()->get(x - n), s.rbegin()->get(x + n)});
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m, start;
    cin>>n>>m>>start;

    add(inter{start, 0});

    vector<pair<int, int>> edges(m);
    for(auto &[d, t] : edges) {
        cin>>d>>t;
    }

    sort(edges.rbegin(), edges.rend());

    for(auto [d, t1] : edges){
        int t2 = t1 + 1;
        if(t2 == n+1) t2 = 1; 

        int v1 = get_val(t1, n);
        int v2 = get_val(t2, n);
        rem(t1);
        rem(t2);
        add(inter{t1 > 1 ? t1-1 : n, v1 + 1});
        add(inter{t1 < n ? t1+1 : 1, v1 + 1});
        add(inter{t1, v2});

        add(inter{t2 > 1 ? t2-1 : n, v2 + 1});
        add(inter{t2 < n ? t2+1 : 1, v2 + 1});
        add(inter{t2, v1});

        // cout << "debug:\n";
        // for(auto i : s) cout << i.x << ' ' << i.y << '\n';
    }


    for(int i = 1; i <= n; i++) cout << get_val(i, n) << '\n';

    return 0;
}