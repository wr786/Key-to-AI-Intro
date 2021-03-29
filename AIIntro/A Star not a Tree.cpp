#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <vector>
#include <array>
#include <random>
#include <limits>
using namespace std;
#define INF ~(1<<31)
#define EPS 1e-8
#define DELTA 0.99978666     // 缓缓下降
#define INIT_T 100           // 初始温度

constexpr array<pair<int, int>, 4> adj4 {{
    {0, 1}, {0, -1}, {-1, 0}, {1, 0}
}};

class Position {
public:
    double x, y;
    Position() {}
    Position(double x_, double y_): x(x_), y(y_) {}
    double dis_to(const Position& other) const {
        return sqrt((x-other.x)*(x-other.x) + (y-other.y)*(y-other.y));
    }
};

int computerCnt;
vector<Position> computers;

inline double dist_sum(const Position& p) {
    double ret = 0;
    for(auto& o: computers) {
        ret += p.dis_to(o);
    }
    return ret;
}

// 模拟退火
double simul_anneal() {
    // 随机数生成器
    static default_random_engine randomEngine;                      // 随机数生成引擎
    static uniform_int_distribution<int> intRandom(0, 3);           // int随机数0~3
    // 不妨从计算机的中点开始移动
    double mx = 0, my = 0;
    for(auto& p: computers) {
        mx += p.x;
        my += p.y;
    }
    mx /= computerCnt, my /= computerCnt;
    Position from(mx, my);
    Position to;
    double t = INIT_T;
    double ret = INF;
    while(t - 0 > EPS) {
        int dir = intRandom(randomEngine);  // 随机选择方向
        to.x = from.x + adj4[dir].first;
        to.y = from.y + adj4[dir].second;
        double cur = dist_sum(to);
        if(ret - cur > EPS) {
            // 若变好了，选择此移动
            ret = cur;
            from = to;
        }
        t *= DELTA; // 退水
    }
    // 温度小于阈值，输出
    return ret;
}

int main() {
    while(cin >> computerCnt) {
        computers.clear();
        for(int i=0; i<computerCnt; i++) {
            double x, y; cin >> x >> y;
            computers.push_back(Position(x, y));
        }
        cout << fixed << setprecision(0) << simul_anneal() << endl;
    }
    return 0;
}