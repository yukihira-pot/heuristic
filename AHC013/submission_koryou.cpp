#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>
#include <list>
#include <cassert>
#include <numeric>
#include <cstdint>
#include <queue>
#include <deque>
#include <stack>
#include <set>
#include <random>
#include <bitset>
#include <utility>
// #include <atcoder/all>
 
using ll = long long;
using ld = long double;
using namespace std;
// using namespace atcoder;
using P = pair<ll, ll>;
using Graph = vector<vector<int>>;
using Priority = priority_queue<ll, vector<ll>, greater<ll>>;// 昇順
using Priority_pair = priority_queue<P, vector<P>, greater<P>>;
// using mint_17 = modint1000000007;
// using mint = modint998244353;

#define mod 1000000007
#define MAX_WIDTH 60
#define MAX_HEIGHT 60
#define INF 1e18
#define MOD 998244353
#define PI 3.141592653589793
#define rep(i, a, b) for(ll i=(a);i<(b);i++)
#define rrep(i, a, b) for(ll i=(a);i>=(b);i--)
#define fore(i, a) for(auto &i: a)
#define all(v) (v).begin(), (v).end()
#define YES(a) cout << ((a) ? "YES" : "NO") << endl
#define Yes(a) cout << ((a) ? "Yes" : "No") << endl
#define pb(a) push_back(a)
#define vi vector<int>
#define vll vector<ll>
#define vs vector<string>
#define vc vector<char>
#define vp vector<pair<ll, ll>>
#define mll map<ll, ll>
#define msl map<string, ll>
#define COUT(n) cout << (n) << endl
#define isInGrid(a, b, h, w) (0 <= (a) && (a) < (h) && 0 <= (b) && (b) < (w))
template<typename T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return true; } return false; }
template<typename T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return true; } return false; }
int vx[] = {1, 0, -1, 0}, vy[] = {0, 1, 0, -1};

struct UnionFind {
    map<pair<int,int>, pair<int, int>> parent;
    map<pair<int, int>, int> united;
    UnionFind() :parent() {}

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
        united[x] = true;
        united[y] = true;
        if (x != y) {
            parent[x] = y;
        }
    }

    bool same(pair<int, int> x, pair<int, int> y) {
        x = find(x);
        y = find(y);
        return x == y;
    }

    bool isUnite(pair<int, int> x){ return united[x]; }
};

char USED[30];
int dy[4] = {0, 1, 0, -1};
int dx[4] = {1, 0, -1, 0};
UnionFind uf;
bool is_end = false, is_move_end = false;
int move_count = 0;

struct MoveAction {
    int before_y, before_x, after_y, after_x;
    MoveAction(int before_y, int before_x, int after_y, int after_x) : 
        before_y(before_y), before_x(before_x), after_y(after_y), after_x(after_x) {}
};

struct ConnectAction {
    int c1_y, c1_x, c2_y, c2_x;
    ConnectAction(int c1_y, int c1_x, int c2_y, int c2_x) : 
        c1_y(c1_y), c1_x(c1_x), c2_y(c2_y), c2_x(c2_x) {}
};

struct Result {
    vector<MoveAction> move;
    vector<ConnectAction> connect;
    Result(const vector<MoveAction> &move, const vector<ConnectAction> &con) : move(move), connect(con) {}
};

vector<MoveAction> move_ans;
vector<ConnectAction> connect_ans;

struct Solver {
    int N, K;
    int action_count_limit;
    mt19937 engine;
    vector<string> field;

    Solver(int N, int K, const vector<string> &field, int seed = 0) : 
        N(N), K(K), action_count_limit(K * 100), field(field)
    {
        engine.seed(seed);
    }

    bool can_move(int y, int x, int dir) const
    {
        int ny = y + dy[dir];
        int nx = x + dx[dir];
        if (isInGrid(ny, nx, N, N)) {
            return field[ny][nx] == '0';
        }
        return false;
    }

    void init_connect(int k_num) {
        rep(y,0,N) {
            rep(x,0,N) {
                rep(dir, 0, 4) {
                    if(!is_end && field[y][x] == (char)('0'+k_num)) {
                        ll ny = y + dy[dir], nx = x + dx[dir];

                        rep(i,0,N) {// 初期状態で行列が同じブロックを接続
                            if(!isInGrid(ny, nx, N, N)) break;

                            if(field[ny][nx] == field[y][x]) {
                                ConnectAction res = line_fill(y, x, dir);
                                if(res.c1_y != -1) {
                                    // connect_ans.push_back(res);
                                    // action_count_limit--;
                                    if(action_count_limit <= 0) {
                                        is_end = true;
                                    }
                                }
                                break;
                            } else if(field[ny][nx] != '0') {
                                break;
                            }
                            ny += dy[dir];
                            nx += dx[dir];

                            if(is_end) break;
                        }
                    }
                }
            }
        }
        return;
    }

