#include <bits/stdc++.h>
#include <bit>
using namespace std;

namespace ntt{
const long long mod = 998244353; // mod = 2^k * c + 1

constexpr long long mod_pow(long long n, long long h){
    long long res = 1;
    while(h){
        if(h&1) res = res * n % mod;
        n = n * n % mod;
        h >>= 1;
    }
    return res;
}

constexpr long long mod_primitive_root(){ // kiszámítja a moduló egy primitív gyökét
    array<long long, 64 - countl_zero((unsigned long long)mod)> primes;
    int size = 0;
    long long p = 2, m = mod-1;
    while(p*p <= m) { if(m % p == 0) primes[size++] = p; while(m % p == 0) m /= p; ++p; }
    if(m > 1) primes[size++] = m;
    for(long long i = 2; i < mod; i++){
        bool ok = true;
        for(int j = 0; j < size; j++) ok = ok && mod_pow(i, (mod - 1) / primes[j]) != 1;
        if(ok) return i;
    }
    return -1;
}

constexpr long long elim_pow2(long long n) { return n >> countr_zero((unsigned long long)n); }

const int max_bitc = countr_zero((unsigned long long)(mod - 1));
const long long root1 = mod_pow(mod_primitive_root(), elim_pow2(mod-1)); // q primitív gyök: root1 = q^c
const long long root1_inv = mod_pow(root1, mod-2);
 
const int max_bits_used = 22; // legfeljebb 2^max_bits_used lehet a transzformált array, max_bits_used <= max_bitc
long long roots[max_bitc], inv_roots[max_bitc], w[1<<max_bits_used], w_inv[1<<max_bits_used];
 
vector<int> rev;
void calc_rev(int B){ // bitreverse order
    int n = 1<<B; 
    if((int)rev.size() == n) return;
    rev.resize(n);
    for(int i = 0; i < n; i++) rev[i] = (rev[i / 2] | (i & 1) << B) / 2;
}

void dft(vector<long long> &a, bool inverse){ // calc_rev kell elotte, a.size() ketto hatvány
    int n = (int)a.size();
    for(int i = 0; i < n; i++) if (i < rev[i]) swap(a[i], a[rev[i]]);
    int lvl = 0;
    long long x, y;
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for(int j = 0; j < k; j++) {
                x = a[i + j];
                y = a[i + j + k];
                y = y * (inverse ? w_inv[k + j - 1] : w[k + j - 1]) % mod;
                a[i + j] = x + y;
                if(a[i + j] >= mod) a[i + j] -= mod;
                a[i  + j + k] = x - y;
                if(a[i  + j + k] < 0) a[i  + j + k] += mod;
            }
        }
        ++lvl;
    }
    if(inverse){
        long long inv = mod_pow(n, mod-2);
        for(long long &x : a) x = x * inv % mod;
    }
}
 
vector<long long> conv(const auto &a, const auto &b){ // konvolúció a és b között
    static vector<long long> in1, in2, out;
    int len = (int)(a.size() + b.size()) - 1;
    int B = 32 - __builtin_clz(len - 1), n = 1<<B;
    calc_rev(B);
    in1.assign(n, 0); in2.assign(n, 0);
    for(int i = 0; i < (int)a.size(); i++) in1[i] = a[i];
    for(int i = 0; i < (int)b.size(); i++) in2[i] = b[i];
    dft(in1, false);
    dft(in2, false);
    out.assign(n, 0);
    for(int i = 0; i < n; i++){
        out[i] = (long long)in1[i] * in2[i] % mod;
    }
    dft(out, true);
    out.resize(len);
    return out;
}
 
void ntt_init(){ // gyökök eloszámítása
    roots[max_bitc - 1] = root1;
    for(int i = max_bitc - 2; i >= 0; i--) roots[i] = roots[i + 1] * roots[i + 1] % mod;
    inv_roots[max_bitc - 1] = root1_inv;
    for(int i = max_bitc - 2; i >= 0; i--) inv_roots[i] = inv_roots[i + 1] * inv_roots[i + 1] % mod;
 
    w[0] = 1;
    w_inv[0] = 1;
    for(int i = 1; i < max_bits_used; i++){
        w[(1<<i) - 1] = 1;
        w_inv[(1<<i) - 1] = 1;
        for(int j = 0; j < (1<<i); j++){
            w[(1<<i) + j] = w[(1<<i) + j - 1] * roots[i] % mod;
            w_inv[(1<<i) + j] = w_inv[(1<<i) + j - 1] * inv_roots[i] % mod;
        }
    }
}

/*
F(P) gyöke Q_{k+1} = Q_k - F(Q_k) / F'(Q_k)
*/

void mod_x(vector<long long> &a, int n) { a.resize(min(n, (int)a.size())); }
vector<long long> add(const vector<long long> &a, const vector<long long> &b) {
    vector<long long> res;
    if(a.size() < b.size()) { res = b; for(int i = 0; i < (int)a.size(); i++) res[i] += a[i]; }
    else { res = a; for(int i = 0; i < (int)b.size(); i++) res[i] += b[i]; }
    return res;
}

vector<long long> inverse(const vector<long long> &a, int n){ // a[0] != 0;
    vector<long long> q = {mod_pow(a[0], mod-2)}; // inv(a[0])
    int len = 1;
    while(len < n){
        len *= 2;
        vector<long long> tmp = conv(span(a.begin(), min(len, (int)a.size())), span(q.begin(), min(len, (int)q.size())));
        for(int i = 0; i < min(len, (int)tmp.size()); i++) tmp[i] = mod - tmp[i];
        tmp[0] += 2;
        if(tmp[0] >= mod) tmp[0] -= mod;
        q = conv(span(q.begin(), min(len, (int)q.size())), span(tmp.begin(), min(len, (int)tmp.size())));
        q.resize(len);
    }
    q.resize(n);
    return q;
}

void derivative(vector<long long> &a){
    for(int i = 0; i < (int)a.size() - 1; i++) a[i] = a[i + 1] * (i + 1) % mod; // % mod;
    a.pop_back();
}

void integral(vector<long long> &a){
    a.resize(a.size() + 1);
    for(int i = (int)a.size() - 1; i > 0; i--) a[i] = a[i - 1] * mod_pow(i, mod-2) % mod; // % mod;
    a[0] = 0;
}

vector<long long> log(vector<long long> &a, int n){
    vector<long long> inv = inverse(a, n);
    vector<long long> ad = a;
    derivative(ad);
    ad = conv(ad, inv);
    integral(ad);
    ad.resize(n);
    return ad;
}

vector<long long> exp(vector<long long> &a, int n){
    
}

}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    ntt::ntt_init();

    int n;
    cin>>n;

    vector<long long> a(n);
    for(auto &x : a) cin>>x;

    vector<long long> l = ntt::log(a, n);
    for(long long x : l) cout << x << ' ';
    cout << '\n';

    return 0;
}