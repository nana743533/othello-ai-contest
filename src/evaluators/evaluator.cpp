#include "evaluator.h"
#include <algorithm>
#include <cmath>

namespace othello {

double Evaluator::getProgress(const Bitboard& board) {
    int emptyCount = board.getEmptyCount();
    // 初期状態: 60個空き -> progress = 0.0
    // 終了状態: 0個空き -> progress = 1.0
    return 1.0 - (emptyCount / 60.0);
}

Score Evaluator::evaluateMobility(const Bitboard& board) {
    // 自分の合法手の数
    uint64_t myMoves = board.getLegalMoves();
    int myMobility = __builtin_popcountll(myMoves);

    // 相手の合法手の数（盤面を入れ替えて計算）
    uint64_t oppMoves = board.swap().getLegalMoves();
    int oppMobility = __builtin_popcountll(oppMoves);

    // 合法手が少ない方を優位とする
    return (myMobility - oppMobility);
}

Score Evaluator::evaluateCorner(const Bitboard& board) {
    int score = 0;

    // 角のビット位置: a1=0, h1=7, a8=56, h8=63
    // 自分が持っている角
    uint64_t myCorners = board.player & CORNER_MASK;
    // 相手が持っている角
    uint64_t oppCorners = board.opponent & CORNER_MASK;

    score += 3 * (__builtin_popcountll(myCorners) - __builtin_popcountll(oppCorners));

    // X-Squares（角に隣接する危険な位置）のペナルティ
    uint64_t myXSquares = board.player & X_SQUARES_MASK;
    uint64_t oppXSquares = board.opponent & X_SQUARES_MASK;

    score -= 2 * (__builtin_popcountll(myXSquares) - __builtin_popcountll(oppXSquares));

    return score;
}

Score Evaluator::evaluateStability(const Bitboard& board) {
    int myStable = countStable(board, board.player);
    int oppStable = countStable(board, board.opponent);

    return (myStable - oppStable);
}

int Evaluator::countStable(const Bitboard& board, uint64_t stones) {
    int stable = 0;

    // 角から始まる確定石を探索
    // 右上・右下・左上・左下の4方向

    // 各角について
    int corners[4] = {0, 7, 56, 63};  // a1, h1, a8, h8

    for (int corner : corners) {
        if ((stones & (1ULL << corner)) == 0) continue; // 角を持っていない

        // 各方向について連続する石を数える
        // a1: 右方向、下方向
        if (corner == 0) {
            // 右方向
            for (int x = 0; x < 8; x++) {
                if ((stones & (1ULL << x)) == 0) break;
                stable++;
            }
            // 下方向
            for (int y = 0; y < 8; y++) {
                if ((stones & (1ULL << (y * 8))) == 0) break;
                stable++;
            }
        }
        // h1: 左方向、下方向
        else if (corner == 7) {
            // 左方向
            for (int x = 7; x >= 0; x--) {
                if ((stones & (1ULL << x)) == 0) break;
                stable++;
            }
            // 下方向
            for (int y = 0; y < 8; y++) {
                if ((stones & (1ULL << (y * 8 + 7))) == 0) break;
                stable++;
            }
        }
        // a8: 右方向、上方向
        else if (corner == 56) {
            // 右方向
            for (int x = 0; x < 8; x++) {
                if ((stones & (1ULL << (56 + x))) == 0) break;
                stable++;
            }
            // 上方向
            for (int y = 7; y >= 0; y--) {
                if ((stones & (1ULL << (y * 8))) == 0) break;
                stable++;
            }
        }
        // h8: 左方向、上方向
        else if (corner == 63) {
            // 左方向
            for (int x = 7; x >= 0; x--) {
                if ((stones & (1ULL << (56 + x))) == 0) break;
                stable++;
            }
            // 上方向
            for (int y = 7; y >= 0; y--) {
                if ((stones & (1ULL << (y * 8 + 7))) == 0) break;
                stable++;
            }
        }
    }

    return stable / 2; // 角を2回カウントしているので補正
}

Score Evaluator::evaluateCoinParity(const Bitboard& board) {
    return board.getPlayerCount() - board.getOpponentCount();
}

Evaluator::Weights Evaluator::getWeights(double progress) {
    Weights w;

    if (progress < MIDGAME_START) {
        // 序盤: 線形補間
        double t = progress / MIDGAME_START;
        w.mobilityWeight = OPENING_WEIGHTS.mobilityWeight * (1 - t) + MIDGAME_WEIGHTS.mobilityWeight * t;
        w.cornerWeight = OPENING_WEIGHTS.cornerWeight * (1 - t) + MIDGAME_WEIGHTS.cornerWeight * t;
        w.stabilityWeight = OPENING_WEIGHTS.stabilityWeight * (1 - t) + MIDGAME_WEIGHTS.stabilityWeight * t;
        w.parityWeight = OPENING_WEIGHTS.parityWeight * (1 - t) + MIDGAME_WEIGHTS.parityWeight * t;
    } else if (progress < ENDGAME_START) {
        // 中盤: 線形補間
        double t = (progress - MIDGAME_START) / (ENDGAME_START - MIDGAME_START);
        w.mobilityWeight = MIDGAME_WEIGHTS.mobilityWeight * (1 - t) + ENDGAME_WEIGHTS.mobilityWeight * t;
        w.cornerWeight = MIDGAME_WEIGHTS.cornerWeight * (1 - t) + ENDGAME_WEIGHTS.cornerWeight * t;
        w.stabilityWeight = MIDGAME_WEIGHTS.stabilityWeight * (1 - t) + ENDGAME_WEIGHTS.stabilityWeight * t;
        w.parityWeight = MIDGAME_WEIGHTS.parityWeight * (1 - t) + ENDGAME_WEIGHTS.parityWeight * t;
    } else {
        // 終盤
        w = ENDGAME_WEIGHTS;
    }

    return w;
}

Score Evaluator::evaluate(const Bitboard& board) {
    double progress = getProgress(board);
    Weights w = getWeights(progress);

    Score mobility = evaluateMobility(board);
    Score corner = evaluateCorner(board);
    Score stability = evaluateStability(board);
    Score parity = evaluateCoinParity(board);

    return w.mobilityWeight * mobility +
           w.cornerWeight * corner +
           w.stabilityWeight * stability +
           w.parityWeight * parity;
}

} // namespace othello
