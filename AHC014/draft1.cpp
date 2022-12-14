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

struct UnionFind {
    // par[i] := parent of i
    vector<int> par, rank, siz;

    // initialization
    UnionFind(int N) : par(N, -1), rank(N, 0), siz(N, 1) {}

    int root(int x) {
        /* returns the root of x */
        if (par[x] == -1) return x;
        // 経路圧縮
        return par[x] = root(par[x]);
    }

    void unite(int x, int y) {
        /* unite the trees where x and y belong */
        int rx = root(x);
        int ry = root(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) swap(rx, ry);
        par[ry] = rx;
        if (rank[rx] == rank[ry]) rank[rx]++;
        siz[rx] += siz[ry];
    }

    bool same(int x, int y) {
        /* returns true if x and y belong to the same tree */
        return root(x) == root(y);
    }

    int size(int x) {
        /* returns the size of the tree that includes x */
        return siz[root(x)];
    }
};

struct ConnectAction {
    int x1, y1, x2, y2, x3, y3, x4, y4;
    ConnectAction(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), x4(x4), y4(y4) {}
};

struct Solver {
    vector<ConnectAction> res;
    vector<pair<int, int>> coordinates;
    vector<vector<bool>> field, used;
    int N;
    UnionFind UF;
    Solver(int N, vector<vector<bool>> &field) : N(N), field(field), used(N, vector<bool>(N)), UF(N * N) {}

    pair<int, int> can_straightly_connect(pair<int, int> p2, pair<int, int>p3, pair<int, int> p4) 
    {
        auto [x2, y2] = p2;
        auto [x3, y3] = p3;
        auto [x4, y4] = p4;
        pair<int, int> cannot_connect_co = {-1, -1};
        int x1 = x2 ^ x3 ^ x4;
        int y1 = y2 ^ y3 ^ y4;
        if (field[x1][y1]) return cannot_connect_co;
        if (!(field[x2][y2] and field[x3][y3] and field[x4][y4])) return cannot_connect_co;
        rep(x, x2, x3+1) {
            if (used[x][y2] or used[x][y4]) return cannot_connect_co;
        }
        rep(y, y2, y4+1) {
            if (used[x2][y] or used[x3][y]) return cannot_connect_co;
        }
        rep(x, x2+1, x3) {
            if (field[x][y2] or field[x][y4]) return cannot_connect_co;
        }
        rep(y, y2+1, y4) {
            if (field[x2][y] or field[x3][y]) return cannot_connect_co;
        }
        return {x1, y1};
    }

        int cnt = 0;
    void straightly_connect(pair<int, int> p2, pair<int, int> p3, pair<int, int> p4) 
    {
        auto [x2, y2] = p2;
        auto [x3, y3] = p3;
        auto [x4, y4] = p4;
        pair<int, int> cannot_connect_co = {-1, -1};
        pair<int, int> connect_co = can_straightly_connect(p2, p3, p4);
        auto [x1, y1] = connect_co;
        if (connect_co != cannot_connect_co) {
            field[x1][y1] = true;
            for (int x = x2; x <= x3; x++) {
                used[x][y2] = true;
                used[x][y4] = true;
            }
            for (int y = y2; y <= y4; y++) {
                used[x2][y] = true;
                used[x3][y] = true;
            }
            coordinates.push_back({connect_co});
            res.emplace_back(x1, y1, x4, y4, x2, y2, x3, y3);
            field[x1][y1] = field[x2][y2] = field[x3][y3] = field[x4][y4] = false;
            cnt++;
        }
    }

    void straightly_connect_action() 
    {
        rep(x2, N) {
            rep(y2, N) {
                rep(x4, x2+1, N) {
                    rep(y4, y2+1, N) {
                        int x3 = x4;
                        int y3 = y2;
                        pair<int, int> p2 = {x2, y2}, p3 = {x3, y3}, p4 = {x4, y4};
                        straightly_connect(p2, p3, p4);
                    }
                }
            }
        }

        rep(x2, N) {
            rep(y2, N) {
                rep(x4, x2) {
                    rep(y4, y2+1, N) {
                        int x3 = x2, y3 = y4;
                        pair<int, int> p2 = {x2, y2}, p3 = {x3, y3}, p4 = {x4, y4};
                        straightly_connect(p2, p3, p4);
                    }
                }
            }
        }

        rep(x2, N) {
            rep(y2, N) {
                rep(x4, x2) {
                    rep(y4, y2) {
                        int x3 = x4, y3 = y2;
                        pair<int, int> p2 = {x2, y2}, p3 = {x3, y3}, p4 = {x4, y4};
                        straightly_connect(p2, p3, p4);
                    }
                }
            }
        }

        rep(x2, N) {
            rep(y2, N) {
                rep(x4, x2+1, N) {
                    rep(y4, y2) {
                        int x3 = x2, y3 = y4;
                        pair<int, int> p2 = {x2, y2}, p3 = {x3, y3}, p4 = {x4, y4};
                        straightly_connect(p2, p3, p4);
                    }
                }
            }
        }
    }

    vector<ConnectAction> ret() { 
        straightly_connect_action();
        return res; 
    }
};

// -----------------------------------------------

vector<vector<bool>> field_input() {
    int N, M;
    cin >> N >> M;
    vector<vector<bool>> field(N, vector<bool>(N));
    while (M--) {
        int x, y;
        cin >> x >> y;
        field[x][y] = true;
    }
    return field;
}

void print_answer(vector<ConnectAction> &res) {
    cout << res.size() << "\n";
    for (auto& [x1, y1, x2, y2, x3, y3, x4, y4] : res) {
        cout << x1 << " " << y1 << " " << x2 << " " << y2 << " "
        << x3 << " " << y3 << " " << x4 << " " << y4 << "\n";
    }
}

void solve() {
    vector<vector<bool>> field = field_input();
    int N = field.size();
    Solver solver(N, field);
    vector<ConnectAction> ans = solver.ret();
    print_answer(ans);
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    solve();
}