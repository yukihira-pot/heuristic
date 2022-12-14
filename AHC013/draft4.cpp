#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <iomanip>
#include <limits>
#include <bitset>
#include <cassert>
#include <random>
#include <utility>

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
#define out(x) {cout << x << "\n";}
#define dbg(x) {cerr << #x << ": " << x << "\n";}
#define inc(x, l, r) ((l) <= (x) and (x) < (r))

const int INFI = 1 << 30;
const ll INFL = 1LL << 60;
// const ll MOD = 1000000007; // 1e9 + 7
// const ll MOD = 998244353;

template<class T>bool chmax(T& a, const T& b){if(a<b){a=b;return 1;}return 0;}
template<class T>bool chmin(T& a, const T& b){if(b<a){a=b;return 1;}return 0;}
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

using Field = vector<vector<char>>;
int operation_counter_original, move_counter_original, connect_counter_original;
int operation_counter, move_counter, connect_counter;

struct UnionFind {
    vector<int> par, rank, siz;

    UnionFind(int N) : par(N, -1), rank(N, 0), siz(N, 1) {}

    int root(int x) {
        if (par[x] == -1) return x;
        return par[x] = root(par[x]);
    }

    void unite(int x, int y) {
        int rx = root(x);
        int ry = root(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) swap(rx, ry);
        par[ry] = rx;
        if (rank[rx] == rank[ry]) rank[rx]++;
        siz[rx] += siz[ry];
    }

    bool same(int x, int y) {
        return root(x) == root(y);
    }

    int size(int x) {
        return siz[root(x)];
    }
};

vector<vector<char>> generate_partition_vecs(int K) {
    vector<vector<char>> res;
    vector<char> tmp;
    rep(i, K) tmp.pb((char)('1' + i));
    
    do {
        res.pb(tmp);
    } while (next_permutation(all(tmp)));

    return res;
}

vector<pii> genarate_partition_ranges(int N, int K) {
    vector<pii> res;
    rep(i, K) {
        res.pb({i * (N / K) + 1, (i + 1) * (N / K)});
    }
    res[0].first--;
    if (res.back().second != N) {
        res.back().second++;
    }

    return res;
}

map<char, int> generate_convert_partition_to_index(vector<char> &partition_vec) {
    map<char, int> res;
    rep(i, partition_vec.size()) {
        res[partition_vec[i]] = i;
    }

    return res;
}

bool can_move(int N, Field &field, int cx, int cy, int nx, int ny) {
    if (inc(nx, 0, N) and inc(ny, 0, N)) {
        return field[nx][ny] == '0';
    }
    return false;
}


