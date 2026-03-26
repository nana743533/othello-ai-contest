#include "simple_evaluator.h"

namespace othello {

int SimpleEvaluator::getWeight(const std::string& coord) {
    if (coord.length() < 2) return 0;

    // 座標のパース: "c3" -> column='c'(2), row=3(2)
    // Python: W[int(a[1])-1][ord(a[0])-97]
    int col = coord[0] - 'a';      // ord(a[0]) - 97
    int row = coord[1] - '1';      // int(a[1]) - 1

    // 範囲チェック
    if (col < 0 || col >= 8 || row < 0 || row >= 8) {
        return 0;
    }

    // Pythonコードでは W[row][col] だが、rowは下から数えている
    // A1 -> row=0, col=0 -> WEIGHTS[7][0] = 100 (左下の角)
    // 座標系: a1=左下, h8=右上
    // WEIGHTS[7-row][col] でマッピング
    return WEIGHTS[7 - row][col];
}

std::string SimpleEvaluator::chooseBestMove(const Bitboard& board) {
    uint64_t legalMoves = board.getLegalMoves();

    if (legalMoves == 0) {
        return "pass";
    }

    std::string bestMove;
    int bestWeight = -999999;

    // 全ての合法手について重みを計算
    while (legalMoves) {
        int lsb = __builtin_ctzll(legalMoves);
        int col = lsb % 8;
        int row = lsb / 8;

        // 座標を文字列に変換
        std::string coord;
        coord += ('a' + col);
        coord += ('1' + row);

        int weight = getWeight(coord);

        if (weight > bestWeight) {
            bestWeight = weight;
            bestMove = coord;
        }

        legalMoves &= (legalMoves - 1);
    }

    return bestMove;
}

int SimpleEvaluator::evaluate(const Bitboard& board) {
    int score = 0;

    // 自分の石について重みを合計
    uint64_t myStones = board.player;
    while (myStones) {
        int lsb = __builtin_ctzll(myStones);
        int col = lsb % 8;
        int row = lsb / 8;
        score += WEIGHTS[7 - row][col];
        myStones &= (myStones - 1);
    }

    // 相手の石について重みを合計（引く）
    uint64_t oppStones = board.opponent;
    while (oppStones) {
        int lsb = __builtin_ctzll(oppStones);
        int col = lsb % 8;
        int row = lsb / 8;
        score -= WEIGHTS[7 - row][col];
        oppStones &= (oppStones - 1);
    }

    return score;
}

} // namespace othello
