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
