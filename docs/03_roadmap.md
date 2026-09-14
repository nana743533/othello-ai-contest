# ロードマップ（ポインタ）

最終更新: 2026-09-15

## 計画の正本はここではない

**目的・完了条件・NA（O-1〜O-19）・基礎インプットの計画は、外部脳リポジトリ側が正本。**

| 内容 | 場所 |
|---|---|
| **タスクと日程（学習L／実装B／検証V／事務D・週次表・マイルストーン）** | `~/ghq/github.com/nana743533/life-management/projects/othello-ai/roadmap.md` |
| 概要・戦略（なぜこの山か・進め方のルール） | 同 `projects/othello-ai/README.md` |
| **足りない基礎のチェックリスト（状態管理）** | 同 `projects/othello-ai/basics.md` |
| 入出力仕様 | [01_io_spec.md](01_io_spec.md) |
| 実装方針（ビットボード・NegaScout・評価関数・終盤） | [02_implementation_plan.md](02_implementation_plan.md) |
| **自分のコードで説明できない箇所**（O-2の成果物） | `04_gaps.md`（O-2 で作る） |

life-management 側の運用ルールで「プロジェクトの詳細は1箇所だけに置く（二重管理しない）」としているため、ここには計画を書かない。

## このプロジェクトの位置づけ（1行）

**到達力（数学・ML）を上げるための山。** 探索の最適化＝遂行力の訓練にしかならないので、**評価関数を自分で学習させる（Phase 2）まで必ず行く**ことが、この山を選んだ意味。判定の目安は「手を動かしていて式を立てる場面・データを当てはめる場面が出てこないなら、それは遂行力側の作業」。

## Phase の並び（詳細は正本へ）

| 時期 | 内容 | タスク |
|---|---|---|
| 9/15〜9/21 | 提出して現在地を測る／自分のコードの根拠を洗い出す | L1・V1・D1〜D2 |
| 9月末〜10月 | 探索を**理解して**仕上げる＋自己対戦フレームワーク | L2〜L4・B1・B2・V2 |
| 11月 | **実装を止めて学習に寄せる**（準1級の追い込み期） | L5〜L7 |
| **12/1〜12/7** | **完全に休み**（準1級 受験 12/5） | — |
| 12月中旬〜1月中旬 | **評価関数を学習させる**（本体） | L8・B3〜B5・V3 |
| 1月下旬〜2月中旬 | Multi-ProbCut（統計の実地） | L9・B6・V4 |
| 2月下旬〜 | 定石・終盤深化・順位上げ | B7・V5 |

※ 統計検定準1級（2026/12/5 受験）と並行。**11月は準1級を優先し、こちらは実装を止めてインプットに寄せる。**

## CodinGame の事実（2026-09-15 確認・開発時に要るぶん）

- URL: https://www.codingame.com/multiplayer/bot-programming/othello-1
- **リーグ制は無い。単一リーダーボード**（参加者638人）。Boss は darkhorse64 作
- 即敗北: 盤外・石のあるマスへの出力／時間内に応答しない／認識できないコマンド
- 思考時間: 初手2000ms／以降150ms ※**自作メモ由来。O-1 で実測して裏を取り、[01_io_spec.md](01_io_spec.md) を直す**
- ステートメントに referee へのリンクあり → ローカル対戦は [cg-brutaltester](https://github.com/dreignier/cg-brutaltester) と組み合わせる（O-6）

## 現在地（2026-09-15）

最終コミット 2026-07-28。**CodinGame への提出は未実施**（＝順位がまだ無い）。

- `cpp/src/` — `core/bitboard`、`search/search`（NegaScout）、`search/endgame_solver`、`utils/transposition_table`、`evaluators/`
- `cpp/tests/` — Google Test、`cpp/submit.cpp` — 提出用の単一ファイル
- `python/` — 010_random → 020_greedy → 030_minimax → 040_path_score → 041_endgame

8/7 の大学Javaレポートで、終盤完全読み・move ordering・mobility評価を `Negamax.java` に移植し、自己対戦20局で旧版に18勝2敗している。
