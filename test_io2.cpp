// ========================================
// CodinGame Othello AI - I/Oテスト用（最少返し石戦略）
// 最も返す石の少ない手を選ぶ
// ========================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int g_boardSize = 8;
int g_playerId = -1;

// 8方向のデルタ
const int DX[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int DY[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

// 盤面をパース
void parseBoard(vector<string>& board) {
    board.clear();
    string line;
    for (int i = 0; i < g_boardSize; i++) {
        if (!getline(cin, line)) return;
        board.push_back(line);
    }
}

// 座標文字列をインデックスに変換
bool coordToIndex(const string& coord, int& x, int& y) {
    if (coord.length() < 2) return false;
    x = coord[0] - 'a';
    y = coord[1] - '1';
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// 指定位置に石を置いたときに返る石数を計算
int countFlips(const vector<string>& board, int x, int y, int playerId) {
    // 空きマスでない場合は置けない
    if (board[y][x] != '.') return 0;

    int myChar = (playerId == 0) ? '0' : '1';
    int oppChar = (playerId == 0) ? '1' : '0';
    int totalFlips = 0;

    // 8方向チェック
    for (int d = 0; d < 8; d++) {
        int dx = DX[d], dy = DY[d];
        int cx = x + dx, cy = y + dy;
        int flips = 0;

        // 相手の石を連続してカウント
        while (cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && board[cy][cx] == oppChar) {
            flips++;
            cx += dx;
            cy += dy;
        }

        // 自分の石で挟めていればカウント
        if (flips > 0 && cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && board[cy][cx] == myChar) {
            totalFlips += flips;
        }
    }

    return totalFlips;
}

int main() {
    // ===== 初回入力 =====
    string line;

    // プレイヤーID
    if (!getline(cin, line)) return 0;
    g_playerId = stoi(line);

    // 盤面サイズ
    if (!getline(cin, line)) return 0;
    g_boardSize = stoi(line);

    // ===== ゲームループ =====
    while (cin) {
        // 盤面の状態を読み込み
        vector<string> board;
        parseBoard(board);

        // 合法手の数
        if (!getline(cin, line)) return 0;
        int actionCount = stoi(line);

        // 合法手のリストを読み込み
        vector<pair<string, int>> moves; // (手, 返す石数)
        for (int i = 0; i < actionCount; i++) {
            if (!getline(cin, line)) return 0;
            // 余分な空白を削除
            while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
                line.pop_back();
            }

            int x, y;
            int flips = 0;
            if (coordToIndex(line, x, y)) {
                flips = countFlips(board, x, y, g_playerId);
            }
            moves.push_back({line, flips});
        }

        // 指し手を出力（最も返す石の少ない手）
        if (actionCount > 0) {
            auto best = min_element(moves.begin(), moves.end(),
                [](const auto& a, const auto& b) {
                    return a.second < b.second;
                });
            cout << best->first << endl;
        } else {
            cout << "pass" << endl;
        }
    }

    return 0;
}
