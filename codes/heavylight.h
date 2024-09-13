#include <bits/stdc++.h>
using namespace std;
struct node{
    vector<int> to;
    int l, r, i, p, hld_p, sz, d; // l: st bal, r: st jobb, i: st idx, p: os, hld_p: light edge elotti os, sz: részfa mérete, d: gyökértol vett távolság
    // heavy út: [l, r], részfa: [i, i + sz), !!! szegmensfában g[x].i-t kell használni
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