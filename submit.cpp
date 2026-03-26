// ========================================
// CodinGame Othello AI - Submit Version
// Single file submission for CodinGame contest
// ========================================

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>

using namespace std;

// ========================================
// Constants & Types
// ========================================

using Score = int64_t;

constexpr Score MAX_SCORE = 1000000;
constexpr int DIRECTION_DELTAS[8][2] = {
    {-1, -1}, {0, -1}, {1, -1},
    {-1,  0},          {1,  0},
    {-1,  1}, {0,  1}, {1,  1}
};

constexpr uint64_t CORNER_MASK =
    (1ULL << 0) | (1ULL << 7) | (1ULL << 56) | (1ULL << 63);
constexpr uint64_t X_SQUARES_MASK =
    (1ULL << 9) | (1ULL << 14) | (1ULL << 49) | (1ULL << 54);

// ========================================
// Bitboard
// ========================================

class Bitboard {
public:
    uint64_t player = 0;
    uint64_t opponent = 0;

    Bitboard() = default;
    Bitboard(uint64_t p, uint64_t o) : player(p), opponent(o) {}

    static Bitboard initial() {
        uint64_t black = (1ULL << 28) | (1ULL << 35);
        uint64_t white = (1ULL << 27) | (1ULL << 36);
        return Bitboard(black, white);
    }

    static uint64_t getMask(int x, int y) {
        if (x < 0 || x >= 8 || y < 0 || y >= 8) return 0;
        return 1ULL << (y * 8 + x);
    }

    static uint64_t getMask(const string& coord) {
        if (coord.length() < 2) return 0;
        return getMask(coord[0] - 'a', coord[1] - '1');
    }

    bool hasPlayerAt(int x, int y) const {
        return (player & getMask(x, y)) != 0;
    }

    bool hasOpponentAt(int x, int y) const {
        return (opponent & getMask(x, y)) != 0;
    }

    bool isEmptyAt(int x, int y) const {
        return ((player | opponent) & getMask(x, y)) == 0;
    }

    void placePlayer(int x, int y) {
        uint64_t mask = getMask(x, y);
        player |= mask;
        opponent &= ~mask;
    }

    void placeOpponent(int x, int y) {
        uint64_t mask = getMask(x, y);
        opponent |= mask;
        player &= ~mask;
    }

    uint64_t getFlipsInDirection(int x, int y, int dx, int dy) const {
        uint64_t flips = 0;
        int cx = x + dx, cy = y + dy;

        while (cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && hasOpponentAt(cx, cy)) {
            flips |= getMask(cx, cy);
            cx += dx;
            cy += dy;
        }

        if (cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && hasPlayerAt(cx, cy)) {
            return flips;
        }
        return 0;
    }

    uint64_t getFlips(int x, int y) const {
        if (!isEmptyAt(x, y)) return 0;
        uint64_t totalFlips = 0;
        for (int i = 0; i < 8; i++) {
            totalFlips |= getFlipsInDirection(x, y, DIRECTION_DELTAS[i][0], DIRECTION_DELTAS[i][1]);
        }
        return totalFlips;
    }

    uint64_t getLegalMoves() const {
        uint64_t legalMoves = 0;
        uint64_t empty = ~(player | opponent) & 0xFFFFFFFFFFFFFFFFULL;

        while (empty) {
            int lsb = __builtin_ctzll(empty);
            int x = lsb % 8, y = lsb / 8;
            if (getFlips(x, y) != 0) {
                legalMoves |= (1ULL << lsb);
            }
            empty &= (empty - 1);
        }
        return legalMoves;
    }

    Bitboard placeAndFlip(int x, int y) const {
        Bitboard result = *this;
        uint64_t flips = getFlips(x, y);
        if (flips == 0) return result;
        result.placePlayer(x, y);
        result.player |= flips;
        result.opponent &= ~flips;
        return result;
    }

    Bitboard placeAndFlip(const string& coord) const {
        if (coord.length() < 2) return *this;
        return placeAndFlip(coord[0] - 'a', coord[1] - '1');
    }

    int getEmptyCount() const {
        return __builtin_popcountll(~(player | opponent) & 0xFFFFFFFFFFFFFFFFULL);
    }

    int getPlayerCount() const {
        return __builtin_popcountll(player);
    }

    int getOpponentCount() const {
        return __builtin_popcountll(opponent);
    }

    Bitboard swap() const {
        return Bitboard(opponent, player);
    }

    static Bitboard fromString(const string& boardStr, bool isBlack) {
        Bitboard board;
        int row = 0;
        for (char c : boardStr) {
            if (c == '\n') continue;
            if (c == '0') {
                if (isBlack) board.placePlayer(row % 8, row / 8);
                else board.placeOpponent(row % 8, row / 8);
            } else if (c == '1') {
                if (isBlack) board.placeOpponent(row % 8, row / 8);
                else board.placePlayer(row % 8, row / 8);
            }
            row++;
        }
        return board;
    }
};

