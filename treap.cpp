#include <bits/stdc++.h>
using namespace std;

const long long mod = 998244353;
mt19937 rnd(42123); // mt19937_64

struct node {
	int val, w, size;

    long long sum = 0, a = 1, b = 0;
    bool rev = false;
    
    node *l, *r;
	node(int c) : val(c), w(rnd()), size(1), sum(c), l(NULL), r(NULL) {}
    ~node() { delete l; delete r; }
    inline void upd() {
        sum = ((val + get_sum(l) + get_sum(r)) * a + b * size) % mod; 
    }
    inline void push() {
        if(rev){
            if(l) l->rev = !l->rev;
            if(r) r->rev = !r->rev;
            swap(l, r);
            rev = false;
        }
        if(a != 1 || b != 0){
            val = (val * a + b) % mod;
            if(l) l->upd_vals(a, b);
            if(r) r->upd_vals(a, b);
            a = 1, b = 0;
        }
    }
    inline void upd_vals(long long a_, long long b_){
        a = a * a_ % mod;
        b = (b * a_ + b_) % mod;
        upd();
    }
    inline static long long get_sum(node* p) { return p ? p->sum : 0LL; }
} *root;

int size(node *p) { return p ? p->size : 0; }

void split(node *p, node *&l, node *&r, int val) { // l < val | val <= r
	if(!p) { l = r = NULL; return; }
	p->push();
    if(size(p->l) < val) split(p->r, p->r, r, val - size(p->l) - 1), l = p;
	else split(p->l, l, p->l, val), r = p;
	p->size = 1 + size(p->l) + size(p->r);
    p->upd();
}

void merge(node *&p, node *l, node *r) {
	if(!l || !r) { p = l ? l : r; return; }
	if (l->w < r->w) l->push(), merge(l->r, l->r, r), p = l;
	else r->push(), merge(r->l, l, r->l), p = r;
	p->size = 1 + size(p->l) + size(p->r);
    p->upd();
}

void insert(node* &p, int idx, int c){
    node *s1, *s2;
    split(p, s1, s2, idx);
    merge(p, s1, new node(c));
    merge(p, p, s2);
}

void remove(node* &p, int idx){
    node *s1, *s2, *s3;
    split(p, s1, s2, idx);
    split(s2, s2, s3, 1);
    merge(p, s1, s3);
    delete s2;
}

void reverse(node* &p, int l, int r){
    node *s1, *s2, *s3;
    split(p, s2, s3, r);
    split(s2, s1, s2, l);
    if(s2) s2->rev = !s2->rev;
    merge(p, s1, s2);
    merge(p, p, s3);
}

void update(node* &p, int l, int r, long long a, long long b){
    node *s1, *s2, *s3;
    split(p, s2, s3, r);
    split(s2, s1, s2, l);
    if(s2) s2->upd_vals(a, b);
    merge(p, s1, s2);
    merge(p, p, s3);
}

long long get_sum(node* &p, int l, int r){
    node *s1, *s2, *s3;
    split(p, s2, s3, r);
    split(s2, s1, s2, l);
    long long res = s2 ? s2->sum : 0LL;
    merge(p, s1, s2);
    merge(p, p, s3);
    return res;
}

node *treap;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);


    vector<int> v;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            for(int k = 0; k < m; k++){
                for(int l = 0; l < n; l++){
                    
                }
            }
        }
    }

    int n, q;
    cin>>n>>q;

    for(int i = 0; i < n; i++){
        int x;
        cin>>x;
        merge(treap, treap, new node(x));
    }

    for(int i = 0; i < q; i++){
        int t;
        cin>>t;
        if(t == 0){
            int p, x;
            cin>>p>>x;
            insert(treap, p, x);
        } else if(t == 1){
            int p;
            cin>>p;
            remove(treap, p);
        } else if(t == 2){
            int l, r;
            cin>>l>>r;
            reverse(treap, l, r);
        } else if(t == 3){
            int l, r;
            long long a, b;
            cin>>l>>r>>a>>b;
            update(treap, l, r, a, b);
        } else{
            int l, r;
            cin>>l>>r;
            long long ans2 = get_sum(treap, l, r);
            cout << ans2 << '\n';
        }
    }

    return 0;
}