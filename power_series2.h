/*
original source: https://judge.yosupo.jp/submission/106000
*/
#include <bits/stdc++.h>
using namespace std;

constexpr int mod = 998244353; // = 2^k * c + 1 | primitív gyöknek jónak kell lennie | 2013265921, 167772161, 2113929217
constexpr int N = 1 << 20; // 2^l, l <= k | max N amit transzformálni lehet

constexpr inline long long mod_pow(long long n, long long h) {
    long long res = 1;
    for(;h;h >>= 1) { if(res&(1LL)) res = res * n % mod; n = n * n % mod; }
    return res;
}

constexpr long long mod_primitive_root(){ // kiszámítja a moduló egy primitív gyökét
    long long primes[64] = {}; int size = 0; long long p = 2, m = mod-1;
    while(p*p <= m) { if(m % p == 0) primes[size++] = p; while(m % p == 0) m /= p; ++p; } if(m > 1) primes[size++] = m;
    for(long long i = 2; i < mod; i++) { bool ok = true; for(int j = 0; j < size; j++) ok = ok && mod_pow(i, (mod - 1) / primes[j]) != 1; if(ok) return i; }
    return -1;
}

constexpr inline long long mod_sqrt(long long a) {
    if (mod_pow(a, mod >> 1) != 1) return 0;
    int Q = (mod - 1) >> (__countr_zero(mod-1));
    long long x = mod_pow(a, (Q + 1) >> 1), y = mod_pow(x, Q);
    for (int k = __countr_zero(mod - 1) - 1; k >= 0; --k)
        if (mod_pow(y, 1 << k) != 1) {
            x *= pow(mod_primitive_root(), mod >> (k + 2));
            y *= pow(mod_primitive_root(), mod >> (k + 1));
        }
    return min(x, mod - x);
}

long long w[N];
long long invi[N + 1];
__attribute__((constructor)) void init() {
    invi[1] = w[N / 2] = 1;
    constexpr long long g = mod_pow(mod_primitive_root(), mod / N);
    for (int i = N / 2 + 1; i < N; ++i) w[i] = w[i - 1] * g % mod;
    for (int i = N / 2 - 1; i > 0; --i) w[i] = w[i << 1];
    for (int i = 2; i <= N; i++) invi[i] = invi[mod % i] * (mod - mod / i) % mod; // csak a log-hoz kell
}
void dft(long long f[], int n) { // n ketto hatvány
    for (int k = n / 2; k; k /= 2)
        for (int i = 0; i < n; i += k + k)
            for (int j = 0; j < k; ++j) {
                long long x = f[i + j]; long long y = f[i + j + k]; f[i + j] = (x + y) % mod; f[i + j + k] = (mod + x - y) * w[k + j] % mod;
            }
}
void ift(long long f[], int n) { // n ketto hatvány
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += k + k)
            for (int j = 0; j < k; ++j) {
                long long x = f[i + j]; long long y = f[i + j + k] * w[k + j] % mod; f[i + j] = (x + y) % mod; f[i + j + k] = (mod + x - y) % mod;
            }
    long long inv = mod - (mod - 1) / n;
    std::reverse(f + 1, f + n);
    for (int i = 0; i < n; ++i) f[i] = f[i] * inv % mod;
}

template<class OP>
vector<long long> poly_op(OP op, vector<long long> a, vector<long long> b) {
    int sz = max(a.size(), b.size()); a.resize(sz); b.resize(sz);
    vector<long long> res(sz);
    for(int i = 0; i < sz; i++) res[i] = op(a[i], b[i]);
    return res;
}
auto add(const vector<long long> &a, const vector<long long> &b) { return poly_op([](long long a, long long b) { return a + b; }, a, b); } // mod kell !!!!!!!!!!!!!!
auto sub(const vector<long long> &a, const vector<long long> &b) { return poly_op([](long long a, long long b) { return a - b; }, a, b); }
auto mul(const vector<long long> &a, const vector<long long> &b) { return poly_op([](long long a, long long b) { return a * b; }, a, b); }
auto& dft(vector<long long> &a, int n) { a.resize(n); dft(a.data(), n); return a; }
auto& ift(vector<long long> &a, int n) { a.resize(n); ift(a.data(), n); return a; }
auto derivative(const vector<long long> &a) { vector<long long> res(max((int)a.size() - 1, 0)); for(int i = 0; i < (int)a.size() - 1; i++) res[i] = a[i + 1] * (i + 1); }
auto integral(const vector<long long> &a) { vector<long long> res(a.size() + 1); for(int i = 1; i < (int)res.size(); i++) res[i] = a[i-1] * invi[i] % mod; } // mod + overflow lehet invi[] mérete N
auto conv(vector<long long> a, vector<long long> b){
    int sz = (int)a.size() + (int)b.size() - 1, n = __bit_ceil(max(sz, 0));
    dft(a, n); dft(b, n);
    a = mul(a, b);
    ift(a, n);
    a.resize(sz);
    return a;
}
auto inv(vector<long long> a) {
    if(a[0] == 0) return vector<long long>{};
    int m = a.size();
    vector<long long> inv = {mod_pow(a[0], mod - 2)};
    for(int k = 1; k < m; k *= 2){
        int n = k * 2; 
        vector<long long> p = inv, q(a.begin(), a.begin() + min((int)a.size(), n)); 
        dft(p, n); dft(q, n);
        q = mul(p, q);
        ift(q, n);
        q.erase(q.begin(), q.begin() + k);
        dft(q, n);
        p = mul(p, q);
        ift(p, n);
        p.resize(k);
        p.insert(p.begin(), k, 0);
        inv = sub(inv, p);
    }
    inv.resize(m);
    return inv;
}

