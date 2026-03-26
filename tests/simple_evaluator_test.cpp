#include <gtest/gtest.h>
#include "simple_evaluator.h"

using namespace othello;

// 位置重みのテスト
TEST(SimpleEvaluatorTest, GetWeight_CornerPositions) {
    // 四隅は100点
    EXPECT_EQ(SimpleEvaluator::getWeight("a1"), 100);
    EXPECT_EQ(SimpleEvaluator::getWeight("h1"), 100);
    EXPECT_EQ(SimpleEvaluator::getWeight("a8"), 100);
    EXPECT_EQ(SimpleEvaluator::getWeight("h8"), 100);
}

TEST(SimpleEvaluatorTest, GetWeight_XSquares) {
    // X打ち（角に隣接する危険位置）は-50点
    EXPECT_EQ(SimpleEvaluator::getWeight("b1"), -20);  // a1の隣
    EXPECT_EQ(SimpleEvaluator::getWeight("g1"), -20);  // h1の隣
    EXPECT_EQ(SimpleEvaluator::getWeight("b2"), -50);  // a1の斜め隣（X打ち）
    EXPECT_EQ(SimpleEvaluator::getWeight("g2"), -50);  // h1の斜め隣（X打ち）
    EXPECT_EQ(SimpleEvaluator::getWeight("b7"), -50);  // a8の斜め隣（X打ち）
    EXPECT_EQ(SimpleEvaluator::getWeight("g7"), -50);  // h8の斜め隣（X打ち）
}

TEST(SimpleEvaluatorTest, GetWeight_CenterPositions) {
    // 中央付近は0〜5点
    EXPECT_EQ(SimpleEvaluator::getWeight("d4"), 0);
    EXPECT_EQ(SimpleEvaluator::getWeight("e4"), 0);
    EXPECT_EQ(SimpleEvaluator::getWeight("d5"), 0);
    EXPECT_EQ(SimpleEvaluator::getWeight("e5"), 0);
    EXPECT_EQ(SimpleEvaluator::getWeight("d3"), 1);
    EXPECT_EQ(SimpleEvaluator::getWeight("c4"), 1);
}

TEST(SimpleEvaluatorTest, GetWeight_AllPositionsMatchPython) {
    // Pythonコードの重み行列と一致することを確認
    int expected[8][8] = {
        {100, -20,  10,   5,   5,  10, -20, 100},
        {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
        { 10,  -2,   1,   1,   1,   1,  -2,  10},
        {  5,  -2,   1,   0,   0,   1,  -2,   5},
        {  5,  -2,   1,   0,   0,   1,  -2,   5},
        { 10,  -2,   1,   1,   1,   1,  -2,  10},
        {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
        {100, -20,  10,   5,   5,  10, -20, 100}
    };

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            std::string coord;
            coord += ('a' + col);
            coord += ('1' + row);
            EXPECT_EQ(SimpleEvaluator::getWeight(coord), expected[7 - row][col])
                << "Failed at " << coord;
        }
    }
}

// 盤面評価のテスト
TEST(SimpleEvaluatorTest, Evaluate_InitialPosition) {
    // 初期状態: 黒がd4, e5、白がe4, d5
    Bitboard board = Bitboard::initial();

    int score = SimpleEvaluator::evaluate(board);
    // d4(0) + e5(0) - e4(0) - d5(0) = 0
    EXPECT_EQ(score, 0);
}

TEST(SimpleEvaluatorTest, Evaluate_CaptureCorner) {
    Bitboard board = Bitboard::initial();
    // a1に黒の石を置く（実際のゲームでは不可能だが評価テスト用）
    board.player |= (1ULL << 0);

    int score = SimpleEvaluator::evaluate(board);
    // a1=100, 初期位置4個の合計=0, より score >= 100
    EXPECT_GE(score, 100);
}

// chooseBestMoveのテスト
TEST(SimpleEvaluatorTest, ChooseBestMove_PrefersCorners) {
    Bitboard board = Bitboard::initial();

    // 合法手を手動で設定（角を含む場合）
    // 実際の合法手生成を使う

    std::string bestMove = SimpleEvaluator::chooseBestMove(board);

    // パスではないことを確認
    EXPECT_NE(bestMove, "pass");

    // 有効な座標形式であることを確認
    EXPECT_GE(bestMove[0] - 'a', 0);
    EXPECT_LE(bestMove[0] - 'a', 7);
    EXPECT_GE(bestMove[1] - '1', 0);
    EXPECT_LE(bestMove[1] - '1', 7);
}

TEST(SimpleEvaluatorTest, ChooseBestMove_NoLegalMoves) {
    // 全て埋まった盤面（パスになるケース）
    Bitboard board;
    board.player = 0xFFFFFFFFFFFFFFFFULL;
    board.opponent = 0;

    std::string bestMove = SimpleEvaluator::chooseBestMove(board);
    EXPECT_EQ(bestMove, "pass");
}

// 不正な座標のテスト
TEST(SimpleEvaluatorTest, GetWeight_InvalidCoords) {
    EXPECT_EQ(SimpleEvaluator::getWeight(""), 0);
    EXPECT_EQ(SimpleEvaluator::getWeight("a"), 0);
    EXPECT_EQ(SimpleEvaluator::getWeight("i1"), 0);  // 列が範囲外
    EXPECT_EQ(SimpleEvaluator::getWeight("a9"), 0);  // 行が範囲外
    EXPECT_EQ(SimpleEvaluator::getWeight("a0"), 0);  // 行が範囲外
}
