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
#define point pair<int, int>
#define pli pair<ll, int>
#define pll pair<ll, ll>

#define UNIQUIFY(v) v.erase( unique(v.begin(), v.end()), v.end() );
#define dbg(x) {cerr << #x << ": " << x << "\n";}
#define inc(x, l, r) ((l) <= (x) and (x) < (r))
#define pinc(p, l, r) (inc(p.first, 0, N) and inc(p.second, 0, N))

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

int dx[] = {1, -1};
int dy[] = {1, -1};

int start_time;
// 最後はここ 4.5 とかにしたい
const double STOP_TIME = 3.0;
ll total_score = 0ll;
int TESTCASES = 50;

struct ConnectAction 
{
    int x1, y1, x2, y2, x3, y3, x4, y4;
    ConnectAction(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), x4(x4), y4(y4) {}
};

struct Solver 
{
    vector<ConnectAction> res;
    vector<vector<bool>> field, used;
    int N;
    map<point, bool> is_connected_mp;
    Solver(int N, vector<vector<bool>> &field) : N(N), field(field), used(N, vector<bool>(N)) {}
    int cnt = 0;
    const int MAX_DIAG = 7;

    int cnvt(int i, int j) { return i * N + j; }

    // O(N log N)
    point can_straightly_connect(point p2, point p3, point p4) 
    {
        auto [x2, y2] = p2;
        auto [x3, y3] = p3;
        auto [x4, y4] = p4;
        point cannot_connect_co = {-1, -1};
        int x1 = x2 ^ x3 ^ x4;
        int y1 = y2 ^ y3 ^ y4;

        int top = max({x1, x2, x3, x4});
        int bottom= min({x1, x2, x3, x4});
        int right = max({y1, y2, y3, y4});
        int left = min({y1, y2, y3, y4});

        vector<int> xs = {x1, x2, x3, x4};
        vector<int> ys = {y1, y2, y3, y4};
    
        if (field[x1][y1]) return cannot_connect_co;
        if (!(field[x2][y2] and field[x3][y3] and field[x4][y4])) return cannot_connect_co;

        // 同じ辺を二度使用していないか
        rep(x, bottom, top) {
            if (is_connected_mp.find( { cnvt(x, left), cnvt(x+1, left) } ) != is_connected_mp.end()) {
                return cannot_connect_co;
            }
            if (is_connected_mp.find( { cnvt(x, right), cnvt(x+1, right) } ) != is_connected_mp.end()) {
                return cannot_connect_co;
            }
        }
        rep(y, left, right) {
            if (is_connected_mp.find( { cnvt(bottom, y), cnvt(bottom, y+1) } ) != is_connected_mp.end()) {
                return cannot_connect_co;
            }
            if (is_connected_mp.find( { cnvt(top, y), cnvt(top, y+1) } ) != is_connected_mp.end()) {
                return cannot_connect_co;
            }
        }

        // 辺上に格子点がないか
        rep(x, bottom+1, top) {
            if (field[x][left]) return cannot_connect_co;
            if (field[x][right]) return cannot_connect_co;
        }
        rep(y, left+1, right) {
            if (field[bottom][y]) return cannot_connect_co;
            if (field[top][y]) return cannot_connect_co;
        }
        return {x1, y1};
    }

    // O(N log N)
    void straightly_connect(point p2, point p3, point p4) 
    {
        auto [x2, y2] = p2;
        auto [x3, y3] = p3;
        auto [x4, y4] = p4;
        point cannot_connect_co = {-1, -1};
        point connect_co = can_straightly_connect(p2, p3, p4);
        auto [x1, y1] = connect_co;
        if (connect_co != cannot_connect_co) {
            field[x1][y1] = true;
            int top = max({x1, x2, x3, x4});
            int bottom= min({x1, x2, x3, x4});
            int right = max({y1, y2, y3, y4});
            int left = min({y1, y2, y3, y4});
            int diag_x = 0, diag_y = 0;
            vector<int> xs = {x1, x2, x3, x4};
            vector<int> ys = {y1, y2, y3, y4};
            for (auto& x : xs) if (x != x1) diag_x = x;
            for (auto& y : ys) if (y != y1) diag_y = y;

            rep(x, bottom, top) {
                is_connected_mp[{ cnvt(x, left), cnvt(x+1, left) }] = true;
                is_connected_mp[{ cnvt(x, right), cnvt(x+1, right) }] = true;
            }
            rep(y, left, right) {
                is_connected_mp[{ cnvt(bottom, y), cnvt(bottom, y+1) }] = true;
                is_connected_mp[{ cnvt(top, y), cnvt(top, y+1) }] = true;
            }

            res.emplace_back(x1, y1, x1, diag_y, diag_x, diag_y, diag_x, y1);
        }
    }

