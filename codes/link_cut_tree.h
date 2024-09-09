/*
original source: https://usaco.guide/adv/link-cut-tree?lang=cpp
*/
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
typedef struct snode *sn;
struct snode {         //////// VARIABLES
	sn p, c[2];        // parent, children
	bool flip = 0;     // subtree flipped or not
	int size;          // # splay tree csúcs, aktuálisban
	ll val;            // value in node
	snode(int _val) : val(_val) { p = c[0] = c[1] = NULL; upd(); }
	friend int get_size(sn x) { return x ? x->size : 0; }
	void prop() {  // lazy prop
		if (!flip) return;
		swap(c[0], c[1]);
		flip = 0;
		for (int i = 0; i < 2; i++)
			if (c[i]) c[i]->flip ^= 1;
	}
	void upd() {  // recalc vals
		for (int i = 0; i < 2; i++) if (c[i]) c[i]->prop();
		size = 1 + get_size(c[0]) + get_size(c[1]);
		// virtuális részva adatok használata
	}
	void vupd(){}
	//////// SPLAY TREE OPERATIONS
	int dir() {
		if (!p) return -2;
		for (int i = 0; i < 2; i++) if (p->c[i] == this) return i;
		return -1;  // p is path-parent pointer
	}  // -> not in current splay tree
	bool is_root() { return dir() < 0; }
	friend void set_link(sn x, sn y, int d) { if (y) y->p = x; if (d >= 0) x->c[d] = y; }
	void rot() {  // assume p and p->p propagated
		assert(!is_root());
		int x = dir(); sn pa = p;
		set_link(pa->p, this, pa->dir()); set_link(pa, c[x ^ 1], x); set_link(this, pa, x ^ 1);
		pa->upd();
	}
	void splay() {
		while (!is_root() && !p->is_root()) {
			p->p->prop(), p->prop(), prop();
			dir() == p->dir() ? p->rot() : rot(); rot();
		}
		if (!is_root()) p->prop(), prop(), rot();
		prop(); upd();
	}
	sn fbo(int b) {  // find by order
		prop(); int z = get_size(c[0]);  // of splay tree
		if (b == z) { splay(); return this; }
		return b < z ? c[0]->fbo(b) : c[1]->fbo(b - z - 1);
	}
	//////// BASE OPERATIONS
	void access() {  // bring this to top of tree, propagate
		for (sn v = this, pre = NULL; v; v = v->p) {
			v->splay();  // now switch virtual children
			if (pre) vupd(); // pre törlése (mostantól rendes gyerek)
			if (v->c[1]) vupd(); // c[1] hozzáadása (mostantól virtuális gyerek)
			v->c[1] = pre; v->upd(); pre = v;
		}
		splay();
		assert(!c[1]);  // right subtree is empty
	}
	void make_root() { // ez lesz a fa gyökere
		access(); flip ^= 1; access();
		assert(!c[0] && !c[1]);
	}
	//////// QUERIES
	friend sn lca(sn x, sn y) {
		if (x == y) return x;
		x->access(), y->access();
		if (!x->p) return NULL;
		x->splay();
		return x->p ?: x;  // y was below x in latter case
	}  // access at y did not affect x -> not connected
	friend bool connected(sn x, sn y) { return lca(x, y); }
	int dist_root() { access(); return get_size(c[0]); } // # nodes above
	sn get_root() { // get root of LCT component
		access(); sn a = this;
		while (a->c[0]) a = a->c[0], a->prop();
		a->access();
		return a;
	}
	sn get_par(int b) {  // get b-th parent on path to root | can also get min, max on path to root, etc
		access(); b = get_size(c[0]) - b;
		assert(b >= 0);
		return fbo(b);
	} 
	//////// MODIFICATIONS
	void set(ll v) { access(); val = v; upd(); } // changes value
	friend void link(sn x, sn y, bool force = 0) { // ha force: x - y él minden esetben | ha nem force: akkor y-nak gyökérnek kell lenni
		assert(!connected(x, y));
		if (force) y->make_root();  // make x par of y
		else { y->access(); assert(!y->c[0]); }
		x->access(); set_link(y, x, 0); y->upd();
	}
	friend void cut(sn y) {  // cut y from its parent | ha nincs RTE
		y->access();
		assert(y->c[0]);
		y->c[0]->p = y->c[0] = NULL;
		y->upd();
	}
	friend void cut(sn x, sn y) {  // if x, y adj in tree
		x->make_root(); y->access();
		assert(y->c[0] == x && !x->c[0] && !x->c[1]);
		cut(y);
	}
};
/*
Link-cut tree, műveletek: link, cut, set | lca, connected, dist_root, get_root, get_par
Út querry-hez a get_par-hoz hasonló implementáció kell + fbo implementáció, ha nem a teljes út kell. | Ha a, b út kell: 1. make_root(a), 2. query b-től gyökérig
Részfa adatokhoz a vupd()-et kell módosítani, (az upd()-ben is bele kell írni) | fontos kell legyen a műveletnek inverze 
*/