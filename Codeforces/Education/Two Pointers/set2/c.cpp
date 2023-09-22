#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SegTree {
    int n;
    vector<ll> tree, lazy;
    SegTree(int n): tree(n*4+1), n(n+1), lazy(n*4+1) {}
    void lazyProcess(int at){
        tree[2*at] += lazy[at];
        tree[2*at+1] += lazy[at];
        lazy[2*at] += lazy[at];
        lazy[2*at+1] += lazy[at];
        lazy[at] = 0;
    }
    void update(int at, int lo, int hi, int from, int to, ll val){
        if(from <= lo && hi <= to){
            tree[at] += val;
            lazy[at] += val;
            return;
        }
        lazyProcess(at);
        int mid = (lo + hi) >> 1;
        if(from <= mid) update(2*at, lo, mid, from, to, val);
        if(to > mid) update(2*at+1, mid+1, hi, from, to, val);
        tree[at] = tree[2*at] + tree[2*at+1];
    }
    ll query(int at, int lo, int hi, int from, int to) {
        if(lo > hi) return 0;
        if(from <= lo && hi <= to) return tree[at];
        lazyProcess(at);
        ll total = 0;
        int mid = (lo+hi)>>1;
        if(from <= mid) total += query(2*at, lo, mid, from, to);
        if(to > mid) total += query(2*at+1, mid+1, hi, from, to);
        return total;
    }
    ll query(int from, int to){
        return query(1, 0, n-1, from, to);
    }
    void update(int from, int to, ll val){
        update(1, 0, n-1, from, to, val);
    }
};


int main(){
    ll n, s;
    cin >> n >> s;
    vector<ll> arr(n);
    for(auto& x : arr) cin >> x;

    ll pref = 0;
    set<ll> prefixes;
    prefixes.insert(0);
    for(auto x : arr) {
        pref+=x;
        prefixes.insert(pref);
    }

    vector<ll> ps;
    for(auto x : prefixes) ps.push_back(x);

    auto get = [&](ll minn) -> int {
        int lo = 0;
        int hi = n;
        int ans = n+1;
        while(lo <= hi) {
            int mid = (lo+hi) >> 1;
            if(ps[mid] >= minn) {
                hi = (ans = mid) - 1;
            } else {
                lo = mid + 1;
            }
        }
        return ans;
    };


    SegTree tree(prefixes.size()+10);

    map<ll, int> compress;
    for(auto x : prefixes){
        compress[x] = compress.size();
    }


    ll tot = 0;
    pref = 0;
    tree.update(0, 0, 1);
    for(auto x : arr){
        pref += x;
        ll minMinus = pref - s;
        int nextAvailable = get(minMinus);
        tot += tree.query(nextAvailable, n);
        tree.update(compress[pref], compress[pref], 1);
    }

    cout << tot << "\n";

}