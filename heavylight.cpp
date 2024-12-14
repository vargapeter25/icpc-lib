#include <bits/stdc++.h>
using namespace std;

const long long mod = 998244353;

struct fenwick_tree{
    vector<long long> v;
    fenwick_tree(int n) : v(n + 10, 0) {}
    void update(int x, long long c){
        for(int i = x + 1; i < (int)v.size(); i += (i & (-i))) v[i] = (v[i] + c) % mod;
    }
    void update(int l, int r, long long c){
        update(l, c);
        update(r, -c);
    }
    long long query(int x){
        long long res = 0;
        for(int i = x + 1; i > 0; i -= (i & (-i))) res = (res + v[i]) % mod;
        return res;
    }
    long long query(int l, int r){ // inclusive
        return (mod + query(r) - query(l - 1)) % mod;
    }
};

long long mod_pow(long long n, long long h){
    long long res = 1;
    while(h){
        if(h&1LL) res = res * n % mod;
        n = n * n % mod;
        h >>= 1;
    }
    return res;
}

struct node{
    vector<int> to;
    int l, r, i, p, hld_p, sz, d; // l: st bal, r: st jobb, i: st idx, p: os, hld_p: light edge elotti os, sz: részfa mérete, d: gyökértol vett távolság
    // heavy út: [l, r], részfa: [i, i + sz), !!! szegmensfában g[x].i-t kell használni
    int sz_up = 0;
};
vector<node> g;
int dfs_sz(int x, int d = 0, int p = -1){
    g[x].sz = 1;
    g[x].d = d;
    for(int y : g[x].to) if(y != p) g[x].sz += dfs_sz(y, d + 1, x);
    return g[x].sz;
}
int IDX = 0; // reset
int dfs_hld(int x, int hld_p, int p = -1){ // x = hld_p = root
    g[x].i = g[x].r = IDX++;
    g[x].l = g[hld_p].i;
    g[x].p = p;
    g[x].hld_p = hld_p;
    sort(g[x].to.begin(), g[x].to.end(), [](int i, int j){ return g[i].sz > g[j].sz; });
    bool fst = true;
    for(int y : g[x].to){
        if(y == p) continue;
        if(fst) { g[x].r = dfs_hld(y, hld_p, x); fst = false; }
        else dfs_hld(y, y, x);
    }
    return g[x].r;
}
void build_hld(int root) { dfs_sz(root); IDX = 0; dfs_hld(root, root); }

const int maxn = 150000;
fenwick_tree ft_up(maxn), ft_down(maxn), ft_simple(maxn);

void update(int x, int d){
    ft_up.update(g[x].i, (long long)g[x].sz_up * d % mod);
    ft_down.update(g[x].i, (long long)g[x].sz * d % mod);
    ft_simple.update(g[x].i, d);
}

long long query(int x, int n){
    long long res = ft_down.query(n - 1); // all
    res = (res + ft_down.query(g[x].i, g[x].i) * (mod - 1 + mod_pow(g[x].sz, mod - 2) * n % mod) % mod) % mod;
    // cout << "node: " << x << '\n';
    // cout << ft_down.query(g[x].i, g[x].i) << ' ' << ft_down.query(g[x].i, g[x].i) << '\n';
    // res = (res + ft_simple.query(g[x].i, g[x].i + g[x].sz - 1) * (n - g[x].sz + 1) % mod + mod - ft_simple.query(g[x].i, g[x].i)) % mod;
    // res = (res + mod - ft_down.query(g[x].i, g[x].i + g[x].sz - 1)) % mod;

    while(x != -1){
        res = (res + ft_up.query(g[x].l, g[x].i - 1) + (mod - ft_down.query(g[x].l, g[x].i - 1))) % mod;
        x = g[x].hld_p;
        if(g[x].p == -1) break;
        long long tmp = ft_down.query(g[g[x].p].i, g[g[x].p].i);
        tmp = (((tmp * mod_pow(g[g[x].p].sz, mod - 2)) % mod) * (n - g[x].sz) + mod - tmp) % mod;
        res = (res + tmp) % mod;
        x = g[x].p;
    }
    return res;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin>>n>>q;

    g.assign(n, node());
    for(int i = 0; i < n - 1; i++){
        int a, b;
        cin>>a>>b;
        --a, --b;
        g[a].to.push_back(b);
        g[b].to.push_back(a);
    }

    build_hld(0);

    for(int i = 0; i < n; i++){
        if(g[i].hld_p != i){
            g[g[i].p].sz_up = n - g[i].sz;
        }
    }

    // for(int i = 0; i < n; i++) cout << i << ": " << g[i].p << '\n';

    long long inv_n = mod_pow(n, mod - 2);
    while(q--){
        int t;
        cin>>t;
        if(t == 1){
            int x, d;
            cin>>x>>d;
            --x;
            update(x, d);
        } else {
            int x;
            cin>>x;
            --x;
            cout << (query(x, n) * inv_n) % mod << '\n';
        }
    }

    return 0;
}