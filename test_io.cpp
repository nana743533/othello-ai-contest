// ========================================
// CodinGame Othello AI - I/Oテスト用
// 入力をそのまま表示するだけのシンプル版
// ========================================

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    // ===== 初回入力 =====
    string line;

    // プレイヤーID
    getline(cin, line);
    int playerId = stoi(line);
    cout << "Player ID: " << playerId << " (" << (playerId == 0 ? "Black/先手" : "White/後手") << ")" << endl;

    // 盤面サイズ
    getline(cin, line);
    int boardSize = stoi(line);
    cout << "Board size: " << boardSize << endl;

    // ===== ゲームループ =====
    int turn = 1;
    while (true) {
        cout << "\n=== Turn " << turn << " ===" << endl;

        // 盤面の状態
        cout << "Board state:" << endl;
        for (int i = 0; i < boardSize; i++) {
            getline(cin, line);
            cout << "  " << line << endl;
        }

        // 合法手の数
        getline(cin, line);
        int actionCount = stoi(line);
        cout << "Legal moves: " << actionCount << endl;

        // 合法手のリスト
        cout << "Moves: ";
        for (int i = 0; i < actionCount; i++) {
            getline(cin, line);
            cout << line;
            if (i < actionCount - 1) cout << ", ";
        }
        cout << endl;

        // とりあえず最初の手を出力
        if (actionCount > 0) {
            cout << ">>> Output: " << line << endl << endl;
        } else {
            cout << ">>> Output: pass" << endl << endl;
        }

        turn++;
    }

    return 0;
}
