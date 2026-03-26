#include <gtest/gtest.h>
#include "bitboard.h"

using namespace othello;

// Bitboard 初期化テスト
TEST(BitboardTest, InitialPosition) {
    Bitboard board = Bitboard::initial();

    // e4 (col=4, row=3) は黒
    EXPECT_TRUE(board.hasPlayerAt(4, 3));
    // d5 (col=3, row=4) は黒
    EXPECT_TRUE(board.hasPlayerAt(3, 4));
    // d4 (col=3, row=3) は相手（白）
    EXPECT_TRUE(board.hasOpponentAt(3, 3));
    // e5 (col=4, row=4) は相手（白）
    EXPECT_TRUE(board.hasOpponentAt(4, 4));

    // 石の数チェック
    EXPECT_EQ(board.getPlayerCount(), 2);
    EXPECT_EQ(board.getOpponentCount(), 2);
    EXPECT_EQ(board.getEmptyCount(), 60);
}

// Bitboard 座標変換テスト
TEST(BitboardTest, CoordinateConversion) {
    EXPECT_EQ(Bitboard::getMask("a1"), 1ULL << 0);
    EXPECT_EQ(Bitboard::getMask("h1"), 1ULL << 7);
    EXPECT_EQ(Bitboard::getMask("a8"), 1ULL << 56);
    EXPECT_EQ(Bitboard::getMask("h8"), 1ULL << 63);
    EXPECT_EQ(Bitboard::getMask("e4"), 1ULL << 28);
}

// Bitboard 合法手生成テスト
TEST(BitboardTest, LegalMoves) {
    Bitboard board = Bitboard::initial();

    uint64_t legalMoves = board.getLegalMoves();

    // 初期配置での黒の合法手: c4, d3, e6, f5
    EXPECT_TRUE(board.isLegalMove("c4"));
    EXPECT_TRUE(board.isLegalMove("d3"));
    EXPECT_TRUE(board.isLegalMove("e6"));
    EXPECT_TRUE(board.isLegalMove("f5"));

    // 非法手
    EXPECT_FALSE(board.isLegalMove("d4")); // 既に石がある
    EXPECT_FALSE(board.isLegalMove("a1")); // 反転できない

    // 合法手の数は4つ
    EXPECT_EQ(__builtin_popcountll(legalMoves), 4);
}

// Bitboard 石の反転テスト
TEST(BitboardTest, PlaceAndFlip) {
    Bitboard board = Bitboard::initial();

    // 黒が e4 の隣の f4 に打つと、e4 の白が反転される...はずだが
    // 実際には初期配置から黒が打てるのは c4, d3, e6, f5 のみ

    // f5 に打つ場合
    Bitboard after = board.placeAndFlip("f5");

    // f5 に黒の石が置かれる
    EXPECT_TRUE(after.hasPlayerAt("f5"));
    // f4 の白が黒に反転される（e4が黒でf4が白だから）
    EXPECT_TRUE(after.hasPlayerAt("f4"));
}

// Bitboard 盤面パーステスト
TEST(BitboardTest, ParseFromString) {
    std::string boardStr =
        "........\n"
        "........\n"
        "........\n"
        "...01...\n"
        "...10...\n"
        "........\n"
        "........\n"
        "........\n";

    Bitboard board = Bitboard::fromString(boardStr, true); // 黒視点

    EXPECT_TRUE(board.hasPlayerAt("e4")); // 黒
    EXPECT_TRUE(board.hasPlayerAt("d5")); // 黒
    EXPECT_TRUE(board.hasOpponentAt("d4")); // 白
    EXPECT_TRUE(board.hasOpponentAt("e5")); // 白
}

// Bitboard スワップテスト
TEST(BitboardTest, Swap) {
    Bitboard board = Bitboard::initial();
    Bitboard swapped = board.swap();

    // スワップ後は元のプレイヤーが相手、元の相手がプレイヤー
    EXPECT_TRUE(swapped.hasPlayerAt("d4")); // 元は相手
    EXPECT_TRUE(swapped.hasOpponentAt("e4")); // 元はプレイヤー
}
