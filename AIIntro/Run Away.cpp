// 模拟退火
#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <set>
#include <cmath>
#include <algorithm>
#include <random>
using namespace std;
#define INF 0x3f3f3f3f
#define EPS 1e-7
#define DELTA 0.9

class Vec {
public:
    double x, y;
    Vec() {}
    Vec(const double& x, const double& y): x(x), y(y) {}
    double length() {
        return sqrt(x * x + y * y);
    }
    bool operator< (const Vec& other) const {
        return (x == other.x)? (y < other.y) : (x < other.x);
    }
    Vec operator+ (const Vec& other) const {
        return Vec(x + other.x, y + other.y);
    }
    Vec operator- (const Vec& other) const {
        return Vec(x - other.x, y - other.y);
    }
    friend ostream& operator<< (ostream& os, const Vec& v) {
        os << fixed << setprecision(1);
        os << "(" << fabs(v.x) << ", " << fabs(v.y) << ")";
        return os;
    }
    friend Vec operator* (const double& k, const Vec& v) {
        return Vec(k*v.x, k*v.y);
    }
};

set<Vec> holes;
int n;
double X, Y;
double ans, ansglobal;
Vec ansvec;

inline double dis_nearest(const Vec& v) {
    double ret = INF;
    for(auto& hole: holes) {
        ret = min(ret, (v-hole).length());
    }
    return ret;
}

inline bool check_boundary(const Vec& v) {
    return (v.x >= -EPS) && (v.x <= X + EPS) && (v.y >= -EPS) && (v.y <= Y + EPS);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int t; cin >> t;
    // 初始化随机数生成器
    static default_random_engine randomEngine;                      // 随机数生成引擎
    static uniform_real_distribution<double> doubleRandom(0, 1);    // 浮点随机数0~1
    while(t--) {
        holes.clear();
        ansglobal = 0;
        cin >> X >> Y >> n;
        for(int i=0; i<n; i++) {
            int tx, ty; cin >> tx >> ty;
            holes.insert(Vec(tx, ty));
        }
        Vec from;
        // 随机找15个初始点
        for(int i=1; i<=15; i++) {
            from.x = doubleRandom(randomEngine) * X;
            from.y = doubleRandom(randomEngine) * Y;
            ans = dis_nearest(from);
            double T = sqrt(X*X + Y*Y); // 设置初始温度(步长)为对角线
            while(T > EPS) {
                double curans = 0.f;
                Vec curvec;
                // 随机选择35个方向走一步看看
                for(int j=1; j<=35; j++) {
                    double rad = doubleRandom(randomEngine) * 2 * M_PI; // 随机选择一个方向
                    Vec nxtvec = from + T*Vec(cos(rad), sin(rad));  // 向这个方向迈出步长T
                    if(!check_boundary(nxtvec)) continue;   // 判断边界
                    double nxtans = dis_nearest(nxtvec);
                    if(nxtans > curans) {
                        curans = nxtans;
                        curvec = nxtvec;
                    }
                }
                if(curans > ans) {
                    // 如果更好，选择这一步
                    ans = curans;
                    from = curvec;
                } else {
                    // 否则概率选取这一步
                    if(doubleRandom(randomEngine) < exp(curans - ans) / T) {
                        ans = curans;
                        from = curvec;
                    }
                }
                T *= DELTA;
            }
            if(ans > ansglobal) {
                ansglobal = ans;
                ansvec = from;
            }
        }
        cout << "The safest point is " << ansvec << ".\n";
    }
}