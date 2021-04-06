#include <iostream>
#include <unordered_map>
using namespace std;
int a[10][10];
typedef unordered_map<int, bool> dmap;
dmap rowvis;
dmap colvis;
dmap blkvis;

inline int get_blk(int x, int y) {
    return 3*((x-1)/3) + (y-1)/3 + 1;
}

inline int make_pair(int a, int b) {
    return a*10 + b;
}

inline void upd_vis(int x, int y, bool status=true) {
    int val = a[x][y];
    rowvis[make_pair(x, val)] = status;
    colvis[make_pair(y, val)] = status;
    blkvis[make_pair(get_blk(x, y), val)] = status;
}

bool dfs(int x, int y) {
    if(x == 10) {return true;}
    int ny = ((y==9)?(1):(y+1));
    int nx = ((y==9)?(x+1):(x));
    if(a[x][y]) return dfs(nx, ny);
    for(int d=1; d<=9; d++) {
        if(!rowvis[make_pair(x, d)] && !colvis[make_pair(y, d)] && !blkvis[make_pair(get_blk(x, y), d)]) {
            a[x][y] = d;
            upd_vis(x, y);
            if(dfs(nx, ny))
                return true;
            upd_vis(x, y, false);
            a[x][y] = 0;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int _t; cin >> _t;
    while(_t--) {
        char c;
        // init
        rowvis.clear();
        colvis.clear();
        blkvis.clear();
        for(int i=1; i<=9; i++) {
            for(int j=1; j<=9; j++) {
                cin >> c;
                a[i][j] = c - '0';
                if(a[i][j])
                    upd_vis(i, j);
            }
        }
        dfs(1, 1);
        for(int i=1; i<=9; i++) {
            for(int j=1; j<=9; j++) {
                cout << a[i][j];
            }
            cout << '\n';
        }
    }
}