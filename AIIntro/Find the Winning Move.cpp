#include <iostream>
#include <algorithm>
#include <array>
using namespace std;

enum BlockState {
    WHITE = -1,
    EMPTY,
    BLACK,
};

BlockState board[16];
int chessCnt;
pair<int, int> ans;

inline int get_ID(int x, int y) {
    return (x<<2)|y;
}

inline pair<int, int> get_xy(int id) {
    return {id>>2, id&3};
}

constexpr array<array<int, 4>, 10> winStates {{
    // in a row
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {8, 9, 10, 11},
    {12, 13, 14, 15},
    // in a col
    {0, 4, 8, 12},
    {1, 5, 9, 13},
    {2, 6, 10, 14},
    {3, 7, 11, 15},
    // in a diagnol
    {0, 5, 10, 15},
    {3, 6, 9, 12}
}};

inline bool check_win() {
    for(auto& winState: winStates) {
        int sum = 0;
        for(auto& idx: winState) {
            sum += board[idx];
        }
        if(abs(sum) == 4) {
            return true;
        }
    }
    return false;
}

int min_search(int);

int max_search(int idx) {
    // the rival wins
    if(check_win()) {
        return -1;
    }
    // the board's full, a draw
    if(chessCnt == 16) {
        return 0;
    }
    for(int idx=0; idx<16; idx++) {
        if(board[idx] == EMPTY) {
            board[idx] = BLACK;
            chessCnt++;
            int ret = min_search(idx);
            chessCnt--;
            board[idx] = EMPTY;
            // prune
            if(ret == 1) {
                return 1;
            }
        }
    }        
    return -1;
}

int min_search(int idx) {
    // I win
    if(check_win()) {
        return 1;
    }
    // the board's full, a draw
    if(chessCnt == 16) {
        return 0;
    }
    for(int idx=0; idx<16; idx++) {
        if(board[idx] == EMPTY) {
            board[idx] = WHITE;
            chessCnt++;
            int ret = max_search(idx);
            chessCnt--;
            board[idx] = EMPTY;
            // 如果有不是必赢的后续，那现在这步不是必赢
            if(ret != 1) {
                return -1;
            }
        }
    }
    return 1;
}

bool solve() {
    for(int idx=0; idx<16; idx++) {
        if(board[idx] == EMPTY) {
            board[idx] = BLACK;
            chessCnt++;
            int ret = min_search(idx);
            chessCnt--;
            board[idx] = EMPTY;
            if(ret == 1) {
                ans = get_xy(idx);
                return true;
            }
        }
    }
    return false;
}

int main() {
    char op;
    while(cin >> op, op != '$') {
        /* input */
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++) {
                int idx = get_ID(i, j);
                char tmp; cin >> tmp;
                switch(tmp) {
                    case '.':
                        board[idx] = EMPTY;
                        break;
                    case 'x':
                        board[idx] = BLACK;
                        break;
                    case 'o':
                        board[idx] = WHITE;
                        break;
                    default:
                        cerr << "[ERROR] Wrong Input Format!\n";
                }
                chessCnt += (board[idx] != '.');
            }
        /* search */
        if(solve()) {
            cout << "(" << ans.first << "," << ans.second << ")\n";
        } else {
            cout << "#####\n";
        }
    }
    return 0;
}