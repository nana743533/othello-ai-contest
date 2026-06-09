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

    // 盤面サイズ
    if (!getline(cin, line)) return 0;
    int boardSize = stoi(line);

    // ===== ゲームループ =====
    while (cin) {
        // 盤面の状態（読み飛ばし）
        for (int i = 0; i < boardSize; i++) {
            if (!getline(cin, line)) return 0;
        }

        // Expert Modeの追加入力（条件付き）
        // 注: 前ターンでEXPERTを出力していた場合、ここで追加の入力が来る
        // 今回は使用しないのでスキップ

        // 合法手の数
        if (!getline(cin, line)) return 0;
        int actionCount = stoi(line);

        // 合法手のリスト（読み飛ばし）
        string firstMove = "pass";
        for (int i = 0; i < actionCount; i++) {
            if (!getline(cin, line)) return 0;
            // 余分な空白を削除
            while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
                line.pop_back();
            }
            if (i == 0) firstMove = line;
        }

        // 指し手を出力（座標のみ）
        if (actionCount > 0) {
            cout << firstMove << endl;
        } else {
            cout << "pass" << endl;
        }
    }

    return 0;
}