    // O(N^5 log N)
    void straightly_connect_action() 
    {
        rep(x2, N) {
            rep(y2, N) {
                rep(x4, x2+1, N) {
                    rep(y4, y2+1, N) {
                        int x3 = x4;
                        int y3 = y2;
                        point p2 = {x2, y2}, p3 = {x3, y3}, p4 = {x4, y4};
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
                        point p2 = {x2, y2}, p3 = {x3, y3}, p4 = {x4, y4};
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
                        point p2 = {x2, y2}, p3 = {x3, y3}, p4 = {x4, y4};
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
                        point p2 = {x2, y2}, p3 = {x3, y3}, p4 = {x4, y4};
                        straightly_connect(p2, p3, p4);
                    }
                }
            }
        }
    }

    // 隣接する p, q 間に辺が存在するかを返す
    bool is_connected(int px, int py, int qx, int qy) {
        return ( is_connected_mp.find({ cnvt(px, py), cnvt(qx, qy) }) != is_connected_mp.end() or is_connected_mp.find({ cnvt(qx, qy), cnvt(px, py) }) != is_connected_mp.end() );
    }
    // 隣接する p, q 間に辺を作成
    void connect_adjacent_point(int px, int py, int qx, int qy) {
        is_connected_mp[{ cnvt(px, py), cnvt(qx, qy) }] = true;
    }

    // 点 p, q 間の任意の 2 点に既に辺が存在するかを返す
    bool is_side_used(point p, point q) {
        // p の x 座標 < q の x 座標となるように
        if (p.first > q.first) swap(p, q);
        auto [px, py] = p;
        auto [qx, qy] = q;
        int y_diff = 0;
        if (py < qy) y_diff = 1;
        else if (py > qy) y_diff = -1;
        int cy = py;
        rep(x, px, qx) {
            if ( is_connected(x, cy, x+1, cy+y_diff) ) return true;
            cy += y_diff;
        }
        return false;
    }
    // p, q 間のに辺を形成する
    void connect(point p, point q) {
        // p の x 座標 < q の x 座標となるように
        if (p.first > q.first) swap(p, q);
        auto [px, py] = p;
        auto [qx, qy] = q;
        int y_diff = 0;
        if (py < qy) y_diff = 1;
        else if (py > qy) y_diff = -1;
        int cy = py;
        rep(x, px, qx) {
            connect_adjacent_point(x, cy, x+1, cy+y_diff);
            cy += y_diff;
        }
    }

    // 点 p, q 間に格子点が存在するかを返す
    bool has_point(point p, point q) {
        // p の x 座標 < q の x 座標となるように
        if (p.first > q.first) swap(p, q);
        auto [px, py] = p;
        auto [qx, qy] = q;
        int y_diff = 0;
        if (py < qy) y_diff = 1;
        else if (py > qy) y_diff = -1;
        int cy = py + y_diff;
        rep(x, px+1, qx) {
            if ( field[x][cy] ) return true;
            cy += y_diff;
        }
        return false;
    }

    // O(N log N)
    bool can_diagonally_connect(point p1, point p3, point p2, point p4) 
    {
        auto [x1, y1] = p1;
        auto [x2, y2] = p2;
        auto [x3, y3] = p3;
        auto [x4, y4] = p4;
        point cannot_connect_co = {-1, -1};

        // (x1, y1) には格子点がなく、他はあるか
        if (field[x1][y1]) 
            return false;
        if (!(field[x2][y2] and field[x3][y3] and field[x4][y4])) 
            return false;
        // 同じ辺を二度使用していないか
        if (is_side_used(p1, p3) or is_side_used(p3, p2) or is_side_used(p2, p4) or is_side_used(p4, p1)) 
            return false; 
        // 辺上に格子点がないか
        if (has_point(p1, p3) or has_point(p3, p2) or has_point(p2, p4) or has_point(p4, p1)) 
            return false;
        // つなげることができる
        return true;
    }

