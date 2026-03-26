#include <gtest/gtest.h>
#include "search.h"
#include "bitboard.h"

using namespace othello;

// 探索エンジン基本テスト
TEST(SearchTest, BasicSearch) {
    Bitboard board = Bitboard::initial();
    SearchEngine engine;

    SearchResult result = engine.search(board, 1000);

    // 何らかの手が返ってくるはず
    EXPECT_FALSE(result.move.empty());
    EXPECT_GT(result.depth, 0);
}

// 探索時間制限テスト
TEST(SearchTest, TimeLimit) {
    Bitboard board = Bitboard::initial();
    SearchEngine engine;

    // 短い時間制限
    SearchResult result = engine.search(board, 10);

    // 何らかの手が返ってくるはず
    EXPECT_FALSE(result.move.empty());
}

// 合法手チェック
TEST(SearchTest, LegalMove) {
    Bitboard board = Bitboard::initial();
    SearchEngine engine;

    SearchResult result = engine.search(board, 100);

    // 初期配置での合法手: c4, d3, e6, f5 のいずれか
    std::vector<std::string> legalMoves = {"c4", "d3", "e6", "f5"};
    bool found = false;
    for (const auto& move : legalMoves) {
        if (result.move == move) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found) << "Move: " << result.move;
}

// パスの処理テスト
TEST(SearchTest, PassHandling) {
    Bitboard board;
    // 全てのマスを埋める（黒勝利）
    board.player = 0xFFFFFFFFFFFFFFFFULL;
    board.opponent = 0;

    SearchEngine engine;
    SearchResult result = engine.search(board, 100);

    // パスが返ってくるはず
    EXPECT_EQ(result.move, "pass");
}

// 終盤ソルバーテスト
TEST(EndgameSolverTest, IsApplicable) {
    Bitboard board = Bitboard::initial();

    // 初期配置では適用不可（60個空き）
    EXPECT_FALSE(EndgameSolver::isApplicable(board, 15));

    // 15個以下で適用可能
    // テスト用に盤面を埋める
    for (int i = 0; i < 50; i++) {
        int x = i % 8;
        int y = i / 8;
        board.placePlayer(x, y);
    }

    EXPECT_TRUE(EndgameSolver::isApplicable(board, 15));
}

// 終盤ソルバー解答テスト
TEST(EndgameSolverTest, Solve) {
    Bitboard board;

    // 残り1マスの状態を作る
    board.player = 0x00FFFFFFFFFFFFFFULL; // 下2行以外黒
    board.opponent = 0xFF00000000000000ULL; // 下2行白

    std::string move = EndgameSolver::solve(board);

    // 何らかの手またはパスが返ってくる
    EXPECT_FALSE(move.empty());
}

// 最終スコアテスト
TEST(EndgameSolverTest, FinalScore) {
    Bitboard board;
    board.player = 0xFFFFFFFFFFFFFFFFULL; // 全て黒
    board.opponent = 0;

    int32_t score = EndgameSolver::finalScore(board);

    // 64-0=64 黒勝ち
    EXPECT_EQ(score, 64);
}