    void final_connect(int k_num) {
        rep(y,0,N) {
            rep(x,0,N) {
                if(!is_end && field[y][x] == (char)('0'+k_num)) {
                    rep(dir, 0, 4) {
                        ll ny = y + dy[dir], nx = x + dx[dir];
                        if(is_end) continue;

                        rep(i,0,N) {// 初期状態で行列が同じブロックを接続
                            if(!isInGrid(ny, nx, N, N)) break;

                            if(field[ny][nx] == field[y][x]) {
                                ConnectAction res = line_fill(y, x, dir);
                                // if(k_num == 1) {
                                //     cout << y << " " << x << " " << dir << endl;
                                // }
                                if(res.c1_y != -1) {
                                    connect_ans.push_back(res);
                                    action_count_limit--;
                                    if(action_count_limit <= 0) {
                                        is_end = true;
                                    }
                                }
                                break;
                            } else if(field[ny][nx] != '0') {
                                break;
                            }
                            ny += dy[dir];
                            nx += dx[dir];

                            if(is_end) break;
                        }
                    }
                }
            }
        }
        return;
    }

    bool is_edge_block(int y, int x) {
        int cnt = 0;
        rep(dir, 0, 4) {
            if(can_connect(y, x, dir, N)) cnt++;
        }

        return (cnt == 1 || cnt == 0);
    }

    int calc_dir(int y, int x) {
        rep(dir, 0, 4) {
            if(can_connect(y, x, dir, N)) {
                return dir;
            }
        }
        return -1;
    }

    // P bfs(int Y, int X) {
    //     queue<P> que;
    //     bool reached[N+1][N+1];
    //     que.push({Y, X});
    //     reached[Y][X] = true;

    //     while(!que.empty()) {
    //         P p = que.front();
    //         que.pop();
    //         int y = p.first, x = p.second;
    //         rep(i, 0, 4) {
    //             int ny = y+dy[i], nx = x+dx[i];
    //             if(!isInGrid(ny, nx, N, N)) continue;
    //             if(field[ny][nx] != '0') continue;
    //             if(reached[ny][nx]) continue;
    //             reached[ny][nx] = true;

    //             if(field[ny][nx] == field[Y][X] && !uf.same(make_pair(Y, X), make_pair(ny, nx))) {
    //                 return make_pair(ny, nx);
    //             }

    //             que.push({ny, nx});
    //         }
    //     }

    //     return make_pair(-1, -1);
    // }

    P search_rev_dir(int y, int x, int dir, int base_y, int base_x) {
        rep(rev_dir, 0, 4) {
            if(rev_dir%2 == dir%2) continue;
            int ny = y+dy[rev_dir], nx = x+dx[rev_dir];
            rep(i,0,N) {
                if(!isInGrid(ny, nx, N, N)) break;
                if(field[ny][nx] == field[base_y][base_x]) {
                    return make_pair(ny, nx);
                }

                if(field[ny][nx] != '0' && field[ny][nx] != USED[field[base_y][base_x]-'0']) break;
                ny += dy[rev_dir];
                nx += dx[rev_dir];
            }
        }

        return make_pair(-1, -1);
    }

    pair<P, P> search_near_block(int y, int x, int dir) {
        int ny = y+dy[dir], nx = x+dx[dir];
        rep(i, 0, N) {
            if(!isInGrid(ny, nx, N, N)) break;
            if(field[ny][nx] == '0' || field[ny][nx] == USED[field[y][x]-'0']) {
                P res = search_rev_dir(ny, nx, dir, y, x);
                if(res.first != -1) {
                    return make_pair(make_pair(ny, nx), res);
                }
            } else {
                break;
            }
            ny += dy[dir];
            nx += dx[dir];
        }
        return make_pair(make_pair(-1, -1), make_pair(-1, -1));
    }

