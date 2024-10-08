/*
original source: https://usaco.guide/adv/treaps?lang=cpp
*/
#include <bits/stdc++.h>
using namespace std;

mt19937 rnd(42123); // mt19937_64 ha long long kell
struct node { // az upd() és push()-t kell implementálni | upd()-et a konstruktor is hívja
	int val, w, size; // val érték (cserélheto), w súly, size a részfa mérete
    node *l, *r; // bal, jobb gyerek
	node(int c) : val(c), w(rnd()), size(1), l(NULL), r(NULL) { upd(); }
    ~node() { delete l; delete r; }
    inline void upd() {} // update az l, r-bol
    inline void push() {} // push l, r-be
} *treap;
int size(node *p) { return p ? p->size : 0; }
void split(node *p, node *&l, node *&r, int val) { // l < val | val <= r
	if(!p) { l = r = NULL; return; }
	p->push();
    if(size(p->l) < val) split(p->r, p->r, r, val - size(p->l) - 1), l = p;
	else split(p->l, l, p->l, val), r = p;
	p->size = 1 + size(p->l) + size(p->r); p->upd();
}
void merge(node *&p, node *l, node *r) {
	if(!l || !r) { p = l ? l : r; return; }
	if (l->w < r->w) l->push(), merge(l->r, l->r, r), p = l;
	else r->push(), merge(r->l, l, r->l), p = r;
	p->size = 1 + size(p->l) + size(p->r); p->upd();
}