#ifndef OTHello_AI_EVALUATOR_H
#define OTHello_AI_EVALUATOR_H

#include "../core/bitboard.h"
#include <cstdint>

namespace othello {

// 評価値の型（スコア範囲を大きく取る）
using Score = int64_t;

// 最大評価値
constexpr Score MAX_SCORE = 1000000;

// 評価関数
class Evaluator {
public:
    // 盤面を評価する（正の値=プレイヤー有利、負の値=相手有利）
    static Score evaluate(const Bitboard& board);

    // 進行度を取得（0.0 = 序盤、1.0 = 終盤）
    static double getProgress(const Bitboard& board);

    // Mobility（着手可能数）の評価
    static Score evaluateMobility(const Bitboard& board);

    // Corner（角の確保）の評価
    static Score evaluateCorner(const Bitboard& board);

    // Stability（確定石）の評価
    static Score evaluateStability(const Bitboard& board);

    // Coin Parity（石数差）の評価
    static Score evaluateCoinParity(const Bitboard& board);

    // 評価値の重みを設定
    struct Weights {
        Score mobilityWeight;
        Score cornerWeight;
        Score stabilityWeight;
        Score parityWeight;
    };

    // 進行度に応じた重みを取得
    static Weights getWeights(double progress);

    // 確定石の数を計算
    static int countStable(const Bitboard& board, uint64_t stones);

private:
    // 各段階の重み（序盤、中盤、終盤）
    static constexpr Weights OPENING_WEIGHTS = {100, 800, 100, 0};
    static constexpr Weights MIDGAME_WEIGHTS = {80, 600, 300, 50};
    static constexpr Weights ENDGAME_WEIGHTS = {50, 400, 500, 100};

    // 進行度の閾値
    static constexpr double MIDGAME_START = 0.3;  // 空きマスが56個以下
    static constexpr double ENDGAME_START = 0.75; // 空きマスが16個以下
};

} // namespace othello

#endif // OTHello_AI_EVALUATOR_H
