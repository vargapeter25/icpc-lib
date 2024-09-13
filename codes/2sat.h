#include <bits/stdc++.h>

using namespace std;

const int c=1000005;
int n, m, compdb, comp[c], res[c];
vector<int> el[c], el2[c];
bool vis[c], vis2[c];
vector<int> sor;
int par(int a) {
    return (a<=n ? a+n : a-n);
}
void add(int a, int b) {
    // a->b;
    el[a].push_back(b);
    el2[b].push_back(a);
    a=par(a), b=par(b);
    el[b].push_back(a);
    el2[a].push_back(b);
}
void dfs(int a) {
    vis[a]=true;
    for (auto x:el[a]) {
        if (!vis[x]) {
            dfs(x);
        }
    }
    sor.push_back(a);
}
void dfs2(int a, int s) {
    vis2[a]=true;
    comp[a]=s;
    for (auto x:el2[a]) {
        if (!vis2[x]) {
            dfs2(x, s);
        }
    }
}
bool solve() {
    // 1-tol n-ig vannak a valtozok
    // false ha nincs megoldas egyebkent res[i]=1 ha az i. valtozo igaz egy lehetseges megoldasban
    for (int i=1; i<=2*n; i++) {
        if (!vis[i]) {
            dfs(i);
        }
    }
    reverse(sor.begin(), sor.end());
    for (auto x:sor) {
        if (!vis2[x]) {
            dfs2(x, ++compdb);
        }
    }
    for (int i=1; i<=n; i++) {
        int a=i, b=par(i);
        if (comp[a]==comp[b]) {
            return false;
        }

        if (comp[b]>comp[a]) {
            res[i]=1;
        }
    }
    return true;
}