    void move_edge_block(P now, P target, int dir) {        
        P nxt = now;
        while(1) {
            P nxt = now;
            nxt.first+=dy[dir], nxt.second+=dx[dir];
            // cout << "hello " << now.first << " " << now.second << " " << target.first << " " << target.second << endl;
            swap(field[now.first][now.second], field[nxt.first][nxt.second]);
            move_ans.emplace_back(now.first, now.second, nxt.first, nxt.second);
            move_count--;
            action_count_limit--;
            if(move_count <= 0) {
                is_move_end = true;
                break;
            }

            if(nxt.first == target.first && nxt.second == target.second) {
                break;
            }
            now.first = nxt.first;
            now.second = nxt.second;
        }
        return;
    }

    void connect_edge_block(P from, P to, P now, int edge_block_move_dir) {
        move_edge_block(now, from, edge_block_move_dir);
        // if(is_end) {
        //     return;
        // }

        // int dir = 0;
        // if(from.first == to.first) {
        //     if(from.second < to.second) {
        //         dir = 0; 
        //     } else {
        //         dir = 2;
        //     }
        // } else {
        //     if(from.first < to.first) {
        //         dir = 1;
        //     } else {
        //         dir = 3;
        //     }
        // }

        // // cout << from.first << " " << from.second << " " << dir << endl;
        // ConnectAction line_fill_res = line_fill(from.first, from.second, dir);
        // if(line_fill_res.c1_y == -1) {
        //     return;
        // }

        // // connect_ans.push_back(line_fill_res);
        // // action_count_limit--;
        // if (action_count_limit <= 0) {
        //     is_end = true;
        // }

        return;
    }

    void move()
    {
        bool is_break = false;
        rep(k_num, 1, K+1) {
            // init_connect(k_num);
            if(is_move_end) {
                break;
            }
            
            rep(y, 0, N) {
                rep(x, 0, N) {
                    if(field[y][x] == (char)('0'+k_num) && !is_move_end) {
                        // 末端なので動かせる
                        // if(is_edge_block(y, x)) {
                        int calc_dir_res = calc_dir(y, x);
                        if(calc_dir_res == -1) {
                            rep(dir, 0, 4) {
                                if(field[y][x] == (char)('0'+k_num) && !is_move_end) {
                                    pair<P, P> res = search_near_block(y, x, dir);
                                    P from = res.first, to = res.second;
                                    P now = {y, x};

                                    if(from.first != -1 && !is_move_end) {
                                        // edgeをfromに動かして、fromとtoを接続する
                                        connect_edge_block(from ,to, now, dir);
                                    }
                                }
                            }
                        } else {
                            rep(dir, 0, 4) {
                                if(dir%2 == calc_dir_res%2 && field[y][x] == (char)('0'+k_num) && !is_move_end) {
                                    pair<P, P> res = search_near_block(y, x, dir);
                                    P from = res.first, to = res.second;
                                    P now = {y, x};

                                    // cout << from.first << " " << from.second << " " << to.first << " " << to.second << endl;
                                    if(from.first != -1 && !is_move_end) {
                                        // edgeをfromに動かして、fromとtoを接続する
                                        connect_edge_block(from ,to, now, dir);
                                    }
                                }
                            }
                        }
                        // }
                    }
                }
            }
        }
    

        rep(k_num, 1, K+1) {
            if(action_count_limit <= 0) {
                is_end = true;
            }

            if(is_end) {
                break;
            }
            final_connect(k_num);
        }
    }

    pair<int, int> search_block(int y, int x, int dir, int limit, int base_block) const
    {
        int ny = y + dy[dir];
        int nx = x + dx[dir];
        rep(i, 0, limit) {
            if(!isInGrid(ny, nx, N, N)) {
                return make_pair(-1, -1);
            }
            
            // ネットワークの末端なら繋げられそう
            if (field[ny][nx] == base_block && !uf.isUnite(make_pair(ny,nx))) {
                return make_pair(ny, nx);
            } else if (field[ny][nx] != '0') {
                return make_pair(-1, -1);
            }
            ny += dy[dir];
            nx += dx[dir];
        }
        return make_pair(-1, -1);
    }

    bool can_connect(int y, int x, int dir, int limit) const
    {
        int ny = y + dy[dir];
        int nx = x + dx[dir];
        rep(i, 0, limit) {
            if(!isInGrid(ny, nx, N, N)) {
                return false;
            }

            if (field[ny][nx] == field[y][x]) {
                return true;
            } else if (field[ny][nx] != '0' && field[ny][nx] != USED[field[y][x]-'0']) {
                return false;
            }
            ny += dy[dir];
            nx += dx[dir];
        }
        return false;
    }

