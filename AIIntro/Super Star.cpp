#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
#define INF 0x3f3f3f3f
#define EPS 1e-7
#define DELTA 0.98
#define INIT_T 100

class Vec {
public:
    double x, y, z;
    Vec() {}
    Vec(double x, double y, double z): x(x), y(y), z(z) {}
    double length() {
        return sqrt(x * x + y * y + z * z);
    }
    Vec operator- (const Vec& other) const {
        return Vec(x - other.x, y - other.y, z - other.z);
    }
    Vec operator+ (const Vec& other) const {
        return Vec(x + other.x, y + other.y, z + other.z);
    }
    friend Vec operator* (double k, const Vec& v) {
        return Vec(k*v.x, k*v.y, k*v.z);
    }
};
typedef Vec Point;
int n;
vector<Point> points;

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    while(cin >> n, n) {
        points.clear();
        for(int i=0; i<n; i++) {
            double tx, ty, tz; cin >> tx >> ty >> tz;
            points.push_back(Point(tx, ty, tz));
        }
        double T = INIT_T;
        Point ansCenter(0, 0, 0);
        double ansRadius = INF;
        int maxDisIdx = 0;
        // 模拟退火？不如说是梯度下降
        while(T > EPS) {
            // 找一个离现在中心最远的点
            for(int i=0; i<n; i++) {
                if((points[i]-ansCenter).length() > (points[maxDisIdx]-ansCenter).length()) {
                    maxDisIdx = i;
                }
            }
            double dis = (points[maxDisIdx]-ansCenter).length();
            // 离中心最远的点的距离当然能把所有点都覆盖
            ansRadius = min(ansRadius, dis);
            // 试图逼近离的最远的点
            ansCenter = ansCenter + (T/dis)*(points[maxDisIdx] - ansCenter);
            T *= DELTA;
        }
        cout << fixed << setprecision(5) << ansRadius << endl;
    }
    return 0;
}