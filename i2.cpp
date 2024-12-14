#include <bits/stdc++.h>
using namespace std;

struct inter{
    mutable int l, r, m, val;
    int get(int x) const { return val + abs(x - m); }
    bool operator<(const inter& i) const { return l < i.l; }
};

set<inter> s;

inline bool contains(const inter& a, const inter& b){
    return a.get(b.m) <= b.val;
}

inline int split(const inter& a, const inter& b){ //a.r < b.l
    if(contains(a, b)) return b.r;
    if(contains(b, a)) return a.l - 1;
    return (a.m + (b.val - a.val) + b.m) / 2;
}

void set_val(int x, int val){
    // cout << "set: " << x << ' ' << val << '\n';
    auto cont = prev(s.upper_bound(inter{x}));
    inter l = {cont->l, x - 1, min(cont->m, x - 1)};
    l.val = cont->get(l.m);
    inter r = {x + 1, cont->r, max(cont->m, x + 1)};
    r.val = cont->get(r.m);
    // cout << "add: " << l.l << ' ' << l.r << ' ' << l.m << ' ' << l.val << '\n';
    // cout << "add: " << r.l << ' ' << r.r << ' ' << r.m << ' ' << r.val << '\n';
    s.erase(cont);
    if(l.l <= l.r) s.insert(l);
    if(r.l <= r.r) s.insert(r);
    auto it = s.insert(inter{x, x, x, val}).first;
    while(it != s.begin()) {
        int nx = split(*prev(it), *it);
        if(nx >= it->r) {
            // cout << "del from left\n";
            it--;
            it->r = next(it)->r;
            s.erase(next(it));
            break;
        } else if(nx < prev(it)->l){
            int tmp = prev(it)->l;
            s.erase(prev(it));
            it->l = tmp;
        } else{
            prev(it)->r = nx;
            it->l = nx+1;
            break;
        }
    }
    while(next(it) != s.end()) {
        int nx = split(*it, *next(it));
        if(nx < it->l){
            it++;
            int tmp = prev(it)->l;
            s.erase(prev(it));
            it->l = tmp;
        } else if(next(it)->r <= nx){
            it->r = next(it)->r;
            s.erase(next(it));
        } else{
            next(it)->l = nx + 1;
            it->r = nx;
            break;
        }
    }
}

int get_val(int x, int n){
    auto it = prev(s.upper_bound(inter{x}));
    return min({it->get(x), s.begin()->get(x - n), s.rbegin()->get(x + n)});
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m, start;
    cin>>n>>m>>start;

    s.insert(inter{0, n + 1, start, 0});

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
        set_val(t1, v2);
        // cout << "debug\n";
        // for(auto i : s) cout << i.l << ' ' << i.r << " | " << i.m << " | " << i.val << '\n';
        set_val(t2, v1);
        // cout << "debug\n";
        // for(auto i : s) cout << i.l << ' ' << i.r << " | " << i.m << " | " << i.val << '\n';
    }


    for(int i = 1; i <= n; i++) cout << get_val(i, n) << '\n';

    return 0;
}