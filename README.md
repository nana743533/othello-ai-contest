# CodinGame Othello AI ドキュメント

このディレクトリには、[CodinGame Othello コンテスト](https://www.codingame.com/multiplayer/bot-programming/othello-1)向けのAI開発に関するドキュメントが含まれています。

## ドキュメント一覧

| ファイル | 内容 |
| :--- | :--- |
| [01_io_spec.md](./01_io_spec.md) | コンテストの入出力仕様書（ゲームルール、入出力フォーマット、制約条件） |
| [02_implementation_plan.md](./02_implementation_plan.md) | C++による実装方針提案書（ビットボード、NegaScout、評価関数、終盤完全読み） |

## 実装方針の概要

強力なオセロAIを構築するために、以下の4本柱を採用します。

1. **ビットボード** — 64ビット整数2つで盤面を表現し、ビット演算で高速に合法手生成・石の反転を処理します。
2. **NegaScout法 + 反復深化** — Alpha-Beta法を改良した探索アルゴリズムと、時間制限を最大活用する反復深化を組み合わせます。
3. **動的評価関数** — Mobility（着手可能数）、Corner（角の確保）、Stability（確定石）を進行度に応じた重みで評価します。
4. **終盤完全読み** — 残り約15マスで評価関数を捨て、最終石差を完全に読み切ります。

## 実装構成

### ファイル構成

```
.
├── src/
│   ├── bitboard.h/cpp           # ビットボード盤面表現
│   ├── evaluator.h/cpp          # 評価関数
│   ├── search.h/cpp             # 探索エンジン（NegaScout）
│   ├── transposition_table.h/cpp # 置換表
│   ├── endgame_solver.h/cpp     # 終盤ソルバー
│   └── main.cpp                 # エントリーポイント、入出力
├── tests/                       # Google Test テストコード
├── submit.cpp                   # CodinGame提出用単一ファイル
├── CMakeLists.txt               # ビルド設定
├── 01_io_spec.md                # 入出力仕様書
├── 02_implementation_plan.md    # 実装計画
└── README.md                    # このファイル
```

## ビルド方法

### 通常ビルド

```bash
mkdir build && cd build
cmake ..
make
```

実行ファイル: `build/othello_ai`

### テスト付きビルド

```bash
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make
ctest
```

## 提出方法

`submit.cpp` を CodinGame に提出してください。これは単一ファイルで全ての機能を含んでいます。

## 実行方法

ローカルでテストする場合：

```bash
# 標準入力からゲーム状態を読み込む
./build/othello_ai
```

入力形式は `01_io_spec.md` を参照してください。

## パフォーマンス

- 時間制限: 150ms/ターン（2ターン目以降）
- 探索深さ: 反復深化で動的に調整
- 終盤: 残り15マス以下で完全読み

## ライセンス

MIT License