vector<pair<pii, pii>> field_move_impl(int N, int K, Field &field, int partition_idx, vector<pair<pii, pii>> &output_move) {

    auto partition_vecs = generate_partition_vecs(K);
    // partition の順番 ex) {'1', '3', '2', '4'}
    vector<char> partition_vec = partition_vecs[partition_idx]; 
    // partition の範囲 { (0, N / K), (N / K + 1, 2 * N / K), ... , }
    auto partition_ranges = genarate_partition_ranges(N, K);
    // partition[i] を index に convert ex) { '1': 0, '3': 1, '2': 2, '4': 3 }
    map<char, int> convert_partition_to_index = generate_convert_partition_to_index(partition_vec);
    // 動ける限界

    rep(i, K) {
        int move_range_left = partition_ranges[i].first;
        int move_range_right = partition_ranges[i].second;
        int not_move_range_left = partition_ranges[i].first;
        int not_move_range_right = partition_ranges[i].second;
        if (i != 0) move_range_left = partition_ranges[i - 1].first;
        if (i != K - 1) move_range_right = partition_ranges[i + 1].second;

        rep(i_itr, N) {
            rep(j_itr, N) {
                int cx = i_itr;
                int cy = j_itr;
                if (field[cx][cy] == partition_vec[i]) {
                    if (inc(cy, move_range_left, move_range_right) and !inc(cy, not_move_range_left, not_move_range_right)) {
                        // 動かしたいとき
                        int up_down_counter = 2;
                        // 2 回まで上下に行けるようにする
                        if (move_range_left <= cy and cy < not_move_range_left) {
                            // 右に行きたいとき
                            while (move_counter > 0 and up_down_counter >= 0 and cy < not_move_range_left) {
                                int nx = cx;
                                int ny = cy + 1;
                                if (can_move(N, field, cx, cy, nx, ny)) {
                                    // field 移動
                                    swap(field[cx][cy], field[nx][ny]);
                                    // 記録
                                    output_move.push_back({{cx, cy}, {nx, ny}});
                                    // カウンター減らす
                                    move_counter--;
                                    // 座標移動
                                    cx = nx;
                                    cy = ny;
                                }
                                else {
                                    int cand_destination = cx + (i % 2 ? 1 : -1);
                                    if (cx == 0) cand_destination = 1;
                                    else if (cx == N-1) cand_destination = N-2;
                                    if (inc(ny, 0, N) and inc(cand_destination, 0, N) and field[cand_destination][cy] == '0') {
                                        swap(field[cx][cy], field[cand_destination][cy]);
                                        output_move.push_back({{cx, cy}, {cand_destination, cy}});
                                        move_counter--;
                                        up_down_counter--;
                                        cx = cand_destination;
                                    }
                                    else {
                                        break;
                                    }
                                }
                            }
                        }
                        else if (not_move_range_right < cy and cy <= move_range_right) {
                            // 左に行きたいとき
                            while (move_counter > 0 and up_down_counter >= 0 and not_move_range_right < cy) {
                                int nx = cx;
                                int ny = cy - 1;
                                if (can_move(N, field, cx, cy, nx, ny)) {
                                    swap(field[cx][cy], field[nx][ny]);
                                    output_move.push_back({{cx, cy}, {nx, ny}});
                                    move_counter--;
                                    cx = nx;
                                    cy = ny;
                                }
                                else {
                                    int cand_destination = cx + (i % 2 ? 1 : -1);
                                    if (cx == 0) cand_destination = 1;
                                    else if (cx == N-1) cand_destination = N-2;
                                    if (inc(ny, 0, N) and inc(cand_destination, 0, N) and field[cand_destination][cy] == '0') {
                                        swap(field[cx][cy], field[cand_destination][cy]);
                                        output_move.push_back({{cx, cy}, {cand_destination, cy}});
                                        move_counter--;
                                        up_down_counter--;
                                        cx = cand_destination;
                                    }
                                    else {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return output_move;
}

vector<pair<pii, pii>> field_gather(int N, int K, Field &field, int partition_idx, vector<pair<pii, pii>> &output_move) {
    // 各 partition ごとに端に集める
    auto partition_vecs = generate_partition_vecs(K);
    // partition の順番 ex) {'1', '3', '2', '4'}
    vector<char> partition_vec = partition_vecs[partition_idx]; 
    // partition の範囲 { (0, N / K), (N / K + 1, 2 * N / K), ... , }
    auto partition_ranges = genarate_partition_ranges(N, K);
    // cout << "move counter" << move_counter << "\n";
    if (move_counter > 0) {
        rep(idx, K) {
            int not_move_range_left = partition_ranges[idx].first;
            rep(j, N) {
                rep(i, N) {
                    int cx = i;
                    int cy = j;
                    if (field[cx][cy] == partition_vec[idx] and cy > not_move_range_left) {
                        while (cy > not_move_range_left and move_counter > 0 ) {
                            int nx = cx;
                            int ny = cy - 1;
                            if (can_move(N, field, cx, cy, nx, ny)) {
                                swap(field[cx][cy], field[nx][ny]);
                                output_move.push_back({{cx, cy}, {nx, ny}});
                                move_counter--;
                                cx = nx;
                                cy = ny;
                            }
                            else {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return output_move;
}

vector<pair<pii, pii>> field_move(int N, int K, Field &field, int partition_idx, vector<pair<pii, pii>> &output_move) {
    auto tmp1 = field_move_impl(N, K, field, partition_idx, output_move);
    auto tmp2 = field_gather(N, K, field, partition_idx, tmp1);

    return tmp2;
}

vector<pair<pii, pii>> field_connect(int N, int K, Field &field, int partition_idx, vector<pair<pii, pii>> &output_connect) {
    /* 各コンピュータから右と下に向かって接続 */ 
    auto partition_vecs = generate_partition_vecs(K);
    // partition の順番 ex) {'1', '3', '2', '4'}
    vector<char> partition_vec = partition_vecs[partition_idx]; 
    // partition の範囲 { (0, N / K), (N / K + 1, 2 * N / K), ... , }
    auto partition_ranges = genarate_partition_ranges(N, K);

    UnionFind UF(N * N);
    vector<vector<bool>> used(N, vector<bool>(N, false));
    
    connect_counter = connect_counter_original + move_counter;

    auto convert_2d_to_1d = [&](int cx, int cy) {
        return N * cx + cy;
    };

    auto can_connect = [&](int cx, int cy, int nx, int ny) {
        if (connect_counter <= 0) return false;
        if (!inc(cx, 0, N) or !inc(cy, 0, N) or !inc(nx, 0, N) or !inc(ny, 0, N)) return false;
        // ①選んだ2個のコンピュータは同じ行か同じ列に存在し、間に他のコンピュータが存在しない
        // ②ケーブルは他のケーブルと交差しない。（あるコンピュータに複数の方向からケーブルを接続することは可能）
        if (field[cx][cy] == '0' or field[nx][ny] == '0') return false;
        if (field[cx][cy] != field[nx][ny]) return false;
        if (cx == nx) {
            // 横方向 (右方向)
            rep(ty, min(cy, ny)+1, max(cy, ny)) {
                if (used[cx][ty] or field[cx][ty] != '0') return false;
            }
        }
        else if (cy == ny) {
            // 縦方向 (下方向)
            rep(tx, min(cx, nx)+1, max(cx, nx)) {
                if (used[tx][cy] or field[tx][cy] != '0') return false;
            }
        }
        else return false; // 右でも下でもない

        // ③既にケーブルで直接つながっているコンピュータ同士を再び指定することはできない。
        if (UF.same(convert_2d_to_1d(cx, cy), convert_2d_to_1d(nx, ny))) return false;
        // ④あるコンピュータの接続先として、そのコンピュータ自身を指定することはできない。
        if (cx == nx and cy == ny) return false;

        return true;
    };

    auto connect = [&](int cx, int cy, int nx, int ny) {
        int cp = convert_2d_to_1d(cx, cy);
        int np = convert_2d_to_1d(nx, ny);
        if (cx == nx) {
            // 横方向 (右方向)
            rep(ty, min(cy, ny)+1, max(cy, ny)) {
                used[cx][ty] = true;
            }
            UF.unite(cp, np);
        }
        else if (cy == ny) {
            // 縦方向 (下方向)
            rep(tx, min(cx, nx)+1, max(cx, nx)) {
                used[tx][cy] = true;
            }
            UF.unite(cp, np);
        }
    };

    rep(idx, K) {
        int not_move_range_left = partition_ranges[idx].first;
        int not_move_range_right = partition_ranges[idx].second;
        rep(i1, N) {
            rep(j1, N) {
                // 右
                rep(j2, j1, N) {
                    if (can_connect(i1, j1, i1, j2)) {
                        output_connect.push_back({{i1, j1}, {i1, j2}});
                        connect(i1, j1, i1, j2);
                        connect_counter--;
                    }
                }

                // 下
                rep(i2, i1, N) {
                    if (can_connect(i1, j1, i2, j1)) {
                        output_connect.push_back({{i1, j1}, {i2, j1}});
                        connect(i1, j1, i2, j1);
                        connect_counter--;
                    }
                }
            }
        }
    }

    return output_connect;
}


int calc_score(int N, int K, Field &field, vector<pair<pii, pii>> &output_move, vector<pair<pii, pii>> &output_connect) {
    auto convert_2d_to_1d = [&](int cx, int cy) {
        return N * cx + cy;
    };

    auto convert_1d_to_2d = [&](int p) {
        int x = p / N;
        int y = p % N;
        pii res = make_pair(x, y);
        return res;
    };

    UnionFind UF(N * N);

    fore (e, output_connect) {
        pii p1_c = e.first;
        pii p2_c = e.second;
        int p1 = convert_2d_to_1d(p1_c.first, p1_c.second);
        int p2 = convert_2d_to_1d(p2_c.first, p2_c.second);
        UF.unite(p1, p2);
    }

    vector<pair<int, int>> computers;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] != '0') {
                computers.emplace_back(i, j);
            }
        }
    }

    int score = 0;
    for (int i = 0; i < (int)computers.size(); i++) {
        for (int j = i+1; j < (int)computers.size(); j++) {
            auto c1 = computers[i];
            auto c2 = computers[j];
            int p1 = convert_2d_to_1d(c1.first, c1.second);
            int p2 = convert_2d_to_1d(c2.first, c2.second);
            if (UF.same(p1, p2)) {
                score += (field[c1.first][c1.second] == field[c2.first][c2.second]) ? 1 : -1;
            }
        }
    }

    return max(score, 0);
}

tuple<int, vector<pair<pii, pii>>, vector<pair<pii, pii>>> my_solve(int N, int K, Field &field, int partition_idx) {
    vector<pair<pii, pii>> res1, res2;
    vector<pair<pii, pii>> output_move = field_move(N, K, field, partition_idx, res1);
    vector<pair<pii, pii>> output_connect = field_connect(N, K, field, partition_idx, res2);
    int score = (-1) * calc_score(N, K, field, output_move, output_connect);
    auto ret = make_tuple(score, output_move, output_connect);

    return ret;
}

// ----------------------------------
struct MoveAction {
    int before_row, before_col, after_row, after_col;
    MoveAction(int before_row, int before_col, int after_row, int after_col) : 
        before_row(before_row), before_col(before_col), after_row(after_row), after_col(after_col) {}
};

struct ConnectAction {
    int c1_row, c1_col, c2_row, c2_col;
    ConnectAction(int c1_row, int c1_col, int c2_row, int c2_col) : 
        c1_row(c1_row), c1_col(c1_col), c2_row(c2_row), c2_col(c2_col) {}
};

struct Result {
    vector<MoveAction> move;
    vector<ConnectAction> connect;
    Result(const vector<MoveAction> &move, const vector<ConnectAction> &con) : move(move), connect(con) {}
};

struct Solver {
    static constexpr char USED = 'x';
    static constexpr int DR[4] = {0, 1, 0, -1};
    static constexpr int DC[4] = {1, 0, -1, 0};

    int N, K;
    int action_count_limit;
    mt19937 engine;
    vector<string> field;

    Solver(int N, int K, const vector<string> &field, int seed = 0) : 
        N(N), K(K), action_count_limit(K * 100), field(field)
    {
        engine.seed(seed);
    }

    bool can_move(int row, int col, int dir) const
    {
        int nrow = row + DR[dir];
        int ncol = col + DC[dir];
        if (0 <= nrow && nrow < N && 0 <= ncol && ncol < N) {
            return field[nrow][ncol] == '0';
        }
        return false;
    }

    vector<MoveAction> move(int move_limit = -1)
    {
        vector<MoveAction> ret;
        if (move_limit == -1) {
            move_limit = K * 50;
        }

        for (int i = 0; i < move_limit; i++) {
            int row = engine() % N;
            int col = engine() % N;
            int dir = engine() % 4;
            if (field[row][col] != '0' && can_move(row, col, dir)) {
                swap(field[row][col], field[row + DR[dir]][col + DC[dir]]);
                ret.emplace_back(row, col, row + DR[dir], col + DC[dir]);
                action_count_limit--;
            }
        }

        return ret;
    }

    bool can_connect(int row, int col, int dir) const
    {
        int nrow = row + DR[dir];
        int ncol = col + DC[dir];
        while (0 <= nrow && nrow < N && 0 <= ncol && ncol < N) {
            if (field[nrow][ncol] == field[row][col]) {
                return true;
            } else if (field[nrow][ncol] != '0') {
                return false;
            }
            nrow += DR[dir];
            ncol += DC[dir];
        }
        return false;
    }

    ConnectAction line_fill(int row, int col, int dir)
    {
        int nrow = row + DR[dir];
        int ncol = col + DC[dir];
        while (0 <= nrow && nrow < N && 0 <= ncol && ncol < N) {
            if (field[nrow][ncol] == field[row][col]) {
                return ConnectAction(row, col, nrow, ncol);
            }
            assert(field[nrow][ncol] == '0');
            field[nrow][ncol] = USED;
            nrow += DR[dir];
            ncol += DC[dir];
        }
        assert(false);
    }

    vector<ConnectAction> connect()
    {
        vector<ConnectAction> ret;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] != '0' && field[i][j] != 'x') {
                    for (int dir = 0; dir < 2; dir++) {
                        if (can_connect(i, j, dir)) {
                            ret.push_back(line_fill(i, j, dir));
                            action_count_limit--;
                            if (action_count_limit <= 0) {
                                return ret;
                            }
                        }
                    }
                }
            }
        }
        return ret;
    }

    Result solve()
    {
        // create random moves
        auto moves = move();
        // from each computer, connect to right and/or bottom if it will reach the same type
        auto connects = connect();
        return Result(moves, connects);
    }
};

struct UnionFind2 {
    map<pair<int,int>, pair<int, int>> parent;
    UnionFind2() :parent() {}

    pair<int, int> find(pair<int, int> x)
    {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            return x;
        } else if (parent[x] == x) {
            return x;
        } else {
            parent[x] = find(parent[x]);
            return parent[x];
        }
    }

    void unite(pair<int, int> x, pair<int, int> y)
    {
        x = find(x);
        y = find(y);
        if (x != y) {
            parent[x] = y;
        }
    }
};

int calc_score2(int N, vector<string> field, const Result &res)
{
    for (auto r : res.move) {
        assert(field[r.before_row][r.before_col] != '0');
        assert(field[r.after_row][r.after_col] == '0');
        swap(field[r.before_row][r.before_col], field[r.after_row][r.after_col]);
    }

    UnionFind2 uf;
    for (auto r : res.connect) {
        pair<int, int> p1(r.c1_row, r.c1_col), p2(r.c2_row, r.c2_col);
        uf.unite(p1, p2);
    }

    vector<pair<int, int>> computers;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] != '0') {
                computers.emplace_back(i, j);
            }
        }
    }

    int score = 0;
    for (int i = 0; i < (int)computers.size(); i++) {
        for (int j = i+1; j < (int)computers.size(); j++) {
            auto c1 = computers[i];
            auto c2 = computers[j];
            if (uf.find(c1) == uf.find(c2)) {
                score += (field[c1.first][c1.second] == field[c2.first][c2.second]) ? 1 : -1;
            }
        }
    }

    return max(score, 0);
}

void print_answer(const Result &res)
{
    cerr << "size: " << res.move.size() + res.connect.size() << "\n";
    cout << res.move.size() << endl;
    for (auto m : res.move) {
        cout << m.before_row << " " << m.before_col << " "
            << m.after_row << " " << m.after_col << endl;
    }
    cout << res.connect.size() << endl;
    for (auto m : res.connect) {
        cout << m.c1_row << " " << m.c1_col << " "
            << m.c2_row << " " << m.c2_col << endl;
    }
}

// --------------------------------------------

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    int N, K; cin >> N >> K;
    vector<string> original_field(N);
    rep(i, N) cin >> original_field[i];
    Field field(N, vector<char>(N));

    rep(i, N) {
        rep(j, N) {
            field[i][j] = original_field[i][j];
        }
    }

    // -------------------

    auto my_factorial = [](int m) {
        int res = 1;
        for (int i = m; i >= 1; i--) {
            res *= i;
        }
        return res;
    };

    vector<tuple<int, vector<pair<pii, pii>>, vector<pair<pii, pii>>>> my_res;
    int rot = my_factorial(K);
    int incr = max(rot / 10, 1);
    for (int itr = 0; itr < rot; itr += incr) {
        for (int i = 1; i <= 21; i += 2) {
            Field field1 = field;

            operation_counter_original = 100 * K;
            move_counter_original = i * K;
            connect_counter_original = operation_counter_original - move_counter_original;

            move_counter = move_counter_original;
            operation_counter = operation_counter_original;

            auto res = my_solve(N, K, field1, 1);
            my_res.push_back(res);
        }
    }

    sort(all(my_res));
    int res1;
    vector<pair<pii, pii>> output_move, output_connect;
    tie(res1, output_move, output_connect) = my_res[0];
    res1 *= -1;

    //-----------------------

    Solver s(N, K, original_field);
    auto ret = s.solve();
    int res2 = calc_score2(N, original_field, ret);

    //------------------------

    cerr << "score (original solution): " << res1 << "\n";
    cerr << "score (sample solution): " << res2 << "\n";

    if (res1 > res2) {
        cerr << "original solution wins\n";
        cout << output_move.size() << "\n";
        fore (e, output_move) {
            cout << e.first.first << " " << e.first.second << " " << e.second.first << " " << e.second.second << "\n";
        }

        cout << output_connect.size() << "\n";
        fore (e, output_connect) {
            cout << e.first.first << " " << e.first.second << " " << e.second.first << " " << e.second.second << "\n";
        }
    }
    else {
        cerr << "sample solution wins\n";
        print_answer(ret);
    }

}