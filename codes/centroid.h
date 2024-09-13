#include <bits/stdc++.h>
using namespace std;

struct node{
    vector<int> to;
    vector<pair<int, int>> p; // csak ha kell
    int sz = 0;
    bool vis = false;
}; 
vector<node> g;
int get_sz(int x, int p = -1){
    g[x].sz = 1;
    for(int y : g[x].to) if(y != p && !g[y].vis) g[x].sz += get_sz(y, x);
    return g[x].sz;
}
pair<int, int> get_c(int x, int n, int p = -1) { // elotte: get_sz
    for(int y : g[x].to) if(y != p && !g[y].vis && g[y].sz * 2 >= n) return g[y].sz * 2 == n ? make_pair(x, y) : get_c(y, n, x);
    return make_pair(x, x);
}
void dfs_sub(int x, int c, int d = 0, int p = -1){
    g[x].p.emplace_back(c, d);
    for(int y : g[x].to) if(y != p && !g[y].vis) dfs_sub(y, c, d + 1, x);
}
void centroid_decomp(int c){
    int sz = get_sz(c);
    c = get_c(c, sz).first;
    g[c].vis = true;
    dfs_sub(c, c); // centroid szülok távolsága a csúcstól (önmagát is beleértve) | a sz itt már nem jó újra kell számolni
    // calc
    for(int y : g[c].to) if(!g[y].vis) centroid_decomp(y);
}