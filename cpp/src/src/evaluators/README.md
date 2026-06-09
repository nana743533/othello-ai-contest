# Evaluators

評価関数の実装を含むディレクトリ。

## ファイル構成

### `evaluator.{h,cpp}`
動的評価関数（4要素評価）。進行度に応じて重みを変化させる。

- **Mobility**: 着手可能数の差
- **Corner**: 角の確保とX打ちのペナルティ
- **Stability**: 確定石の数
- **Coin Parity**: 石数差

序盤はMobilityとCornerを重視し、終盤はStabilityとParityを重視する。

### `simple_evaluator.{h,cpp}`
位置重み付けによるシンプルな評価関数（Pythonコードの移植）。

盤面の各位置に固定の重みを付け、その合計で評価する。

```cpp
// 重み行列
constexpr int WEIGHTS[8][8] = {
    {100, -20,  10,   5,   5,  10, -20, 100},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    { 10,  -2,   1,   1,   1,   1,  -2,  10},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    { 10,  -2,   1,   1,   1,   1,  -2,  10},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    {100, -20,  10,   5,   5,  10, -20, 100}
};
```

## 使い方

```cpp
#include "evaluators/evaluator.h"
#include "evaluators/simple_evaluator.h"
#include "core/bitboard.h"

using namespace othello;

Bitboard board = Bitboard::initial();

// 動的評価関数
Score score = Evaluator::evaluate(board);

// シンプル評価関数
int simpleScore = SimpleEvaluator::evaluate(board);

// グリーディーで最善手を選択
std::string bestMove = SimpleEvaluator::chooseBestMove(board);
```
