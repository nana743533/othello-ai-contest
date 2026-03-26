#include "bitboard.h"
#include "evaluator.h"
#include "search.h"
#include "endgame_solver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace othello;

// グローバル変数
int g_playerId = -1;       // 0=黒(先手), 1=白(後手)
int g_boardSize = 8;
Bitboard g_currentBoard;

// プロトタイプ宣言
void parseInitialInput();
void parseTurnInput(std::vector<std::string>& legalMoves);
std::string chooseMove(const std::vector<std::string>& legalMoves);

int main() {
    try {
        // 初期入力のパース
        parseInitialInput();

        // ゲームループ
        while (std::cin) {
            std::vector<std::string> legalMoves;
            parseTurnInput(legalMoves);

            // 最善手を選択
            std::string bestMove = chooseMove(legalMoves);

            // 出力（座標のみ）
            std::cout << bestMove << std::endl;
        }

    } catch (const std::exception& e) {
        return 1;
    }

    return 0;
}

void parseInitialInput() {
    // プレイヤーID
    std::string line;
    if (!std::getline(std::cin, line)) return;
    g_playerId = std::stoi(line);

    // 盤面サイズ
    if (!std::getline(std::cin, line)) return;
    g_boardSize = std::stoi(line);
}

void parseTurnInput(std::vector<std::string>& legalMoves) {
    // 盤面のパース
    std::string boardStr;
    for (int i = 0; i < g_boardSize; i++) {
        std::string line;
        if (!std::getline(std::cin, line)) return;
        boardStr += line;
    }

    // Expert Modeの追加入力（条件付き）
    // 注: 前ターンでEXPERTを出力していた場合、ここに追加の入力が来る
    // 今回は盤面から状態を構築するため、スキップ可能

    // 合法手の数
    std::string line;
    if (!std::getline(std::cin, line)) return;
    int actionCount = std::stoi(line);

    // 合法手のリスト
    legalMoves.clear();
    for (int i = 0; i < actionCount; i++) {
        if (!std::getline(std::cin, line)) return;
        // 余分な空白を削除
        while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
            line.pop_back();
        }
        legalMoves.push_back(line);
    }

    // 盤面を構築（黒=Player0, 白=Player1）
    bool isBlack = (g_playerId == 0);
    g_currentBoard = Bitboard::fromString(boardStr, isBlack);
}

std::string chooseMove(const std::vector<std::string>& legalMoves) {
    // 合法手がない場合（パス）
    if (legalMoves.empty()) {
        return "pass";
    }

    // 終盤ソルバーの適用判定
    if (EndgameSolver::isApplicable(g_currentBoard, 15)) {
        std::string move = EndgameSolver::solve(g_currentBoard);

        // 合法手チェック
        for (const auto& legal : legalMoves) {
            if (move == legal) {
                return move;
            }
        }

        // ソルバーの結果が非法手なら探索フォールバック
    }

    // 探索エンジンで最善手を探索
    SearchEngine engine;
    SearchResult result = engine.search(g_currentBoard, 140); // 10ms余裕を持つ

    // 合法手チェック
    for (const auto& legal : legalMoves) {
        if (result.move == legal) {
            return result.move;
        }
    }

    // 探索結果が非法手なら、合法手の最初を返す
    return legalMoves[0];
}
