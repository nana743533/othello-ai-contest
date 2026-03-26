#ifndef OTHello_AI_SEARCH_H
#define OTHello_AI_SEARCH_H

#include "bitboard.h"
#include "evaluator.h"
#include <vector>
#include <chrono>
#include <functional>

namespace othello {

// 探索結果
struct SearchResult {
    std::string move;      // 最善手（座標文字列）
    Score score;           // 評価値
    int depth;             // 探索深さ
    uint64_t nodes;        // 探索ノード数

    SearchResult() : move(""), score(0), depth(0), nodes(0) {}
    SearchResult(const std::string& m, Score s, int d, uint64_t n)
        : move(m), score(s), depth(d), nodes(n) {}
};

// 探索エンジン
class SearchEngine {
public:
    SearchEngine();

    // 最善手を探索する
    SearchResult search(const Bitboard& board, int timeoutMs = 150);

    // 最小・最大探索深さを設定
    void setMinDepth(int minDepth) { minDepth_ = minDepth; }
    void setMaxDepth(int maxDepth) { maxDepth_ = maxDepth; }

    // 統計情報をリセット
    void resetStats();

    // 統計情報を取得
    uint64_t getNodeCount() const { return nodeCount_; }

private:
    // 反復深化探索
    SearchResult iterativeDeepening(const Bitboard& board, int timeoutMs);

    // NegaScout探索（Principal Variation Search）
    Score negascout(const Bitboard& board, int depth, Score alpha, Score beta, bool isRoot);

    // 合法手の順序付け
    std::vector<std::pair<Score, std::string>> orderMoves(const Bitboard& board, const std::vector<std::string>& moves);

    // 時間チェック
    bool isTimeUp() const;

    // 座標をビット位置に変換
    static int coordToBit(const std::string& coord);
    static std::string bitToCoord(int bit);

    // メンバ変数
    int minDepth_;           // 最小探索深さ
    int maxDepth_;           // 最大探索深さ
    uint64_t nodeCount_;     // 探索ノード数
    std::chrono::steady_clock::time_point startTime_;
    int timeoutMs_;          // タイムアウト時間（ミリ秒）
};

} // namespace othello

#endif // OTHello_AI_SEARCH_H
