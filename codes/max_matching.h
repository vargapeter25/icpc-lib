/*
original source: https://judge.yosupo.jp/submission/166034
*/
#include <bits/stdc++.h>
using namespace std;

struct Matching {
  queue<int> q; int ans, n;
  vector<int> fa, s, v, pre, match;
  Matching(auto &&g) : ans(0), n(g.size()), fa(n + 1), s(n + 1), v(n + 1), pre(n + 1, n), match(n + 1, n) {
    for (int x = 0; x < n; ++x) if (match[x] == n) ans += Bfs(g, x, n);
  }
  int Find(int u) {
    return u == fa[u] ? u : fa[u] = Find(fa[u]); }
  int LCA(int x, int y, int n) {
    static int tk = 0; tk++; x = Find(x); y = Find(y);
    for (;; swap(x, y)) if (x != n) {
      if (v[x] == tk) return x;
      v[x] = tk;
      x = Find(pre[match[x]]);
    }
  }
  void Blossom(int x, int y, int l) {
    for (; Find(x) != l; x = pre[y]) {
      pre[x] = y, y = match[x];
      if (s[y] == 1) q.push(y), s[y] = 0;
      for (int z: {x, y}) if (fa[z] == z) fa[z] = l;
    }
  }
  bool Bfs(auto &&g, int r, int n) {
    iota(all(fa), 0); ranges::fill(s, -1);
    q = queue<int>(); q.push(r); s[r] = 0;
    for (; !q.empty(); q.pop()) {
      for (int x = q.front(); int u : g[x])
        if (s[u] == -1) {
          if (pre[u] = x, s[u] = 1, match[u] == n) {
            for (int a = u, b = x, last;
                b != n; a = last, b = pre[a])
              last = match[b], match[b] = a, match[a] = b;
            return true;
          }
          q.push(match[u]); s[match[u]] = 0;
        } else if (!s[u] && Find(u) != Find(x)) {
          int l = LCA(u, x, n);
          Blossom(x, u, l); Blossom(u, x, l);
        }
    }
    return false;
  }
}; // init: vector<vector<int>> gráf (n: gráf mérete), párosítás mérete: ans, párosítása i-nek: nincs -> match[i] == n | van -> match[i]