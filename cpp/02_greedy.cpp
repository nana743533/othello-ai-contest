/*
 * CodinGame Othello AI - グリーディープレイヤー
 * 位置重み W から、合法手の中で最も重みが高い手を選ぶ
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static constexpr int W[8][8] = {
    {100, -20,  10,   5,   5,  10, -20, 100},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    { 10,  -2,   1,   1,   1,   1,  -2,  10},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    { 10,  -2,   1,   1,   1,   1,  -2,  10},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    {100, -20,  10,   5,   5,  10, -20, 100},
};

/* 行末の空白・改行残りを取り除く */
static string trim(string s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r')) {
        s.pop_back();
    }
    return s;
}

/* 座標（例: "c3"）の位置重みを返す */
static int score(const string& coord) {
    if (coord.size() < 2) return 0;
    int col = coord[0] - 'a';
    int row = coord[1] - '1';
    if (col < 0 || col >= 8 || row < 0 || row >= 8) return 0;
    return W[row][col];
}

/* 合法手の中から重み最大の手を選ぶ */
static string chooseBestMove(const vector<string>& legalMoves) {
    string bestMove = legalMoves[0];
    int bestScore = score(bestMove);
    for (const string& move : legalMoves) {
        int s = score(move);
        if (s > bestScore) {
            bestScore = s;
            bestMove = move;
        }
    }
    return bestMove;
}

/* CodinGame 入出力ループ。重み最大の合法手を出力する */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;

    /* 初回入力: プレイヤーIDと盤面サイズを読む */
    if (!getline(cin, line)) return 0;
    int playerId = stoi(line);
    (void)playerId;

    if (!getline(cin, line)) return 0;
    int boardSize = stoi(line);

    /* 毎ターン: 盤面を読み飛ばし、重み最大の合法手を選ぶ */
    while (cin) {
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

        if (legalMoves.empty()) {
            cout << "pass" << endl;
        } else {
            cout << chooseBestMove(legalMoves) << endl;
        }
    }

    return 0;
}
