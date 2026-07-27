/*
 * CodinGame Othello AI - Minimax
 * 位置重み評価 + n手先読み（Alpha-Beta Minimax）
 */

#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

#include <algorithm>
#include <array>
#include <climits>
#include <string>
#include <utility>
#include <vector>

/* 8x8 盤面（各行は長さ8の文字列） */
using Board = array<string, 8>;

/* 位置重み行列（角が高評価、その隣は低評価） */
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

/* 石の反転を調べる8方向 */
static constexpr int DIRS[8][2] = {
    {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1},
};

/* 行末の空白・改行残りを取り除く */
inline string trim(string s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r')) {
        s.pop_back();
    }
    return s;
}

/* 盤面文字列8行から Board を作る */
inline Board boardFromLines(const vector<string>& lines) {
    Board board{};
    for (int i = 0; i < 8; i++) {
        board[i] = lines[i];
    }
    return board;
}

/* 座標 (x, y) の位置重みを返す */
inline int cellWeight(int x, int y) {
    return W[y][x];
}

/* CodinGame 形式（例: d3）を盤面座標 (x, y) に変換 */
inline pair<int, int> fromCoord(const string& coord) {
    return {coord[0] - 'a', coord[1] - '1'};
}

/* 盤面座標を CodinGame 形式（例: d3）に変換 */
inline string toCoord(int x, int y) {
    return string(1, static_cast<char>('a' + x)) + to_string(y + 1);
}

/* 座標（例: "c3"）の位置重みを返す */
inline int coordWeight(const string& coord) {
    auto [x, y] = fromCoord(coord);
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return 0;
    return W[y][x];
}

/* (x, y) に player が打ったとき反転するマス一覧 */
inline vector<pair<int, int>> getFlips(const Board& board, int x, int y, int player) {
    const char opponent = static_cast<char>('0' + (1 - player));
    const char me = static_cast<char>('0' + player);
    vector<pair<int, int>> flips;

    for (const auto& dir : DIRS) {
        int cx = x + dir[0];
        int cy = y + dir[1];
        vector<pair<int, int>> line;
        while (cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && board[cy][cx] == opponent) {
            line.push_back({cx, cy});
            cx += dir[0];
            cy += dir[1];
        }
        if (!line.empty() && cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && board[cy][cx] == me) {
            flips.insert(flips.end(), line.begin(), line.end());
        }
    }
    return flips;
}

/* (x, y) が player の合法手かどうか */
inline bool isLegal(const Board& board, int x, int y, int player) {
    if (board[y][x] != '.') return false;
    return !getFlips(board, x, y, player).empty();
}

/* player の合法手を重み降順で返す */
inline vector<string> getLegalMoves(const Board& board, int player) {
    vector<string> moves;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (isLegal(board, x, y, player)) {
                moves.push_back(toCoord(x, y));
            }
        }
    }
    sort(moves.begin(), moves.end(), [](const string& a, const string& b) {
        return coordWeight(a) > coordWeight(b);
    });
    return moves;
}

/* coord に player の石を置き、反転後の盤面を返す */
inline Board applyMove(const Board& board, const string& coord, int player) {
    auto [x, y] = fromCoord(coord);
    Board next = board;
    next[y][x] = static_cast<char>('0' + player);
    for (const auto& [fx, fy] : getFlips(board, x, y, player)) {
        next[fy][fx] = static_cast<char>('0' + player);
    }
    return next;
}

/* 合法手を重み降順に並べ替える */
inline void sortMovesByWeight(vector<string>& moves) {
    sort(moves.begin(), moves.end(), [](const string& a, const string& b) {
        return coordWeight(a) > coordWeight(b);
    });
}

/* 自分目線: 盤面全体の重み合計差 */
inline int evaluateBoard(const Board& board, int me) {
    int myScore = 0;
    int oppScore = 0;
    const char meCh = static_cast<char>('0' + me);
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char cell = board[y][x];
            if (cell == meCh) {
                myScore += cellWeight(x, y);
            } else if (cell != '.') {
                oppScore += cellWeight(x, y);
            }
        }
    }
    return myScore - oppScore;
}

