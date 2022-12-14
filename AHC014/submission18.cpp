#pragma GCC optimize ("O3")
#pragma GCC optimize("unroll-loops")
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
#include <ctime>

// 小さい四角形を各マスに当てはめてみて、次に大きい四角形に行く
// 31403768

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

int straight_t1_dx[] = {1, -1, -1, 1};
int straight_t2_dy[] = {1, 1, -1, -1};

int diagonal_t1_dx[] = {1, -1, -1, 1};
int diagonal_t1_dy[] = {-1, 1, 1, -1};
int diagonal_t2_dx[] = {1, 1, -1, -1};
int diagonal_t2_dy[] = {1, 1, -1, -1};

const int v_size = 144;
int t1_v[] = {1, 1, 2, 2, 2, 3, 3, 1, 3, 3, 4, 2, 4, 4, 1, 4, 4, 5, 3, 5, 5, 2, 5, 4, 5, 6, 6, 3, 6, 6, 1, 5, 5, 7, 2, 6, 6, 7, 4, 7, 7, 3, 7, 1, 6, 6, 8, 5, 7, 8, 8, 2, 4, 7, 8, 8, 7, 9, 6, 9, 3, 8, 8, 9, 5, 9, 1, 7, 9, 4, 9, 2, 7, 8, 8, 10, 10, 6, 9, 10, 10, 3, 9, 5, 10, 10, 1, 8, 8, 11, 9, 11, 7, 11, 4, 10, 10, 11, 2, 6, 9, 11, 11, 9, 12, 5, 11, 3, 8, 10, 10, 12, 12, 7, 11, 12, 12, 1, 9, 4, 11, 6, 12, 12, 2, 10, 5, 12, 3, 11, 1, 10, 4, 12, 2, 11, 3, 12, 1, 11, 2, 12, 1, 12};
int t2_v[] = {1, 2, 1, 2, 3, 2, 3, 3, 1, 4, 3, 4, 2, 4, 4, 1, 5, 4, 5, 3, 5, 5, 2, 6, 6, 4, 5, 6, 3, 6, 5, 1, 7, 5, 6, 2, 7, 6, 7, 4, 7, 7, 3, 6, 1, 8, 6, 8, 8, 5, 7, 7, 8, 2, 4, 8, 9, 7, 9, 6, 8, 3, 9, 8, 9, 5, 7, 1, 9, 9, 4, 8, 10, 2, 10, 7, 8, 10, 10, 6, 9, 9, 3, 10, 5, 10, 8, 1, 11, 8, 11, 9, 11, 7, 10, 4, 11, 10, 9, 11, 2, 6, 11, 12, 9, 11, 5, 10, 12, 3, 12, 8, 10, 12, 12, 7, 11, 9, 1, 11, 4, 12, 6, 12, 10, 2, 12, 5, 11, 3, 10, 1, 12, 4, 11, 2, 12, 3, 11, 1, 12, 2, 12, 1};