// ========================================
// Evaluator
// ========================================

class Evaluator {
public:
    // 各マスの静的な重み（オセロの定石に基づく）
    // 角: 2714, C Squares: -577, 辺中央: -18～69, 内部: -122～-379
    static constexpr int CELL_WEIGHT[64] = {
        2714,  147,   69,  -18,  -18,   69,  147, 2714,
         147, -577, -186, -153, -153, -186, -577,  147,
          69, -186, -379, -122, -122, -379, -186,   69,
         -18, -153, -122, -169, -169, -122, -153,  -18,
         -18, -153, -122, -169, -169, -122, -153,  -18,
          69, -186, -379, -122, -122, -379, -186,   69,
         147, -577, -186, -153, -153, -186, -577,  147,
        2714,  147,   69,  -18,  -18,   69,  147, 2714
    };

    static Score evaluate(const Bitboard& board) {
        // 位置評価（各石の位置に応じた重みの合計）
        Score positionScore = 0;
        for (int i = 0; i < 64; i++) {
            if (board.player & (1ULL << i)) {
                positionScore += CELL_WEIGHT[i];
            } else if (board.opponent & (1ULL << i)) {
                positionScore -= CELL_WEIGHT[i];
            }
        }

        // Mobility評価（着手可能数の差）
        int myMob = __builtin_popcountll(board.getLegalMoves());
        int oppMob = __builtin_popcountll(board.swap().getLegalMoves());
        Score mobilityScore = (myMob - oppMob) * 50;

        // 進行度に応じて石数差を少し考慮
        int emptyCount = board.getEmptyCount();
        Score parityScore = 0;
        if (emptyCount < 20) {
            parityScore = (board.getPlayerCount() - board.getOpponentCount()) * (20 - emptyCount);
        }

        return positionScore + mobilityScore + parityScore;
    }
};

// ========================================
// Search Engine
// ========================================

class SearchEngine {
public:
    string search(const Bitboard& board, int timeoutMs = 140) {
        startTime_ = chrono::steady_clock::now();
        timeoutMs_ = timeoutMs;
        nodeCount_ = 0;

        uint64_t legalMoves = board.getLegalMoves();
        if (legalMoves == 0) return "pass";

        vector<string> moves;
        while (legalMoves) {
            int lsb = __builtin_ctzll(legalMoves);
            moves.push_back(bitToCoord(lsb));
            legalMoves &= (legalMoves - 1);
        }

        string bestMove = moves[0];
        Score bestScore = -MAX_SCORE;

        for (int depth = 1; depth <= 14; depth++) {
            if (isTimeUp()) break;

            Score alpha = -MAX_SCORE;
            for (const auto& move : moves) {
                if (isTimeUp()) break;

                Bitboard nextBoard = board.placeAndFlip(move);
                Score score = -negalpha(nextBoard.swap(), depth - 1, -MAX_SCORE, MAX_SCORE);

                if (score > alpha) {
                    alpha = score;
                    bestMove = move;
                    bestScore = score;
                }
            }
        }

        return bestMove;
    }

private:
    chrono::steady_clock::time_point startTime_;
    int timeoutMs_;
    uint64_t nodeCount_ = 0;

    Score negalpha(const Bitboard& board, int depth, Score alpha, Score beta) {
        nodeCount_++;

        if (depth == 0) {
            return Evaluator::evaluate(board);
        }

        uint64_t legalMoves = board.getLegalMoves();

        if (legalMoves == 0) {
            Bitboard swapped = board.swap();
            if (swapped.getLegalMoves() == 0) {
                int diff = board.getPlayerCount() - board.getOpponentCount();
                if (diff > 0) return MAX_SCORE - diff;
                if (diff < 0) return -MAX_SCORE + diff;
                return 0;
            }
            return -negalpha(swapped, depth - 1, -beta, -alpha);
        }

        vector<string> moves;
        while (legalMoves) {
            int lsb = __builtin_ctzll(legalMoves);
            moves.push_back(bitToCoord(lsb));
            legalMoves &= (legalMoves - 1);
        }

        // Move ordering（良い手を先に探索）
        sort(moves.begin(), moves.end(), [](const string& a, const string& b) {
            int aBit = coordToBit(a), bBit = coordToBit(b);
            bool aIsCorner = (CORNER_MASK & (1ULL << aBit)) != 0;
            bool bIsCorner = (CORNER_MASK & (1ULL << bBit)) != 0;
            bool aIsCSquare = (X_SQUARES_MASK & (1ULL << aBit)) != 0;
            bool bIsCSquare = (X_SQUARES_MASK & (1ULL << bBit)) != 0;

            // 角を最優先
            if (aIsCorner && !bIsCorner) return true;
            if (!aIsCorner && bIsCorner) return false;

            // C Squaresを最後に
            if (aIsCSquare && !bIsCSquare) return false;
            if (!aIsCSquare && bIsCSquare) return true;

            // その他は位置の重みで順序付け
            int aWeight = Evaluator::CELL_WEIGHT[aBit];
            int bWeight = Evaluator::CELL_WEIGHT[bBit];
            return aWeight > bWeight;
        });

        Score maxScore = -MAX_SCORE;
        for (const auto& move : moves) {
            if (isTimeUp()) break;

            Bitboard nextBoard = board.placeAndFlip(move);
            Score score = -negalpha(nextBoard.swap(), depth - 1, -beta, -alpha);

            if (score >= beta) return score;  // Beta cut-off
            alpha = max(alpha, score);
            maxScore = max(maxScore, score);
        }

        return maxScore;
    }

