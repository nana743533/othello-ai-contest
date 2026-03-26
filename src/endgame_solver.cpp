#include "endgame_solver.h"
#include <algorithm>

namespace othello {

bool EndgameSolver::isApplicable(const Bitboard& board, int threshold) {
    return board.getEmptyCount() <= threshold;
}

std::string EndgameSolver::solve(const Bitboard& board) {
    // 黒の視点で探索
    int32_t bestScore = -INF;
    std::string bestMove = "pass";

    // 合法手を取得
    uint64_t legalMoves = board.getLegalMoves();

    // パスのチェック
    if (legalMoves == 0) {
        // 相手にパスして探索
        Bitboard passed = board.swap();
        if (passed.getLegalMoves() == 0) {
            // 双方パス＝ゲーム終了
            return "pass";
        }
        int32_t score = -search(passed, false);
        return "pass"; // パスしか選択肢がない
    }

    // 各手を探索
    while (legalMoves) {
        int lsb = __builtin_ctzll(legalMoves);
        std::string move = bitToCoord(lsb);

        Bitboard nextBoard = board.placeAndFlip(move);
        int32_t score = -search(nextBoard.swap(), false);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        legalMoves &= (legalMoves - 1);
    }

    return bestMove;
}

int32_t EndgameSolver::finalScore(const Bitboard& board) {
    return board.getPlayerCount() - board.getOpponentCount();
}

int32_t EndgameSolver::search(Bitboard board, bool isBlack) {
    // 合法手を取得
    uint64_t legalMoves = board.getLegalMoves();

    // パスの処理
    if (legalMoves == 0) {
        if (!handlePass(board, isBlack)) {
            // ゲーム終了
            return finalScore(board);
        }
        legalMoves = board.getLegalMoves();
    }

    // 葉ノード（盤面が埋まった）
    if (legalMoves == 0) {
        return finalScore(board);
    }

    int32_t bestScore = -INF;

    // 各手を探索
    while (legalMoves) {
        int lsb = __builtin_ctzll(legalMoves);
        Bitboard nextBoard = board.placeAndFlip(lsb);

        // 相手の番
        int32_t score = -search(nextBoard.swap(), !isBlack);

        if (score > bestScore) {
            bestScore = score;
        }

        legalMoves &= (legalMoves - 1);
    }

    return bestScore;
}

int8_t EndgameSolver::searchWLD(Bitboard board, bool isBlack) {
    // 合法手を取得
    uint64_t legalMoves = board.getLegalMoves();

    // パスの処理
    if (legalMoves == 0) {
        if (!handlePass(board, isBlack)) {
            // ゲーム終了
            int32_t score = finalScore(board);
            if (score > 0) return 1;
            if (score < 0) return -1;
            return 0;
        }
        legalMoves = board.getLegalMoves();
    }

    // 葉ノード
    if (legalMoves == 0) {
        int32_t score = finalScore(board);
        if (score > 0) return 1;
        if (score < 0) return -1;
        return 0;
    }

    // まず勝ちを探す
    int8_t result = -1;
    while (legalMoves) {
        int lsb = __builtin_ctzll(legalMoves);
        Bitboard nextBoard = board.placeAndFlip(lsb);

        int8_t score = -searchWLD(nextBoard.swap(), !isBlack);

        if (score == 1) return 1; // 勝ちが見つかったら即座に返す
        if (score == 0) result = 0; // 引き分けを記録

        legalMoves &= (legalMoves - 1);
    }

    return result;
}

bool EndgameSolver::handlePass(Bitboard& board, bool& isBlack) {
    // 相手の合法手をチェック
    Bitboard swapped = board.swap();
    uint64_t opponentMoves = swapped.getLegalMoves();

    if (opponentMoves == 0) {
        // 双方パス＝ゲーム終了
        return false;
    }

    // パスして相手の番へ
    board = swapped;
    isBlack = !isBlack;
    return true;
}

std::string EndgameSolver::bitToCoord(int bit) {
    int x = bit % 8;
    int y = bit / 8;
    std::string coord;
    coord += ('a' + x);
    coord += ('1' + y);
    return coord;
}

} // namespace othello
