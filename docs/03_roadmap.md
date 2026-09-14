# ロードマップ（ポインタ）

最終更新: 2026-09-15

## 計画の正本はここではない

**目的・完了条件・NA（O-1〜O-19）・基礎インプットの計画は、外部脳リポジトリ側が正本。**

| 内容 | 場所 |
|---|---|
| **計画・NA・スケジュール・チェックポイント** | `~/ghq/github.com/nana743533/life-management/projects/othello-ai/README.md` |
| **足りない基礎のチェックリスト（状態管理）** | 同 `projects/othello-ai/basics.md` |
| 入出力仕様 | [01_io_spec.md](01_io_spec.md) |
| 実装方針（ビットボード・NegaScout・評価関数・終盤） | [02_implementation_plan.md](02_implementation_plan.md) |
| **自分のコードで説明できない箇所**（O-2の成果物） | `04_gaps.md`（O-2 で作る） |

life-management 側の運用ルールで「プロジェクトの詳細は1箇所だけに置く（二重管理しない）」としているため、ここには計画を書かない。

## このプロジェクトの位置づけ（1行）

**到達力（数学・ML）を上げるための山。** 探索の最適化＝遂行力の訓練にしかならないので、**評価関数を自分で学習させる（Phase 2）まで必ず行く**ことが、この山を選んだ意味。判定の目安は「手を動かしていて式を立てる場面・データを当てはめる場面が出てこないなら、それは遂行力側の作業」。

## Phase の並び（詳細は正本へ）

| Phase | 内容 | NA | 時期 |
|---|---|---|---|
| 0 | 提出して現在地を測る／コードの根拠を洗い出す | O-1・O-2 | 9/15〜9/21 |
| 1 | 探索を**理解して**仕上げる＋自己対戦フレームワーク | O-3〜O-8 | 9月末〜10月 |
| 2 | **評価関数を学習させる**（本体） | O-9〜O-15 | 10月下旬〜1月中旬 |
| 3 | Multi-ProbCut（統計の実地） | O-16〜O-18 | 1月下旬〜2月 |
| 4 | 定石・終盤深化・順位上げ | O-19 | 2月〜 |

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
