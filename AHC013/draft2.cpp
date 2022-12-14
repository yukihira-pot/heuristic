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
vector<pair<pii, pii>> output_move, output_connect;
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


void field_move(int N, int K, Field &field, int partition_idx) {

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
                            // cerr <<  cx << " " << cy << " " << "wanna move right\n";
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
}

void field_gather(int N, int K, Field &field, int partition_idx) {
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
}

void field_connect(int N, int K, Field &field, int partition_idx) {
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
}


int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    int N, K; cin >> N >> K;
    Field field(N, vector<char>(N));

    rep(i, N) rep(j, N) cin >> field[i][j];

    operation_counter_original = 100 * K;
    move_counter_original = 20 * K;
    connect_counter_original = operation_counter_original - move_counter_original;

    move_counter = move_counter_original;
    operation_counter = operation_counter_original;

    field_move(N, K, field, 0);
    field_gather(N, K, field, 0);
    field_connect(N, K, field, 0);

    dbg(output_move.size() + output_connect.size());

    cout << output_move.size() << "\n";
    fore (e, output_move) {
        cout << e.first.first << " " << e.first.second << " " << e.second.first << " " << e.second.second << "\n";
    }

    cout << output_connect.size() << "\n";
    fore (e, output_connect) {
        cout << e.first.first << " " << e.first.second << " " << e.second.first << " " << e.second.second << "\n";
    }
}