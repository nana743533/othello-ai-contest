/*
 * CodinGame Othello AI - ランダムプレイヤー
 * 入出力仕様（docs/01_io_spec.md）に従い、合法手からランダムに一手を選ぶ
 */

#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

/* 行末の空白・改行残りを取り除く */
static string trim(string s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r')) {
        s.pop_back();
    }
    return s;
}

/* CodinGame 入出力ループ。合法手からランダムに一手を選んで出力する */
int main() {
    /* C の printf/scanf との同期を切って cin/cout を速くする */
    ios::sync_with_stdio(false);
    /* cin 読み取り前の cout 自動 flush を外す */
    cin.tie(nullptr);

    /* 1行分の入力を入れる変数 */
    string line;

    /* 初回入力: プレイヤーIDと盤面サイズを読む */
    if (!getline(cin, line)) return 0;
    int playerId = stoi(line);
    (void)playerId;

    if (!getline(cin, line)) return 0;
    int boardSize = stoi(line);

    mt19937 rng(random_device{}());
    int turn = 0;

    /* 毎ターン: 盤面を読み飛ばし、合法手からランダムに選ぶ */
    while (cin) {
        turn += 1;

        /* 盤面の状態（boardSize 行）を読み飛ばす */
        for (int i = 0; i < boardSize; i++) {
            if (!getline(cin, line)) return 0;
        }

        /* 合法手の数を読む */
        if (!getline(cin, line)) return 0;
        int actionCount = stoi(line);

        /* 合法手のリストを読む */
        vector<string> legalMoves;
        legalMoves.reserve(actionCount);
        for (int i = 0; i < actionCount; i++) {
            if (!getline(cin, line)) return 0;
            legalMoves.push_back(trim(line));
        }

        /* 手番と合法手数を MSG に載せて出力する */
        string msg = "t" + to_string(turn) + "," + to_string(actionCount);
        if (legalMoves.empty()) {
            cout << "pass MSG " << msg << endl;
        } else {
            uniform_int_distribution<int> dist(0, static_cast<int>(legalMoves.size()) - 1);
            cout << legalMoves[dist(rng)] << " MSG " << msg << endl;
        }
    }

    return 0;
}
