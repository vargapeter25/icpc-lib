#include <bits/stdc++.h>
using namespace std;

const long long INF = 2e18 + 10; // Elég nagynak kell lennie
struct line{ // a * x + b | a, b: kezdetben a legrosszabb egyenes
    mutable long long a = 0, b = INF;
    mutable long double lef = -2e18; bool point = false; // csak a set cht-hoz
    long long get(long long x) const { return a * x + b; }
    long double intersect(const line& e) const { return (long double)(e.b - b) / (a - e.a); }
    bool bad() const { return b == INF; } // ellenorzi, hogy az egyenes mindennél rosszabb-e (nincsen)
};

struct li_chao{ // update(line) hozzáad egy egyenest, query(x) x-helyen lévo minimum y értéket adja vissza
    struct node{ // Elore foglalt memóriával gyorsabb
        line e;
        node *l = NULL, *r = NULL;
    } *root;
    long long L, R;
    li_chao(long long L, long long R) : root(new node()), L(L), R(R) {}
    void update(node* &p, long long l, long long r, line e){
        if(e.bad()) return;
        if(!p) p = new node();
        int m = (l + r) / 2;
        bool lef = e.get(L) < p->e.get(L);
        bool mid = e.get(m) < p->e.get(m);
        if(mid) swap(e, p->e);
        if(r - l == 1) return;
        else if(lef != mid) update(p->l, l, m, e);
        else update(p->r, m, r, e);
    }
    void update(line e) { update(root, L, R, e); } 
    long long query(node *p, long long l, long long r, long long x) {
        if(!p) return INF;
        int m = (l + r) / 2;
        if(x < m) return min(p->e.get(x), query(p->l, l, m, x));
        return min(p->e.get(x), query(p->r, m, r, x));
    }
    long long query(long long x) { return query(root, L, R, x); }
};

struct CHT{ // set cht, valosakhoz is jo
    struct comp{ bool operator()(const line& e1, const line& e2) const { return !e1.point && !e2.point ? e1.a > e2.a : e1.lef < e2.lef; } };
    set<line, comp> lines;
    static inline bool check(const line& a, const line& b, const line& c) { return a.intersect(c) < a.intersect(b); }
    void update(const line& e){
        auto it = lines.insert(e).first;
        if(it->b < e.b) return;
        it->b = e.b;
        auto prv = it == lines.begin() ? lines.end() : prev(it);
        auto nxt = next(it);
        if(prv != lines.end() && nxt != lines.end() && check(*prv, *it, *nxt)) {
            lines.erase(it);
            return;
        }
        while(prv != lines.end() && prv != lines.begin()){
            auto prv2 = prev(prv);
            if(check(*prv2, *prv, *it)){
                lines.erase(prv);
                prv = prv2;
            } else {
                break;
            }
        }
        while(nxt != lines.end() && next(nxt) != lines.end()){
            auto nxt2 = next(nxt);
            if(check(*it, *nxt, *nxt2)) {
                lines.erase(nxt);
                nxt = nxt2;
            } else{
                break;
            }
        }
        if(prv != lines.end()) it->lef = prv->intersect(*it);
        if(nxt != lines.end()) nxt->lef = it->intersect(*nxt);
    }
    long long query(long long x){
        line tmp;
        tmp.lef = x;
        tmp.point = true;
        auto it = lines.upper_bound(tmp);
        assert(it != lines.begin());
        return prev(it)->get(x);
    }
};