int start_time;
// 最後はここ 4.5 とかにしたい
const double STOP_TIME = 4.7;
const int TESTCASES = 50;
ll total_score = 0ll;

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
    vector<point> coordinates;
    Solver(int N, vector<vector<bool>> &field) : N(N), field(field), used(N, vector<bool>(N)) {
        rep(x, N) {
            rep(y, N) {
                if (field[x][y]) {
                    coordinates.push_back({x, y});
                }
            }
        }
    }

    int cnt = 0;
    // 斜め方向に長方形を作る際のパラメータ上限
    const int MAX_DIAG = 12;
    // 水平, 垂直方向に長方形を作る際のパラメータ上限
    const int MAX_STRAIGHT = 12;

    int cnvt(int i, int j) { return i * N + j; }
    bool has_same_point(point p1, point p2, point p3, point p4) 
    {
        if (p1 == p2 or p1 == p3 or p1 == p4) return true;
        if (p2 == p3 or p2 == p4) return true;
        if (p3 == p4) return true;
        return false;
    }
    
    // 隣接する p, q 間に辺が存在するかを返す O(log N)
    bool is_connected_adjacent_points(int px, int py, int qx, int qy) 
    {
        return ( 
            is_connected_mp.find({ cnvt(px, py), cnvt(qx, qy) }) != is_connected_mp.end() or 
            is_connected_mp.find({ cnvt(qx, qy), cnvt(px, py) }) != is_connected_mp.end() 
        );
    }

    // 隣接する p, q 間に辺を作成 O(log N)
    void connect_adjacent_points(int px, int py, int qx, int qy) 
    {
        is_connected_mp[{ cnvt(px, py), cnvt(qx, qy) }] = true;
    }

    // p の x 座標 < q の x 座標, x 座標が同じなら p の y 座標 < q の y 座標
    pair<point, point> point_swap(point p, point q)
    {
        if (p.first > q.first) swap(p, q);
        else if (p.first == q.first and p.second > q.second) swap(p, q);
        return {p, q};
    }

    // 点 p, q 間の任意の 2 点に既に辺が存在するかを返す O(N log N)
    bool is_connected(point p, point q) 
    {
        auto [p1, q1] = point_swap(p, q);
        auto [px, py] = p1;
        auto [qx, qy] = q1;
        int y_diff = 0;
        if (py < qy) y_diff = 1;
        else if (py > qy) y_diff = -1;
        int cy = py;
        rep(x, px, qx) {
            if ( is_connected_adjacent_points(x, cy, x+1, cy+y_diff) ) return true;
            cy += y_diff;
        }
        if (px == qx) {
            rep(y, py, qy) {
                if ( is_connected_adjacent_points(px, y, px, y+1) ) return true;
            }
        }
        return false;
    }
    // p, q 間に辺を作成 O(N log N)
    void connect(point p, point q) 
    {
        auto [p1, q1] = point_swap(p, q);
        auto [px, py] = p1;
        auto [qx, qy] = q1;
        int y_diff = 0;
        if (py < qy) y_diff = 1;
        else if (py > qy) y_diff = -1;
        int cy = py;
        rep(x, px, qx) {
            connect_adjacent_points(x, cy, x+1, cy+y_diff);
            cy += y_diff;
        }
        if (px == qx) {
            rep(y, py, qy) {
                connect_adjacent_points(px, y, px, y+1);
            }
        }
    }

    // 点 p, q 間に格子点が存在するかを返す O(N)
    bool has_point(point p, point q) 
    {
        auto [p1, q1] = point_swap(p, q);
        auto [px, py] = p1;
        auto [qx, qy] = q1;
        int y_diff = 0;
        if (py < qy) y_diff = 1;
        else if (py > qy) y_diff = -1;
        int cy = py;
        rep(x, px+1, qx) {
            cy += y_diff;
            if ( field[x][cy] ) return true;
        }
        if (px == qx) {
            rep(y, py+1, qy) {
                if ( field[px][y] ) return true;
            }
        }
        return false;
    }

    // p1, p2, p3, p4 で長方形を作れるかを返す O(N log N)
    bool can_make_rect(point p1, point p3, point p2, point p4) 
    {
        auto [x1, y1] = p1;
        auto [x2, y2] = p2;
        auto [x3, y3] = p3;
        auto [x4, y4] = p4;
        // (x1, y1) には格子点がなく、他はあるか
        if (field[x1][y1]) 
            return false;
        if (!(field[x2][y2] and field[x3][y3] and field[x4][y4])) 
            return false;
        // 同じ辺を二度使用していないか
        if (is_connected(p1, p3) or is_connected(p3, p2) or is_connected(p2, p4) or is_connected(p4, p1)) 
            return false; 
        // 辺上に格子点がないか
        if (has_point(p1, p3) or has_point(p3, p2) or has_point(p2, p4) or has_point(p4, p1)) 
            return false;
        return true;
    }

    // 長方形を作る O(N log N)
    void make_rect(point p1, point p3, point p2, point p4) 
    {
        auto [x1, y1] = p1;
        auto [x2, y2] = p2;
        auto [x3, y3] = p3;
        auto [x4, y4] = p4;
        bool can_make_rect_flg = can_make_rect(p1, p3, p2, p4);
        if (can_make_rect_flg) {
            field[x1][y1] = true;
            connect(p1, p3);
            connect(p3, p2);
            connect(p2, p4);
            connect(p4, p1);
            coordinates.push_back({x1, y1});
            sort(all(coordinates));
            res.emplace_back(x1, y1, x3, y3, x2, y2, x4, y4);
        }
    }

    tuple<point, point, point, point> make_points(int x2, int y2, int t1, int t2, int t1_dx, int t2_dx, int t1_dy, int t2_dy, int mode)
    {
        tuple<point, point, point, point> ret;
        point p1, p2, p3, p4;
        if (mode == 0) { // straightly_make_rect
            int x1 = x2 + t1 * t1_dx;
            int y1 = y2 + t2 * t2_dy;
            int x3 = x1;
            int y3 = y2;
            int x4 = x2;
            int y4 = y1;
            p1 = {x1, y1};
            p2 = {x2, y2};
            p3 = {x3, y3};
            p4 = {x4, y4};
            // p1 から時計 or 反時計周りになるように決めておく
            if (!( !has_same_point(p1, p2, p3, p4) and pinc(p1, 0, N) and pinc(p2, 0, N) and pinc(p3, 0, N) and pinc(p4, 0, N) )) {
                p1 = {-1, -1};
            }
        } else { // diagonnaly_make_rect
            int x1 = x2 + t1 * t1_dx + t2 * t2_dx;
            int y1 = y2 + t1 * t1_dy + t2 * t2_dy;
            int x3 = x2 + t1 * t1_dx;
            int y3 = y2 + t1 * t1_dy;
            int x4 = x2 + t2 * t2_dx;
            int y4 = y2 + t2 * t2_dy;
            p1 = {x1, y1};
            p2 = {x2, y2};
            p3 = {x3, y3};
            p4 = {x4, y4};
            if ( !( !has_same_point(p1, p2, p3, p4) and 
            pinc(p1, 0, N) and pinc(p2, 0, N) and pinc(p3, 0, N) and pinc(p4, 0, N) and t1_dx * t1_dy != t2_dx * t2_dy) ) {
                p1 = {-1, -1};
            }
        }
        // cerr << p1.first << " " << p1.second << "\n";
        ret = make_tuple(p1, p3, p2, p4);
        return ret;
    }

    // 軸と垂直に長方形を作る O(N^3 log N) 定数倍重い
    void straightly_make_rect_action(int x2, int y2, int t1, int t2, int t1_dx, int t2_dx, int t1_dy, int t2_dy)
    {
        auto [p1, p3, p2, p4] = make_points(x2, y2, t1, t2, t1_dx, t2_dx, t1_dy, t2_dy, 0);
        if (p1 != make_pair(-1, -1)) {
            // p1 から時計 or 反時計周りになるように決めておく
            make_rect(p1, p3, p2, p4);
        }
    }

    // 45 度傾いた長方形を作る O(N^3 log N) 定数倍重い
    void diagonally_make_rect_action(int x2, int y2, int t1, int t2, int t1_dx, int t2_dx, int t1_dy, int t2_dy) 
    {
        auto [p1, p3, p2, p4] = make_points(x2, y2, t1, t2, t1_dx, t2_dx, t1_dy, t2_dy, 1);
        if (p1 != make_pair(-1, -1)) {
            make_rect(p1, p3, p2, p4);
        }
    }

    void make_rect_action() 
    {
        while (1) {
            if (1.0 * (clock() - start_time) / CLOCKS_PER_SEC > STOP_TIME) return;
            rep(ts, v_size) {
                int t1 = t1_v[ts], t2 = t2_v[ts];
                rep(c_cnt, 3) {
                    int sz = coordinates.size();
                    rep(co, sz) {
                        int x2 = coordinates[co].first;
                        int y2 = coordinates[co].second;
                        rep(i, 4) {
                            int t1_dx = straight_t1_dx[i];
                            int t1_dy = straight_t2_dy[i];
                            int t2_dx = diagonal_t2_dx[i];
                            int t2_dy = diagonal_t2_dy[i];
                            straightly_make_rect_action(
                                x2, y2, t1, t2, t1_dx, t2_dx, t1_dy, t2_dy
                            );
                            if (coordinates.size() > sz) sz++;
                            t1_dx = diagonal_t1_dx[i];
                            t1_dy = diagonal_t1_dy[i];
                            diagonally_make_rect_action(
                                x2, y2, t1, t2, t1_dx, t2_dx, t1_dy, t2_dy
                            );
                            if (coordinates.size() > sz) sz++;
                        }
                    }
                }
            } 
        }
    }

    vector<vector<bool>> ret_field() { return field; }

    vector<ConnectAction> ret_ans() 
    { 
        make_rect_action();
        return res; 
    }
};

// -----------------------------------------------

pair<vector<vector<bool>>, int> field_input() 
{
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

void print_answer(vector<ConnectAction> &res) 
{
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
    ll ans = (1000000ll * N * N * after) / (M * before);
    return ans;
}

// -----------------------------------------------

void solve() {
    auto [field, M] = field_input();
    start_time = clock();
    int N = field.size();
    Solver solver(N, field);
    vector<ConnectAction> ans = solver.ret_ans();
    vector<vector<bool>> new_field = solver.ret_field();
    print_answer(ans);
    // ll score = print_score(N, M, field, new_field);
    // cerr << "\nscore: " << score << "\n";
}

int main(int argc, char* argv[]) {
    cin.tie(0);
    ios::sync_with_stdio(false);

    solve();
}