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
#define pii pair<int, int>
#define pli pair<ll, int>
#define pll pair<ll, ll>

#define UNIQUIFY(v) v.erase( unique(v.begin(), v.end()), v.end() );
#define dbg(x) {cerr << #x << ": " << x << "\n";}
#define inc(x, l, r) ((l) <= (x) and (x) < (r))

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

// sizを重みづけするよりも、新しい配列を作った方がいい
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
    UnionFind UF;
    int N;
    map<pair<int, int>, bool> is_connected;
    Solver(int N, vector<vector<bool>> &field) : N(N), field(field), used(N, vector<bool>(N)), UF(N * N) {}

    int cnvt(int i, int j) { return i * N + j; }

    pair<int, int> can_straightly_connect(pair<int, int> p2, pair<int, int>p3, pair<int, int> p4) 
    {
        auto [x2, y2] = p2;
        auto [x3, y3] = p3;
        auto [x4, y4] = p4;
        pair<int, int> cannot_connect_co = {-1, -1};
        int x1 = x2 ^ x3 ^ x4;
        int y1 = y2 ^ y3 ^ y4;

        int top = max({x1, x2, x3, x4});
        int bottom= min({x1, x2, x3, x4});
        int right = max({y1, y2, y3, y4});
        int left = min({y1, y2, y3, y4});

        vector<int> xs = {x1, x2, x3, x4};
        vector<int> ys = {y1, y2, y3, y4};
    
        if (field[x1][y1]) return cannot_connect_co;
        // if (x1 == 11 and y1 == 21 and x2 == 10 and y2 == 21 and x3 == 10 and y3 == 13 and y2 == 21 and x3 == 10 and y3 == 13) dbg("exe0");
        if (!(field[x2][y2] and field[x3][y3] and field[x4][y4])) return cannot_connect_co;

        rep(x, bottom, top) {
            // if (x1 == 11 and y1 == 21 and x2 == 10 and y2 == 21 and x3 == 10 and y3 == 13 and y2 == 21 and x3 == 10 and y3 == 13) dbg("exe1");
            if (is_connected.find( { cnvt(x, left), cnvt(x+1, left) } ) != is_connected.end()) {
                return cannot_connect_co;
            }
            if (is_connected.find( { cnvt(x, right), cnvt(x+1, right) } ) != is_connected.end()) {
                return cannot_connect_co;
            }
            // if (UF.same(cnvt(x, left), cnvt(x+1, left))) return cannot_connect_co;
            // if (UF.same(cnvt(x, right), cnvt(x+1, right))) return cannot_connect_co;
        }
        rep(y, left, right) {
            // if (x1 == 11 and y1 == 21 and x2 == 10 and y2 == 21 and x3 == 10 and y3 == 13) {
            //     dbg("exe2");
            //     dbg(bottom);
            //     dbg(y);
            //     dbg(top);
            // }
            if (is_connected.find( { cnvt(bottom, y), cnvt(bottom, y+1) } ) != is_connected.end()) {
                return cannot_connect_co;
            }
            if (is_connected.find( { cnvt(top, y), cnvt(top, y+1) } ) != is_connected.end()) {
                return cannot_connect_co;
            }
            // if (UF.same(cnvt(bottom, y), cnvt(bottom, y+1))) {
            //     return cannot_connect_co;
            // }
            // if (UF.same(cnvt(top, y), cnvt(top, y+1))) {
            //     return cannot_connect_co;
            // }
        }
        if (x1 == 11 and y1 == 21 and x2 == 10 and y2 == 21 and x3 == 10 and y3 == 13) dbg("exe2 fin");
        rep(x, bottom+1, top) {
            if (x1 == 11 and y1 == 21 and x2 == 10 and y2 == 21 and x3 == 10 and y3 == 13) dbg("exe3");
            if (field[x][left]) return cannot_connect_co;
            if (field[x][right]) return cannot_connect_co;
        }
        rep(y, left+1, right) {
            if (x1 == 11 and y1 == 21) dbg("exe4");
            if (field[bottom][y]) return cannot_connect_co;
            if (field[top][y]) return cannot_connect_co;
        }
        return {x1, y1};
    }

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
            int top = max({x1, x2, x3, x4});
            int bottom= min({x1, x2, x3, x4});
            int right = max({y1, y2, y3, y4});
            int left = min({y1, y2, y3, y4});
            for (int x = bottom; x <= top; x++) {
                used[x][left] = true;
                used[x][right] = true;
            }
            for (int y = left; y <= right; y++) {
                used[bottom][y] = true;
                used[top][y] = true;
            }
            coordinates.push_back({connect_co});
            int diag_x = 0, diag_y = 0;
            vector<int> xs = {x1, x2, x3, x4};
            vector<int> ys = {y1, y2, y3, y4};
            for (auto& x : xs) if (x != x1) diag_x = x;
            for (auto& y : ys) if (y != y1) diag_y = y;

            rep(x, bottom, top) {
                is_connected[{ cnvt(x, left), cnvt(x+1, left) }] = true;
                // is_connected[{ cnvt(x+1, left), cnvt(x, left) }] = true;
                is_connected[{ cnvt(x, right), cnvt(x+1, right) }] = true;
                // is_connected[{ cnvt(x+1, right), cnvt(x, right) }] = true;
                // UF.unite( cnvt(x, left), cnvt(x+1, left) );
                // UF.unite( cnvt(x, right), cnvt(x+1, right) );
            }
            rep(y, left, right) {
                is_connected[{ cnvt(bottom, y), cnvt(bottom, y+1) }] = true;
                // is_connected[{ cnvt(bottom, y+1), cnvt(bottom, y) }] = true;
                is_connected[{ cnvt(top, y), cnvt(top, y+1) }] = true;
                // is_connected[{ cnvt(top, y+1), cnvt(top, y) }] = true;
                // UF.unite( cnvt(bottom, y), cnvt(bottom, y+1) );
                // UF.unite( cnvt(top, y), cnvt(top, y+1) );
            }

            res.emplace_back(x1, y1, x1, diag_y, diag_x, diag_y, diag_x, y1);
        }
    }

    void straightly_connect_action() 
    {
        rep(cnt0, 3) {
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
                            // if (p2 == make_pair(10, 21) and p4 == make_pair(11, 13)) {
                            //     cerr << "kokodayo\n";
                            //     int x1 = x2 ^ x3 ^ x4;
                            //     int y1 = y2 ^ y3 ^ y4;
                            //     dbg(x1);
                            //     dbg(y1);
                            // }
                            straightly_connect(p2, p3, p4);
                        }
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