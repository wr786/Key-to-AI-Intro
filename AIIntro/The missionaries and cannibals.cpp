#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <queue>
using namespace std;
#define DEBUGx
// 左岸传教士人，食人魔人，船是否在(0|1)
unordered_map<int, unordered_map<int, unordered_map<int, bool> >> vis;
int m, n;

class Status {
public:
    int M, C;
    bool B;
    int step;
    Status() {}
    Status(int m, int c, bool b=true, int s=0):
        M(m), C(c), B(b), step(s) {}
    int check(int dM, int dC) {
        // 船上若有M，则人数要不少于C
        if(dM && dM < dC) return 7;
        if(B) {     // 船在左
            // 要有足够的人运
            if(M < dM || C < dC) return 1;
            // M要剩得比C多
            if(M > dM && M - dM < C - dC) return 2;
            // 对岸也一样
            if(m - M + dM > 0 && m - M + dM < m - C + dC) return 3;
            // 避免左右横跳
            if(vis[M - dM][C - dC][!B]) return 4;
            return 0;
        } else {    // 船在右
            // 要有足够的人运
            if(m-M < dM || m-C < dC) return 1;
            // M要剩得比C多
            if(m-M > dM && m-M-dM < m-C-dC) return 2;
            // 对岸也一样
            if(M+dM > 0 && M+dM < C+dC) return 3;
            // 避免左右横跳
            if(vis[M + dM][C + dC][!B]) return 4;
            return 0;
        }
    }
    Status transfer(int dM, int dC) {
        Status ret = *this;
        if(B) {
            ret.M -= dM;
            ret.C -= dC;
        } else {
            ret.M += dM;
            ret.C += dC;
        }
        ret.B ^= true;
        ret.step++;
        vis[ret.M][ret.C][ret.B] = true;
        return ret;
    }
    bool operator < (const Status & other) const {return step > other.step;}
};

// BFS
priority_queue<Status> pq;

int main() {
    cin >> m >> n;
    pq.push(Status(m, m));
    vis[m][m][1] = true;
    while(!pq.empty()) {
        Status cur = pq.top(); pq.pop();
        if(!cur.M && !cur.C) {
            cout << cur.step << endl;
            return 0;
        }
        for(int i=1; i<=n; i++) {
            for(int j=0; j<=i; j++) {
                int ret = cur.check(i-j, j);
                if(ret == 0) {
                    pq.push(cur.transfer(i-j, j));
                } else {
#ifdef DEBUG
                    printf("[INFO] (%d,%d,%d): (%d,%d) check failed. ERROR %d\n", 
                                    cur.M, cur.C, cur.B, i-j, j, ret);
#endif
                }
            }
        }
    }
    cout << -1 << endl;
    return 0;
}