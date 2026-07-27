#!/usr/bin/env python3
"""
041_endgame.py の終盤完全読みレイテンシ計測

ランダム対戦で空きマス数が target になるまで進め、
choose_endgame_move の所要時間を測る。
"""

from __future__ import annotations

import argparse
import importlib.util
import random
import statistics
import time
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("041_endgame.py")


def load_endgame_module():
    """041_endgame.py をモジュールとして読み込む"""
    spec = importlib.util.spec_from_file_location("endgame041", MODULE_PATH)
    mod = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    spec.loader.exec_module(mod)
    return mod


def initial_board():
    """標準の初期盤面を返す"""
    return [
        "........",
        "........",
        "........",
        "...01...",
        "...10...",
        "........",
        "........",
        "........",
    ]


def play_until_empty(mod, target_empty: int, seed: int):
    """
    ランダム対戦で空きマスが target_empty 以下になるまで進める。
    戻り値: (board, player) / 途中終局なら (None, player)
    """
    rng = random.Random(seed)
    board = initial_board()
    player = 0
    while mod.count_empty(board) > target_empty:
        moves = mod.get_legal_moves(board, player)
        if not moves:
            player = 1 - player
            moves = mod.get_legal_moves(board, player)
            if not moves:
                return None, player
        board = mod.apply_move(board, rng.choice(moves), player)
        player = 1 - player
    return board, player


def resolve_to_play(mod, board, player):
    """パス局面なら相手番に回し、合法手がある手番を返す"""
    if mod.get_legal_moves(board, player):
        return player
    other = 1 - player
    if mod.get_legal_moves(board, other):
        return other
    return None


def measure_once(mod, board, me, repeats: int) -> tuple[str, int, list[float]]:
    """choose_endgame_move を repeats 回測り、(手, 評価値, ms一覧) を返す"""
    legal = mod.get_legal_moves(board, me)
    times_ms = []
    move, score = legal[0], 0
    for _ in range(repeats):
        t0 = time.perf_counter()
        move, score = mod.choose_endgame_move(board, legal, me)
        times_ms.append((time.perf_counter() - t0) * 1000.0)
    return move, score, times_ms


def run_bench(args) -> int:
    """空きマス・シードごとに終盤読みのレイテンシを計測して表示する"""
    mod = load_endgame_module()
    empties = [int(x) for x in args.empties.split(",")]
    seeds = list(range(args.seeds))

    print("041_endgame endgame latency benchmark")
    print(f"module: {MODULE_PATH}")
    print(f"ENDGAME_EMPTY={mod.ENDGAME_EMPTY}")
    print(f"empties={empties} seeds=0..{args.seeds - 1} repeats={args.repeats}")
    print(f"CodinGame turn limit (after turn1): 150 ms")
    print()
    print(f"{'empty':>5} {'seed':>4} {'legal':>5} {'me':>3} "
          f"{'move':>4} {'score':>6} {'avg_ms':>10} {'max_ms':>10}")

    summary: dict[int, list[float]] = {e: [] for e in empties}

    for empty in empties:
        for seed in seeds:
            board, player = play_until_empty(mod, empty, seed)
            if board is None:
                print(f"{empty:5d} {seed:4d}   - game ended early")
                continue
            if mod.count_empty(board) != empty:
                # 厳密一致のみ計測（途中終局で空きが減りすぎたケースを除外）
                continue
            me = resolve_to_play(mod, board, player)
            if me is None:
                print(f"{empty:5d} {seed:4d}   - no legal moves")
                continue
            legal = mod.get_legal_moves(board, me)
            move, score, times_ms = measure_once(mod, board, me, args.repeats)
            avg_ms = statistics.mean(times_ms)
            max_ms = max(times_ms)
            summary[empty].append(avg_ms)
            print(
                f"{empty:5d} {seed:4d} {len(legal):5d} {me:3d} "
                f"{move:>4} {score:+6d} {avg_ms:10.1f} {max_ms:10.1f}"
            )

    print()
    print("summary (avg_ms over seeds)")
    print(f"{'empty':>5} {'n':>3} {'mean_ms':>10} {'median_ms':>10} "
          f"{'min_ms':>10} {'max_ms':>10} {'>150ms':>7}")
    for empty in empties:
        vals = summary[empty]
        if not vals:
            print(f"{empty:5d}   0          -          -          -          -       -")
            continue
        over = sum(1 for v in vals if v > 150.0)
        print(
            f"{empty:5d} {len(vals):3d} {statistics.mean(vals):10.1f} "
            f"{statistics.median(vals):10.1f} {min(vals):10.1f} "
            f"{max(vals):10.1f} {over:7d}"
        )
    return 0


def main():
    """CLI 引数を解釈してベンチマークを実行する"""
    parser = argparse.ArgumentParser(
        description="041_endgame.py 終盤完全読みのレイテンシ計測"
    )
    parser.add_argument(
        "--empties",
        default="12,10,8,6",
        help="計測する空きマス数（カンマ区切り）",
    )
    parser.add_argument(
        "--seeds",
        type=int,
        default=5,
        help="各空きマス数で使う乱数シード数（0..N-1）",
    )
    parser.add_argument(
        "--repeats",
        type=int,
        default=1,
        help="同一局面の計測回数",
    )
    args = parser.parse_args()
    raise SystemExit(run_bench(args))


if __name__ == "__main__":
    main()
