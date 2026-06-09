#include <gtest/gtest.h>
#include "evaluators/evaluator.h"
#include "core/bitboard.h"

using namespace othello;

// 評価関数基本テスト
TEST(EvaluatorTest, EvaluateInitialPosition) {
    Bitboard board = Bitboard::initial();
    Score score = Evaluator::evaluate(board);

    // 初期配置では互角に近い値になる（多少の偏りはありうる）
    EXPECT_GT(score, -1000);
    EXPECT_LT(score, 1000);
}

// 進行度テスト
TEST(EvaluatorTest, Progress) {
    Bitboard initial = Bitboard::initial();
    double progressInitial = Evaluator::getProgress(initial);

    // 初期状態は progress = 0 に近い
    EXPECT_NEAR(progressInitial, 0.0, 0.1);

    // 全て埋まった状態（盤面を直接操作してテスト）
    Bitboard full;
    full.player = 0xFFFFFFFFFFFFFFFFULL;
    full.opponent = 0;
    double progressFull = Evaluator::getProgress(full);

    // 全て埋まった状態は progress = 1.0
    EXPECT_NEAR(progressFull, 1.0, 0.01);
}

// Mobility評価テスト
TEST(EvaluatorTest, Mobility) {
    Bitboard board = Bitboard::initial();
    Score mobility = Evaluator::evaluateMobility(board);

    // 初期配置では合法手が4つなので、Mobilityは0に近い
    EXPECT_GE(mobility, -10);
    EXPECT_LE(mobility, 10);
}

// Corner評価テスト
TEST(EvaluatorTest, Corner) {
    Bitboard board = Bitboard::initial();
    Score corner = Evaluator::evaluateCorner(board);

    // 初期配置では角に石がない
    EXPECT_EQ(corner, 0);

    // 角に黒を置く
    board.placePlayer("a1");
    Score cornerWithA1 = Evaluator::evaluateCorner(board);

    // a1を取ったら評価値が上がるはず
    EXPECT_GT(cornerWithA1, corner);
}

// 確定石評価テスト
TEST(EvaluatorTest, Stability) {
    Bitboard board = Bitboard::initial();
    Score stability = Evaluator::evaluateStability(board);

    // 初期配置では確定石が0
    EXPECT_EQ(stability, 0);

    // 角とその周辺を埋めて確定石を作る
    board.placePlayer("a1");
    board.placePlayer("a2");
    board.placePlayer("b1");

    Score stabilityWithCorner = Evaluator::evaluateStability(board);

    // 角を取ったら確定石が増えるはず
    EXPECT_GT(stabilityWithCorner, 0);
}

// 石数差評価テスト
TEST(EvaluatorTest, CoinParity) {
    Bitboard board = Bitboard::initial();
    Score parity = Evaluator::evaluateCoinParity(board);

    // 初期配置では石数が等しい
    EXPECT_EQ(parity, 0);

    // 石を追加して差を作る
    board.placePlayer("a1");
    board.placePlayer("b1");

    Score parityWithAdvantage = Evaluator::evaluateCoinParity(board);

    // 2石増えたので+2
    EXPECT_EQ(parityWithAdvantage, 2);
}

// 動的ウェイトテスト
TEST(EvaluatorTest, DynamicWeights) {
    Evaluator::Weights opening = Evaluator::getWeights(0.0);
    Evaluator::Weights midgame = Evaluator::getWeights(0.5);
    Evaluator::Weights endgame = Evaluator::getWeights(1.0);

    // 序盤はMobility重視
    EXPECT_GT(opening.mobilityWeight, 0);

    // 終盤はCoin Parity重視
    EXPECT_GT(endgame.parityWeight, 0);
    EXPECT_GT(endgame.parityWeight, opening.parityWeight);

    // 終盤はStability重視
    EXPECT_GT(endgame.stabilityWeight, opening.stabilityWeight);
}
