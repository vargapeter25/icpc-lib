 #include <bits/stdc++.h>

using namespace std;
vector<int> prefix_function(string s) {
    // prefix function ABAAB -> (0, 0, 1, 1, 2)
    int n=s.size();
    vector<int> ans(n, 0);
    for (int i=1; i<n; i++) {
        int ert=ans[i-1];
        while (ert && s[i]!=s[ert]) {
            ert=ans[ert-1];
        }
        if (s[i]==s[ert]) {
            ert++;
        }
        ans[i]=ert;
    }
    return ans;
}

vector<int> z_function(string s) {
    // z function ABAAB -> (0, 0, 1, 2, 0);
    int n=s.size();
    vector<int> ans(n, 0);
    int l=0, r=0;
    int lepes=0;
    for (int i=1; i<n; i++) {
        int len=0;
        if (i<r) {
            len=min(r-i, ans[i-l]);
        }
        while (i+len<n && s[i+len]==s[len]) {
            lepes++;
            len++;
        }
        ans[i]=len;
        if (i+len>r) {
            l=i, r=i+len;
        }
    }
    return ans;
}


vector<int> find_periods(string s) {
    // milyen hosszu prefix ismetlesevel kaphato meg s
    // ABABA -> (2, 4, 5)
    // a teljes periodushoz (n%i==0) feltetel kell
    int n=s.size();
    vector<int> z=z_function(s);
    z[0]=n;
    vector<int> ans;
    for (int i=1; i<n; i++) {
        if (i+z[i]==n) {
            ans.push_back(i);
        }
    }
    ans.push_back(n);
    return ans;
}

int min_rotation(string s) {
    // mennyivel kell elcsusztatni ABAAB -> 2
    int n=s.size();
    s+=s;
    int i=0, pos=0;
    while (i < n) {
        int k=i, j=i+1;
        pos=i;
        while (j<2*n && s[k]<=s[j]) {
            if (s[k]<s[j]) k=i;
            else k++;
            j++;
        }
        while (i<=k) {
            i+=j-k;
        }
    }
    return pos;
    // return s.substr(pos, n);
}