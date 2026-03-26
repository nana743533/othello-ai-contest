// ========================================
// CodinGame Othello AI - I/Oテスト用
// 入力を確認して指し手を出力（座標のみ）
// ========================================

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    // ===== 初回入力 =====
    string line;

    // プレイヤーID
    if (!getline(cin, line)) return 0;
    int playerId = stoi(line);
    cerr << "Player ID: " << playerId << " (" << (playerId == 0 ? "Black" : "White") << ")" << endl;

    // 盤面サイズ
    if (!getline(cin, line)) return 0;
    int boardSize = stoi(line);
    cerr << "Board size: " << boardSize << endl;

    // ===== ゲームループ =====
    int turn = 1;
    while (cin) {
        cerr << "\n=== Turn " << turn << " ===" << endl;

        // 盤面の状態
        cerr << "Board state:" << endl;
        for (int i = 0; i < boardSize; i++) {
            if (!getline(cin, line)) return 0;
            cerr << "  " << line << endl;
        }

        // Expert Modeの追加入力（条件付き）
        // 注: 前ターンでEXPERTを出力していた場合、ここで追加の入力が来る
        // 今回は使用しないのでスキップ

        // 合法手の数
        if (!getline(cin, line)) return 0;
        int actionCount = stoi(line);
        cerr << "Legal moves: " << actionCount << endl;

        // 合法手のリスト
        string firstMove = "pass";
        for (int i = 0; i < actionCount; i++) {
            if (!getline(cin, line)) return 0;
            // 余分な空白を削除
            while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
                line.pop_back();
            }
            if (i == 0) firstMove = line; // 最初の手を保存
            cerr << "  " << line << endl;
        }

        // 指し手を出力（座標のみ、coutに送信される）
        if (actionCount > 0) {
            cout << firstMove << endl;
            cerr << ">>> Output: " << firstMove << endl;
        } else {
            cout << "pass" << endl;
            cerr << ">>> Output: pass" << endl;
        }

        turn++;
    }

    return 0;
}
