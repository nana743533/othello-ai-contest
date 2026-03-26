#include "search.h"
#include <algorithm>
#include <climits>

namespace othello {

SearchEngine::SearchEngine()
    : minDepth_(1), maxDepth_(20), nodeCount_(0), timeoutMs_(150)
{
}

SearchResult SearchEngine::search(const Bitboard& board, int timeoutMs) {
    startTime_ = std::chrono::steady_clock::now();
    timeoutMs_ = timeoutMs;
    nodeCount_ = 0;

    // 合法手を取得
    uint64_t legalMoves = board.getLegalMoves();

    // 合法手がない場合（パス）
    if (legalMoves == 0) {
        SearchResult result;
        result.move = "pass";
        result.score = Evaluator::evaluate(board);
        result.depth = 0;
        result.nodes = 0;
        return result;
    }

    // 反復深化で探索
    SearchResult result = iterativeDeepening(board, timeoutMs);

    return result;
}

SearchResult SearchEngine::iterativeDeepening(const Bitboard& board, int timeoutMs) {
    SearchResult bestResult;
    std::vector<std::string> legalMoves;

    // 合法手を文字列リストに変換
    uint64_t moves = board.getLegalMoves();
    while (moves) {
        int lsb = __builtin_ctzll(moves);
        legalMoves.push_back(bitToCoord(lsb));
        moves &= (moves - 1);
    }

    // 深さ1から順に探索
    for (int depth = minDepth_; depth <= maxDepth_; depth++) {
        if (isTimeUp()) break;

        SearchResult currentResult;
        currentResult.depth = depth;

        Score alpha = -Evaluator::MAX_SCORE;
        Score beta = Evaluator::MAX_SCORE;

        // 各手を探索
        for (const auto& move : legalMoves) {
            if (isTimeUp()) break;

            Bitboard nextBoard = board.placeAndFlip(move);

            // 相手の番（盤面を入れ替えて自分の視点で評価）
            Score score = -negascout(nextBoard.swap(), depth - 1, -beta, -alpha, false);

            if (score > alpha) {
                alpha = score;
                currentResult.move = move;
                currentResult.score = score;
            }

            // ベータカット
            if (alpha >= beta) break;
        }

        currentResult.nodes = nodeCount_;
        bestResult = currentResult;

        // 時間チェック
        if (isTimeUp()) break;
    }

    bestResult.nodes = nodeCount_;
    return bestResult;
}

Score SearchEngine::negascout(const Bitboard& board, int depth, Score alpha, Score beta, bool isRoot) {
    nodeCount_++;

    // 葉ノードまたは深さ制限
    if (depth == 0) {
        return Evaluator::evaluate(board);
    }

    // 合法手を取得
    uint64_t legalMoves = board.getLegalMoves();

    // 合法手がない場合（パス）
    if (legalMoves == 0) {
        // 相手もパスならゲーム終了
        Bitboard swapped = board.swap();
        if (swapped.getLegalMoves() == 0) {
            // ゲーム終了：石数差を返す
            int diff = board.getPlayerCount() - board.getOpponentCount();
            if (diff > 0) return Evaluator::MAX_SCORE - diff;
            if (diff < 0) return -Evaluator::MAX_SCORE + diff;
            return 0;
        }
        // 相手の番へ（盤面を入れ替えずに探索継続）
        return -negascout(swapped, depth - 1, -beta, -alpha, false);
    }

    // 手の順序付け
    std::vector<std::string> moves;
    while (legalMoves) {
        int lsb = __builtin_ctzll(legalMoves);
        moves.push_back(bitToCoord(lsb));
        legalMoves &= (legalMoves - 1);
    }

    // Move Ordering: 角を優先
    std::sort(moves.begin(), moves.end(), [](const std::string& a, const std::string& b) {
        // 角のビット位置: a1=0, h1=7, a8=56, h8=63
        std::vector<int> corners = {0, 7, 56, 63};
        int aBit = coordToBit(a);
        int bBit = coordToBit(b);
        bool aIsCorner = std::find(corners.begin(), corners.end(), aBit) != corners.end();
        bool bIsCorner = std::find(corners.begin(), corners.end(), bBit) != corners.end();
        if (aIsCorner && !bIsCorner) return true;
        if (!aIsCorner && bIsCorner) return false;
        return aBit < bBit; // その他はビット位置順
    });

    // 最初の手をフルウィンドウで探索
    Bitboard firstBoard = board.placeAndFlip(moves[0]);
    Score score = -negascout(firstBoard.swap(), depth - 1, -beta, -alpha, false);

    if (score >= beta) return score; // ベータカット
    alpha = std::max(alpha, score);

    // 残りの手をNegaScoutウィンドウで探索
    for (size_t i = 1; i < moves.size(); i++) {
        if (isTimeUp() && !isRoot) break;

        Bitboard nextBoard = board.placeAndFlip(moves[i]);

        // Null Window Search
        Score bound = -negascout(nextBoard.swap(), depth - 1, -alpha - 1, -alpha, false);

        // Null Windowが_fail high_したら再探索
        if (bound > alpha && bound < beta) {
            score = -negascout(nextBoard.swap(), depth - 1, -beta, -bound, false);
        } else {
            score = bound;
        }

        if (score > alpha) alpha = score;
        if (alpha >= beta) break; // ベータカット
    }

    return alpha;
}

std::vector<std::pair<Score, std::string>> SearchEngine::orderMoves(
    const Bitboard& board,
    const std::vector<std::string>& moves)
{
    std::vector<std::pair<Score, std::string>> ordered;

    for (const auto& move : moves) {
        // 角は最優先
        int bit = coordToBit(move);
        Score priority = 0;

        if ((CORNER_MASK & (1ULL << bit)) != 0) {
            priority = 1000;
        } else if ((X_SQUARES_MASK & (1ULL << bit)) != 0) {
            priority = -1000; // X-Squareは回避
        }

        ordered.push_back({priority, move});
    }

    std::sort(ordered.begin(), ordered.end(),
        [](const auto& a, const auto& b) { return a.first > b.first; });

    return ordered;
}

bool SearchEngine::isTimeUp() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_).count();
    return elapsed >= timeoutMs_;
}

int SearchEngine::coordToBit(const std::string& coord) {
    if (coord.length() < 2) return 0;
    int x = coord[0] - 'a';
    int y = coord[1] - '1';
    return y * 8 + x;
}

std::string SearchEngine::bitToCoord(int bit) {
    int x = bit % 8;
    int y = bit / 8;
    std::string coord;
    coord += ('a' + x);
    coord += ('1' + y);
    return coord;
}

void SearchEngine::resetStats() {
    nodeCount_ = 0;
}

} // namespace othello
