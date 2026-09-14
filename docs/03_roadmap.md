# ロードマップ（ポインタ）

最終更新: 2026-09-15

## 計画の正本はここではない

**目的・完了条件・NA（O-1〜O-19）・基礎インプットの計画は、外部脳リポジトリ側が正本。**

| 内容 | 場所 |
|---|---|
| **手順と日程（①〜⑨の一本道）** | `~/ghq/github.com/nana743533/life-management/projects/othello-ai/roadmap.md` |
| 概要・戦略（なぜこの山か・進め方のルール） | 同 `projects/othello-ai/README.md` |
| **足りない基礎のチェックリスト（状態管理）** | 同 `projects/othello-ai/basics.md` |
| 入出力仕様 | [01_io_spec.md](01_io_spec.md) |
| 実装方針（ビットボード・NegaScout・評価関数・終盤） | [02_implementation_plan.md](02_implementation_plan.md) |
| **自分のコードで説明できない箇所**（O-2の成果物） | `04_gaps.md`（O-2 で作る） |

life-management 側の運用ルールで「プロジェクトの詳細は1箇所だけに置く（二重管理しない）」としているため、ここには計画を書かない。

## このプロジェクトの位置づけ（1行）

**到達力（数学・ML）を上げるための山。** 探索の最適化＝遂行力の訓練にしかならないので、**評価関数を自分で学習させる（Phase 2）まで必ず行く**ことが、この山を選んだ意味。判定の目安は「手を動かしていて式を立てる場面・データを当てはめる場面が出てこないなら、それは遂行力側の作業」。

## Phase の並び（詳細は正本へ）

| 手順 | 内容 | 時期 |
|---|---|---|
| ① | 環境を用意して、1回提出する | 9/21まで |
| ② | 自分のコードを説明できるようにする（→ `04_gaps.md`） | 9/15〜10/5 |
| ③ | **「強くなったか」を測れるようにする**（自己対戦フレームワーク） | 10/6〜10/26 |
| ④ | 機械学習の基礎を手で動かす（**11月は実装を止める**） | 10/27〜11/30 |
| — | **完全に休み**（準1級 受験 12/5） | 12/1〜12/7 |
| ⑤ | パターン評価関数を読んで、自分の特徴量を設計する | 12/8〜12/28 |
| ⑥ | 学習を回す | 12/29〜1/11 |
| ⑦ | **手書きの評価関数と戦わせる**（★本丸） | 1/12〜1/18 |
| ⑧ | Multi-ProbCut を入れる | 1/19〜2/15 |
| ⑨ | 定石と終盤を詰めて順位を上げる | 2/16〜 |

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
