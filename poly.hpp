#include <bits/stdc++.h>
using namespace std;

inline int inv(int x) {}

void fft(vector<int> &a){

}

void conv(const auto& a, const auto& b, vector<int>& out){
    static const int s_size = 1e4;
    static vector<int> buf(s_size);
    int deg = (int)a.size() + (int)b.size() - 1;
    int B = (32 - __bulitin_clz(deg)), n = 1<<B;
    out.resize(n);
    buf.resize(n);
    fill(out.begin(), out.end(), 0);
    fill(buf.begin(), buf.end(), 0);
    copy(a.begin(), a.end(), out.begin());
    copy(b.begin(), b.end(), buf.begin());
    fft(out);
    fft(buf);
    /*
    dot prod
    */
    fft(out);
    out.resize(deg);
}

inline void mod_x(vector<int> &a, int n){
    a.resize(min((int)a.size(), n));
}

inline void get_derivative(vector<int> &a){
    for(int i = 0; i < (int)a.size() - 1; i++) a[i] = a[i + 1] * (i + 1);
    a.pop_back();
}

inline void get_integral(vector<int> &a){
    a.push_back(0);
    for(int i = (int)a.size() - 1; i > 0; i--) a[i] = a[i-1] * inv(i);
    a[0] = 0;
    return;
}

void inv(const vector<int> &a, int n, vector<int>& out){
    static vector<int> ab;
    assert(a.size() > 0 && a[0] != 0);
    int cur = 1;
    out.resize(1);
    out[0] = inv(a[0]);
    while(cur < n){
        cur *= 2;
        conv(span(a.begin(), min(cur, (int)a.size())), span(out.begin(), cur / 2), ab);
        for(int i = 0; i < n; i++) ab[i] = -ab[i];
        ab[0] += 2;
        conv(span(out.begin(), cur / 2), span(ab.begin(), min((int)ab.size(), cur)), out);
    }
    out.resize(n);
}

vector<int> log(const vector<int> &a, int n){
    vector<int> a_d = a;
    get_derivative(a_d);
    vector<int> a_inv;
    inv(a, n, a_inv);
    conv(a_d, a_inv, a_d);
    get_integral(a_d);
    a_d.resize(n);
    return a_d;
}

vector<int> exp(const vector<int> &a, int n){
    assert(!a.empty() && a[0] == 0);
    vector<int> b = {1};
    int cur = 1;
    while(cur < n){
        cur *= 2;
        
    }
}

vector<int> pow(const vector<int> &a, int k, int n){
    vector<int> a_log = log(a, n);
    for(int &x : a_log) x *= k;
    return exp(a_log, n);
}

vector<int> sum_of_pow(const vector<int> &a, int n){
    vector<int> b = a;
    for(int &x : b) x = -x;
    b[0] += 1;
    vector<int> out;
    inv(a, n, out);
    return out;
}