    bool isTimeUp() const {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - startTime_).count();
        return elapsed >= timeoutMs_;
    }

    static int coordToBit(const string& coord) {
        return (coord[1] - '1') * 8 + (coord[0] - 'a');
    }

    static string bitToCoord(int bit) {
        string coord;
        coord += ('a' + (bit % 8));
        coord += ('1' + (bit / 8));
        return coord;
    }
};

// ========================================
// Endgame Solver
// ========================================

class EndgameSolver {
public:
    static bool isApplicable(const Bitboard& board, int threshold = 15) {
        return board.getEmptyCount() <= threshold;
    }

    static string solve(const Bitboard& board) {
        uint64_t legalMoves = board.getLegalMoves();
        if (legalMoves == 0) return "pass";

        int32_t bestScore = -1000000;
        string bestMove;

        while (legalMoves) {
            int lsb = __builtin_ctzll(legalMoves);
            string move = bitToCoord(lsb);

            Bitboard nextBoard = board.placeAndFlip(move);
            int32_t score = -search(nextBoard.swap());

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }

            legalMoves &= (legalMoves - 1);
        }

        return bestMove;
    }

private:
    static int32_t search(Bitboard board) {
        uint64_t legalMoves = board.getLegalMoves();

        if (legalMoves == 0) {
            Bitboard swapped = board.swap();
            if (swapped.getLegalMoves() == 0) {
                return board.getPlayerCount() - board.getOpponentCount();
            }
            return -search(swapped);
        }

        int32_t bestScore = -1000000;
        while (legalMoves) {
            int lsb = __builtin_ctzll(legalMoves);
            Bitboard nextBoard = board.placeAndFlip(lsb % 8, lsb / 8);
            int32_t score = -search(nextBoard.swap());

            if (score > bestScore) bestScore = score;

            legalMoves &= (legalMoves - 1);
        }

        return bestScore;
    }

    static string bitToCoord(int bit) {
        string coord;
        coord += ('a' + (bit % 8));
        coord += ('1' + (bit / 8));
        return coord;
    }
};

// ========================================
// Main
// ========================================

int g_playerId = -1;
int g_boardSize = 8;
Bitboard g_currentBoard;

void parseInitialInput() {
    string line;
    if (!getline(cin, line)) return;
    g_playerId = stoi(line);

    if (!getline(cin, line)) return;
    g_boardSize = stoi(line);
}

void parseTurnInput(vector<string>& legalMoves) {
    string boardStr;
    for (int i = 0; i < g_boardSize; i++) {
        string line;
        if (!getline(cin, line)) return;
        boardStr += line;
    }

    string line;
    if (!getline(cin, line)) return;
    int actionCount = stoi(line);

    legalMoves.clear();
    for (int i = 0; i < actionCount; i++) {
        if (!getline(cin, line)) return;
        while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
            line.pop_back();
        }
        legalMoves.push_back(line);
    }

    bool isBlack = (g_playerId == 0);
    g_currentBoard = Bitboard::fromString(boardStr, isBlack);
}

string chooseMove(const vector<string>& legalMoves) {
    if (legalMoves.empty()) return "pass";

    // 終盤ソルバー（残り22マス以下で完全読み）
    if (EndgameSolver::isApplicable(g_currentBoard, 22)) {
        string move = EndgameSolver::solve(g_currentBoard);

        for (const auto& legal : legalMoves) {
            if (move == legal) return move;
        }
    }

    // 探索エンジン
    SearchEngine engine;
    string move = engine.search(g_currentBoard, 140);

    for (const auto& legal : legalMoves) {
        if (move == legal) return move;
    }

    return legalMoves[0];
}

int main() {
    try {
        parseInitialInput();

        while (cin) {
            vector<string> legalMoves;
            parseTurnInput(legalMoves);

            string bestMove = chooseMove(legalMoves);

            cout << bestMove << endl;
        }

    } catch (...) {
        return 1;
    }

    return 0;
}
