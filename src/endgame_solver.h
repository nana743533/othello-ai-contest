#ifndef OTHello_AI_ENDGAME_SOLVER_H
#define OTHello_AI_ENDGAME_SOLVER_H

#include "bitboard.h"
#include <cstdint>

namespace othello {

// 終盤ソルバー（完全読み）
class EndgameSolver {
public:
    // 完全読みで最善手を探索
    // 戻り値: 最善手の座標文字列（パスの場合は"pass"）
    static std::string solve(const Bitboard& board);

    // 最終スコアを計算（正: 黒勝ち、負: 白勝ち）
    static int32_t finalScore(const Bitboard& board);

    // 終盤ソルバーが適用可能か判定
    static bool isApplicable(const Bitboard& board, int threshold = 15);

private:
    // 完全読みの探索
    // 戻り値: 最終石数差（正: 黒勝ち、負: 白勝ち）
    static int32_t search(Bitboard board, bool isBlack);

    // Win/Loss/Draw判定（高速版）
    // 戻り値: 1=黒勝ち, -1=白勝ち, 0=引き分け
    static int8_t searchWLD(Bitboard board, bool isBlack);

    // パスの処理
    static bool handlePass(Bitboard& board, bool& isBlack);

    // 定数
    static constexpr int32_t INF = 1000000;
};

} // namespace othello

#endif // OTHello_AI_ENDGAME_SOLVER_H