    // O(N log N)
    void diagonally_connect(point p1, point p3, point p2, point p4) {
        auto [x1, y1] = p1;
        auto [x2, y2] = p2;
        auto [x3, y3] = p3;
        auto [x4, y4] = p4;
        bool can_connect = can_diagonally_connect(p1, p3, p2, p4);
        if (can_connect) {
            field[x1][y1] = true;
            cnt++;
            connect(p1, p3);
            connect(p3, p2);
            connect(p2, p4);
            connect(p4, p1);
            res.emplace_back(x1, y1, x3, y3, x2, y2, x4, y4);
        }
    }

    bool has_same_point(point p1, point p2, point p3, point p4) {
        if (p1 == p2 or p1 == p3 or p3 == p4) return true;
        if (p2 == p3 or p2 == p4) return true;
        if (p3 == p4) return true;
        return false;
    }

    // O(N^4 log N)
    void diagonally_connect_action() {
        rep(x2, N) {
            rep(y2, N) {
                rep(t1, 1, MAX_DIAG + 1) {
                    rep(t2, 1, t1 + 1) {
                        fore (t1_dx, dx) {
                            fore (t1_dy, dy) {
                                fore (t2_dx, dx) {
                                    fore (t2_dy, dy) {
                                        if (t1_dx * t1_dy == t2_dx * t2_dy) continue;
                                        int x1 = x2 + t1 * t1_dx + t2 * t2_dx;
                                        int y1 = y2 + t1 * t1_dy + t2 * t2_dy;
                                        int x3 = x2 + t1 * t1_dx;
                                        int y3 = y2 + t1 * t1_dy;
                                        int x4 = x2 + t2 * t2_dx;
                                        int y4 = y2 + t2 * t2_dy;
                                        point p1 = {x1, y1};
                                        point p2 = {x2, y2};
                                        point p3 = {x3, y3};
                                        point p4 = {x4, y4};
                                        if (!has_same_point(p1, p2, p3, p4)) {
                                            if (pinc(p1, 0, N) and pinc(p2, 0, N) and pinc(p3, 0, N) and pinc(p4, 0, N)) {
                                                // p1 から時計 or 半時計になるように決めておく
                                                diagonally_connect(p1, p3, p2, p4);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void connect_action() {
        while (1) {
            if (1.0 * (clock() - start_time) / CLOCKS_PER_SEC > STOP_TIME) return;
            straightly_connect_action();
            straightly_connect_action();
            diagonally_connect_action();
            diagonally_connect_action();
        }
    }

    vector<vector<bool>> ret_field() {
        return field;
    }

    vector<ConnectAction> ret_ans() { 
        connect_action();
        return res; 
    }
};

// -----------------------------------------------

pair<vector<vector<bool>>, int> field_input() {
    int N, M;
    cin >> N >> M;
    vector<vector<bool>> field(N, vector<bool>(N));
    rep (square, M) {
        int x, y;
        cin >> x >> y;
        field[x][y] = true;
    }
    return make_pair(field, M);
}

void print_answer(vector<ConnectAction> &res) {
    cout << res.size() << "\n";
    for (auto& [x1, y1, x2, y2, x3, y3, x4, y4] : res) {
        cout << x1 << " " << y1 << " " << x2 << " " << y2 << " "
        << x3 << " " << y3 << " " << x4 << " " << y4 << "\n";
    }
}

// -----------------------------------------------

ll w(ll x, ll y, ll c) { return (x - c) * (x - c) + (y - c) * (y - c) + 1; }

ll print_score(int N, int M, vector<vector<bool>>& pre_field, vector<vector<bool>>& new_field)
{
    ll before = 0, after = 0;
    ll c = (N - 1) / 2;
    rep(x, N) {
        rep(y, N) {
            before += w(x, y, c);
            after += new_field[x][y] * w(x, y, c);
        }
    }
    return (1000000ll * N * N * after) / (M * before);
}

// -----------------------------------------------

void solve() {
    auto [field, M] = field_input();
    start_time = clock();
    int N = field.size();
    Solver solver(N, field);
    vector<ConnectAction> ans = solver.ret_ans();
    vector<vector<bool>> new_field = solver.ret_field();
    // print_answer(ans);
    ll score = print_score(N, M, field, new_field);
    total_score += score;
    cerr << score << "\n";
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    cerr << "test at draft7\nTL: " << STOP_TIME << " s\nTESTCASES: " << TESTCASES << "\n";
    rep(ii, TESTCASES) {
        solve();
    }
    cerr << "\n";
    cerr << total_score * (50 / TESTCASES) << "\n";
}