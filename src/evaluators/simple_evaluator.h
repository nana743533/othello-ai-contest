#ifndef OTHello_AI_SIMPLE_EVALUATOR_H
#define OTHello_AI_SIMPLE_EVALUATOR_H

#include "../core/bitboard.h"
#include <string>

namespace othello {

// 位置重み付けによるシンプルな評価関数
// PythonコードをC++に移植したもの
// W = [
//     [100,-20, 10,  5,  5, 10,-20,100],
//     [-20,-50, -2, -2, -2, -2,-50,-20],
//     [ 10, -2,  1,  1,  1,  1, -2, 10],
//     [  5, -2,  1,  0,  0,  1, -2,  5],
//     [  5, -2,  1,  0,  0,  1, -2,  5],
//     [ 10, -2,  1,  1,  1,  1, -2, 10],
//     [-20,-50, -2, -2, -2, -2,-50,-20],
//     [100,-20, 10,  5,  5, 10,-20,100]
// ]
class SimpleEvaluator {
public:
    // 座標（例: "c3"）から重みを取得
    static int getWeight(const std::string& coord);

    // 合法手の中から最も重みが高い手を選ぶ（グリーディー）
    static std::string chooseBestMove(const Bitboard& board);

    // 盤面評価（重みの合計）
    static int evaluate(const Bitboard& board);

private:
    // 重み行列（8x8）
    static constexpr int WEIGHTS[8][8] = {
        {100, -20,  10,   5,   5,  10, -20, 100},  // A8-H8 (row 7)
        {-20, -50,  -2,  -2,  -2,  -2, -50, -20},  // A7-H7 (row 6)
        { 10,  -2,   1,   1,   1,   1,  -2,  10},  // A6-H6 (row 5)
        {  5,  -2,   1,   0,   0,   1,  -2,   5},  // A5-H5 (row 4)
        {  5,  -2,   1,   0,   0,   1,  -2,   5},  // A4-H4 (row 3)
        { 10,  -2,   1,   1,   1,   1,  -2,  10},  // A3-H3 (row 2)
        {-20, -50,  -2,  -2,  -2,  -2, -50, -20},  // A2-H2 (row 1)
        {100, -20,  10,   5,   5,  10, -20, 100}   // A1-H1 (row 0)
    };
};

} // namespace othello

#endif // OTHello_AI_SIMPLE_EVALUATOR_H