/* 一手の寄与。自分の打点は +W、相手の打点は -W */
inline int moveDelta(const string& coord, int currentPlayer, int me) {
    int w = coordWeight(coord);
    return currentPlayer == me ? w : -w;
}

/* 空きマス数を数える */
inline int countEmpty(const Board& board) {
    int empty = 0;
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == '.') empty++;
        }
    }
    return empty;
}

/* 終局時の石数差（自分 - 相手） */
inline int finalScore(const Board& board, int me) {
    int my = 0;
    int opp = 0;
    const char meCh = static_cast<char>('0' + me);
    const char oppCh = static_cast<char>('0' + (1 - me));
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == meCh) my++;
            else if (cell == oppCh) opp++;
        }
    }
    return my - opp;
}


/* 読みの深さ（半手 = ply） */
static constexpr int DEPTH = 4;

/* Alpha-Beta Minimax。me 目線の盤面評価値を返す */
static int minimax(
    const Board& board,
    int depth,
    int currentPlayer,
    int me,
    int alpha,
    int beta
) {
    /* 深さ 0 ならリーフ: 現在局面を評価して返す */
    if (depth == 0) {
        return evaluateBoard(board, me);
    }

    vector<string> moves = getLegalMoves(board, currentPlayer);
    if (moves.empty()) {
        /* 双方パスなら終局、そうでなければパスして手番交代 */
        if (getLegalMoves(board, 1 - currentPlayer).empty()) {
            return evaluateBoard(board, me);
        }
        return minimax(board, depth - 1, 1 - currentPlayer, me, alpha, beta);
    }

    if (currentPlayer == me) {
        /* 自分の手番: 評価値が最大になる手を選ぶ */
        int bestScore = INT_MIN;
        for (const string& move : moves) {
            Board next = applyMove(board, move, currentPlayer);
            int score = minimax(next, depth - 1, 1 - currentPlayer, me, alpha, beta);
            bestScore = max(bestScore, score);
            alpha = max(alpha, bestScore);
            if (beta <= alpha) break;
        }
        return bestScore;
    }

    /* 相手の手番: 相手目線で最大 = 自分目線では評価値最小 */
    int bestScore = INT_MAX;
    for (const string& move : moves) {
        Board next = applyMove(board, move, currentPlayer);
        int score = minimax(next, depth - 1, 1 - currentPlayer, me, alpha, beta);
        bestScore = min(bestScore, score);
        beta = min(beta, bestScore);
        if (beta <= alpha) break;
    }
    return bestScore;
}

/* 合法手ごとに n 手先まで読み、評価値が最大の手を選ぶ */
static pair<string, int> chooseMove(const Board& board, vector<string> legalMoves, int me) {
    sortMovesByWeight(legalMoves);
    string bestMove = legalMoves[0];
    int bestScore = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;

    for (const string& move : legalMoves) {
        Board next = applyMove(board, move, me);
        int score = minimax(next, DEPTH - 1, 1 - me, me, alpha, beta);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
        alpha = max(alpha, bestScore);
    }
    return {bestMove, bestScore};
}

/* CodinGame 入出力ループ。最善手と先読み評価値を出力する */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    /* 初回入力: プレイヤーIDと盤面サイズを読む */
    if (!getline(cin, line)) return 0;
    int me = stoi(line);

    if (!getline(cin, line)) return 0;
    int boardSize = stoi(line);

    /* 毎ターン: 盤面と合法手を読み、Minimax で手を選ぶ */
    while (cin) {
        vector<string> rows;
        rows.reserve(boardSize);
        for (int i = 0; i < boardSize; i++) {
            if (!getline(cin, line)) return 0;
            rows.push_back(line);
        }
        Board board = boardFromLines(rows);

        if (!getline(cin, line)) return 0;
        int actionCount = stoi(line);

        vector<string> legalMoves;
        legalMoves.reserve(actionCount);
        for (int i = 0; i < actionCount; i++) {
            if (!getline(cin, line)) return 0;
            legalMoves.push_back(trim(line));
        }

        if (legalMoves.empty()) {
            cout << "pass" << endl;
            continue;
        }

        auto [move, score] = chooseMove(board, legalMoves, me);
        cout << move << " MSG " << score << endl;
    }

    return 0;
}
