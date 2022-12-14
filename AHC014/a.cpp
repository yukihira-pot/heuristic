#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_map>
#include <stack>
#include <deque>
#include <queue>
#include <iomanip>
#include <limits>
#include <bitset>
// #include <atcoder/all>

using namespace std;
using ll = long long;

#define _overload(_1, _2, _3, name, ...) name
#define rep1(n) rep3(____, 0, n)
#define rep2(i, n) rep3(i, 0, n)
#define rep3(i, a, b) for (ll i=(a); i<(ll)(b); i++)
#define rep(...) _overload(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)

#define rrep(i, a, b) for(ll i = a; i >= b; i--)
#define fore(i, a) for(auto &i:a)
#define all(x) (x).begin(),(x).end()
#define pb push_back
#define pii pair<int, int>
#define pli pair<ll, int>
#define pll pair<ll, ll>

#define UNIQUIFY(v) v.erase( unique(v.begin(), v.end()), v.end() );
#define out(x) {cout << fixed << setprecision(12) << x << "\n";}
#define dbg(x) {cerr << #x << ": " << x << "\n";}
#define inc(x, l, r) ((l) <= (x) and (x) < (r))

const int INFI = 1 << 30;
const ll INFL = 1LL << 60;

// const ll MOD = 998244353;
// const ll MOD = 1000000007; // 1e9 + 7
// using mint = atcoder::modint998244353;
// using mint = atcoder::modint1000000007;

template <typename S, typename T>
bool chmax(S& a, const T& b){ if(a < b){ a = b; return 1; } return 0; }
template <typename S, typename T>
bool chmin(S& a, const T& b){ if(b < a){ a = b; return 1; } return 0; }

template<class... T>
constexpr auto min(T... a) {
    return min(initializer_list<common_type_t<T...>>{a...});
}
template<class... T>
constexpr auto max(T... a) {
    return max(initializer_list<common_type_t<T...>>{a...});
}

template<typename T>
ostream& operator << (ostream& os, vector<T> &v) {
    int sz = (int)v.size();
    for (int i = 0; i < sz; i++) {
        os << v[i] << (i == sz - 1 ? "" : " ");
    }
    return os;
}

int dx[] = {0, -1, 0, 1, -1, -1, 1, 1};
int dy[] = {-1, 0, 1, 0, 1, -1, -1, 1};

// using Graph = vector<vector<int> >;



int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    vector<tuple<int, int, int>> v;
    rep(i, 1, 13) {
        rep(j, 1, 13) {
            v.push_back(make_tuple(i * j * 3 * abs(i - j) * abs(i - j), i, j));
        }
    }

    sort(all(v));

    int sz = v.size();

    cout << "int t1_v[] = {";
    rep(i, sz) {
        auto [s, t1, t2] = v[i];
        cout << t1 << (i == sz - 1 ? "" : ", ");
    }
    cout << "};\n";
    cout << "int t2_v[] = {";
    rep(i, sz) {
        auto [s, t1, t2] = v[i];
        cout << t2 << (i == sz - 1 ? "" : ", ");
    }
    cout << "};\n";
}