    ConnectAction line_fill(int y, int x, int dir)
    {
        int ny = y + dy[dir];
        int nx = x + dx[dir];
        while (isInGrid(ny, nx, N, N)) {
            pair<int, int> c1,c2;
            c1 = make_pair(y, x);
            c2 = make_pair(ny,nx);
            if (field[ny][nx] == field[y][x]) {
                if(!uf.same(c1, c2)) {
                    uf.unite(c1, c2);
                    return ConnectAction(y, x, ny, nx);
                } else {
                    break;
                }            
            }
            assert(field[ny][nx] == '0');
            field[ny][nx] = USED[field[y][x]-'0'];
            ny += dy[dir];
            nx += dx[dir];
        }
        // ここはバグってない
        ny -= dy[dir];
        nx -= dx[dir];
        while(ny != y || nx != x) {
            field[ny][nx] = '0';
            ny -= dy[dir];
            nx -= dx[dir];
        }
        return ConnectAction(-1, -1, -1, -1);
        assert(false);
    }

    // vector<ConnectAction> connect()
    // {
    //     vector<ConnectAction> ret;
    //     rep(i,0,N) {
    //         rep(j,0,N) {
    //             if (field[i][j] != '0' && field[i][j] != USED) {
    //                 // →と↓に向かって接続を試みる
    //                 rep(dir, 0, 2) {
    //                     if (can_connect(i, j, dir)) {
    //                         ret.push_back(line_fill(i, j, dir));
    //                         action_count_limit--;
    //                         if (action_count_limit <= 0) {
    //                             return ret;
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //     return ret;
    // }

    Result solve()
    {
        // create random moves
        move_count = 6 * (action_count_limit / 10);
        move();
        // from each computer, connect to right and/or bottom if it will reach the same type
        // auto connects = connect();
        // rep(i,0,N) {
        //     rep(j,0,N) {
        //         cout << i << " " << j << " " << field[i][j] << endl;
        //     }
        // }
        // cout << "count " << action_count_limit << endl;
        return Result(move_ans, connect_ans);
    }
};

int calc_score(int N, vector<string> field, const Result &res)
{
    for (auto r : res.move) {
        // cout << "hello " << r.before_y << " " << r.before_x << endl;
        // cout << r.before_y << " " << r.before_x << " " << r.after_y << " " << r.after_x << endl;
        assert(field[r.before_y][r.before_x] != '0');
        assert(field[r.after_y][r.after_x] == '0');
        swap(field[r.before_y][r.before_x], field[r.after_y][r.after_x]);
    }

    UnionFind UF;
    for (auto r : res.connect) {
        pair<int, int> p1(r.c1_y, r.c1_x), p2(r.c2_y, r.c2_x);
        UF.unite(p1, p2);
    }

    vector<pair<int, int>> computers;
    rep(i,0,N) {
        rep(j,0,N) {
            if (field[i][j] != '0') {
                computers.emplace_back(i, j);
            }
        }
    }

    int score = 0;
    rep(i,0,(int)computers.size()) {
        rep(j,i+1, (int)computers.size()) {
            auto c1 = computers[i];
            auto c2 = computers[j];
            if (UF.find(c1) == UF.find(c2)) {
                score += (field[c1.first][c1.second] == field[c2.first][c2.second]) ? 1 : -1;
            }
        }
    }

    return max(score, 0);
}

void print_answer(const Result &res)
{
    cout << res.move.size() << endl;
    for (auto m : res.move) {
        cout << m.before_y << " " << m.before_x << " "
            << m.after_y << " " << m.after_x << endl;
    }
    cout << res.connect.size() << endl;
    for (auto m : res.connect) {
        cout << m.c1_y << " " << m.c1_x << " "
            << m.c2_y << " " << m.c2_x << endl;
    }
}


int main()
{
    int N, K;
    cin >> N >> K;
    vector<string> field(N);
    rep(i,0,N) {
        cin >> field[i];
    }

    rep(i,1,K+1) {
        USED[i] = (char)('a'+(i-1));
    }

    Solver s(N, K, field);
    auto ret = s.solve();

    cerr << "Score = " << calc_score(N, field, ret) << endl;

    print_answer(ret);

    // rep(i,0,N) {
    //     rep(j,0,N) {
    //         cout << i << " " << j << " " << field[i][j] << endl;
    //     }
    // }

    return 0;
}
