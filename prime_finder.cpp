#include <bits/stdc++.h>
using namespace std;

bool is_prime(long long x){
    long long p = 2;
    while(p*p <= x && x % p != 0) ++p;
    return p*p > x;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n = 400;

    long long base = 1<<24;
    for(int i = 1; i <= n; i++){
        long long x = base * i + 1;
        if(is_prime(x)){
            cout << x << '\n';
        }
    }

    return 0;
}