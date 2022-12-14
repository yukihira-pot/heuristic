#ifdef mago

#include "my_pch.h"

#else

#include <bits/stdc++.h>

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

#define vi vector<int>
#define vvi vector<vi>
#define vvvi vector<vvi>
#define vl vector<ll>
#define vvl vector<vl>
#define vvvl vector<vvl>
#define vb vector<bool>
#define vvb vector<vb>

const int INFI = 1 << 30;
const ll INFL = 1LL << 60;


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
ostream& operator << (ostream& os, const vector<T> &v) {
    int sz = (int)v.size();
    for (int i = 0; i < sz; i++) {
        os << v[i] << (i == sz - 1 ? "" : " ");
    }
    return os;
}

template<typename T>
istream& operator >> (istream& is, vector<T> &v) {
    int sz = (int)v.size();
    for (int i = 0; i < sz; i++) {
        is >> v[i];
    }
    return is;
}

int dx[] = {0, -1, 0, 1, -1, -1, 1, 1};
int dy[] = {-1, 0, 1, 0, 1, -1, -1, 1};

#endif

// using Graph = vector<vector<int> >;

// #include <atcoder/all>
// const ll MOD = 998244353;
// const ll MOD = 1000000007; // 1e9 + 7
// using mint = atcoder::modint998244353;
// using mint = atcoder::modint1000000007;


void solve(int width, int height) {
    int my_matter, opp_matter;
    cin >> my_matter >> opp_matter;
    vvi scrap_amount(height, vi(width)), owner(height, vi(width));
    vvi units(height, vi(width)), recycler(height, vi(width));
    vvi can_build(height, vi(width)), can_spawn(height, vi(width));
    vvi in_range_of_recycler(height, vi(width));

    rep(i,height) {
        rep(j, width) {
            cin >> scrap_amount[i][j] >> owner[i][j]
            >> units[i][j] >> recycler[i][j] >> can_build[i][j]
            >> can_spawn[i][j] >> in_range_of_recycler[i][j];
        }
    }

    rep(i, height) {
        dbg(units[i]);
    }

    rep(i, height) {
        rep(j, width) {
            if (units[i][j] > 0 and owner[i][j] == 1) {
                rep(k, 4) {
                    int ni = i + dx[k], nj = j + dy[k];
                    if (inc(ni, 0, height) and inc(nj, 0, width) 
                    and scrap_amount[ni][nj] > 0) {
                        cout << "MOVE " << 1 << " " << j << " " << i << " " << nj << " " << ni << ";";
                        units[i][j]--;
                        units[ni][nj]++;
                    }
                }
            }
        }
    }
    cout << endl;
    // cout << "WAIT" << endl;
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(12);

    int width, height; cin >> width >> height;
    while(1) solve(width, height);
}

/*
    date: 2022-12-14 18:15
*/