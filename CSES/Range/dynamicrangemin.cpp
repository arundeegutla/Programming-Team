#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define int long long

// 1-based tree but 0-based range
struct SegTree {
    int n;
    vector<ll> tree, lazy;
    SegTree(int n) : tree(n * 4 + 1), n(n + 1), lazy(n * 4 + 1) {}
    void lazyProcess(int at) {
        tree[2 * at] += lazy[at];
        tree[2 * at + 1] += lazy[at];
        lazy[2 * at] += lazy[at];
        lazy[2 * at + 1] += lazy[at];
        lazy[at] = 0;
    }
    void update(int at, int lo, int hi, int from, int to, ll val) {
        if (from <= lo && hi <= to) {
            tree[at] += val;
            lazy[at] += val;
            return;
        }
        lazyProcess(at);
        int mid = (lo + hi) >> 1;
        if (from <= mid) update(2 * at, lo, mid, from, to, val);
        if (to > mid) update(2 * at + 1, mid + 1, hi, from, to, val);
        tree[at] = min(tree[2 * at], tree[2 * at + 1]);
    }
    ll query(int at, int lo, int hi, int from, int to) {
        if (lo > hi) return 0;
        if (from <= lo && hi <= to) return tree[at];
        lazyProcess(at);
        ll minn = 1e18;
        int mid = (lo + hi) >> 1;
        if (from <= mid) minn = query(2 * at, lo, mid, from, to);
        if (to > mid) minn = min(minn, query(2 * at + 1, mid + 1, hi, from, to));
        return minn;
    }
    ll query(int from, int to) {
        return query(1, 0, n - 1, from, to);
    }
    void update(int from, int to, ll val) {
        update(1, 0, n - 1, from, to, val);
    }
};

signed main() {
    int n, q;
    cin >> n >> q;
    SegTree tree(n);
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        tree.update(i, i, x);
    }

    while (q--) {
        int type, a, b;
        cin >> type >> a >> b;
        if (type == 2) {
            a--, b--;
            cout << tree.query(a, b) << "\n";
        } else {
            a--;
            tree.update(a, a, -tree.query(a, a));
            tree.update(a, a, b);
        }
    }
}