vector<long long> log(vector<long long>) { // res[0] = 0
    int n = __bit_ceil(size() * 2 - 1);
    return copy().derivative().dft(n).mul(inv().dft(n)).ift(n).integral().resize(size());
}

struct poly : std::vector<mint> { using std::vector<mint>::vector;
    poly &add(const poly &o) { if (size() < o.size()) resize(o.size()); for (int i = 0; i < o.size(); ++i) (*this)[i] += o[i]; return *this; }
    poly &sub(const poly &o) { if (size() < o.size()) resize(o.size()); for (int i = 0; i < o.size(); ++i) (*this)[i] -= o[i]; return *this; }
    poly &mul(const poly &o) { if (size() < o.size()) resize(o.size()); for (int i = 0; i < o.size(); ++i) (*this)[i] *= o[i]; return *this; }
    poly &mul(const mint &o) { for (mint &i: *this) i *= o; return *this; }
    poly &derivative() { for(int i = 0; i < (int)size() - 1; i++) (*this)[i] = (*this)[i + 1] * mint(i + 1); pop_back(); return *this; }
    poly &integral()   { resize(size()+1); for(int i = (int)size() - 1; i > 0; i--) (*this)[i] = (*this)[i-1] * invi[i]; (*this)[0] = mint(); return *this; } // lehet overflow invi mérete N !!!
    
    poly copy() const { return *this; }
    poly &resize(auto sz) { return vector::resize(sz), *this; }
    poly &dft(int n) { return ::dft(resize(n).data(), n), *this; }
    poly &ift(int n) { return ::ift(resize(n).data(), n), *this; }
    poly &ins(int sz) { return insert(begin(), sz, mint()), *this; }
    poly &del(int sz) { return erase(begin(), begin() + sz), *this; }
    poly &reverse() { return std::reverse(begin(), end()), *this;}
    poly pre(int sz) const { return sz < size() ? poly(begin(), begin() + sz) : copy(); }
    poly &reduce() { while(!empty() && back().x == 0) pop_back(); return *this; }

    poly conv(const poly &o){
        int n = __bit_ceil(size() + o.size() - 1); 
        return copy().dft(n).mul(o.copy().dft(n)).ift(n).resize(size() + o.size() - 1);
    }
    poly inv() const {
        if (front().x == 0) return {};
        int m = size();
        poly inv = {front().inv()};
        for (int k = 1; k < m; k *= 2) {
            int n = k * 2; poly a = inv.copy().dft(n), b = pre(n).dft(n);
            inv.sub(a.copy().mul(b).ift(n).del(k).dft(n).mul(a).ift(n).resize(k).ins(k));
        }
        return inv.resize(m);
    }
    poly log() const{ // res[0] = 0
        int n = __bit_ceil(size() * 2 - 1);
        return copy().derivative().dft(n).mul(inv().dft(n)).ift(n).integral().resize(size());
    }
    poly exp() const { // p[0] == 0, különben nem valid az eremény
        if (front().x != 0) return {};
        int m = size();
        poly e = {1};
        for (int k = 1; k < m; k *= 2) {
            int n = k * 2;
            poly elog = e.resize(n).log(); e.dft(n*2);
            e.add(pre(n).sub(elog).dft(n*2).mul(e)).ift(n*2).resize(n);
        }
        return e.resize(m);
    }
    poly pow(auto k) const { // k: int, long long
        if(k == 0) return poly{1}.resize(size());
        int j = 0;
        while(j < size() && (*this)[j].x == 0) ++j;
        if(j == size()) return poly{0}.resize(size());
        mint c = (*this)[j];
        return copy().del(j).mul(c.inv()).log().mul(mint(k % mod)).exp().mul(c.pow(k % (mod - 1))).ins(j > size() / k ? (long long)size() : j * k).resize(size());
    }
    poly sqrt() const { // ha nem létezik akkor az eredmény: {}
        int j = 0;
        while(j < size() && (*this)[j].x == 0) ++j;
        if(j == size()) return poly{0}.resize(size());
        mint c = (*this)[j].sqrt();
        if(c.x == 0 || j % 2 != 0) return {};
        return copy().del(j).mul((*this)[j].inv()).resize(size() - j / 2).pow(mint(2).inv().x).mul(c).ins(j / 2);
    }
    poly div(const poly& o) {
        poly a = copy().reduce().reverse(), b = o.copy().reduce().reverse();
        int m = a.size() - b.size() + 1;
        if(a.empty() || b.empty() || a.size() < b.size()) return b.empty() ? poly{} : poly{0};
        return a.conv(b.resize(a.size()).inv()).resize(m).reverse();
    }
    poly rem(const poly& o) { 
        return copy().sub(div(o).conv(o));
    }
};

/*
poly: ugyanúgy muködik, mint az std::vector
muveletek: +, -, * pontoknékt, derivátl, integrál, (resize, dft, ift, ins, del, reverse)
constans muveletek: conv, inv, log, exp, pow, sqrt, div, rem
*/