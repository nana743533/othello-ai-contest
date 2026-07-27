#!/usr/bin/env python3
"""
CodinGame Othello AI - ランダムプレイヤー

入出力仕様（docs/01_io_spec.md）に従い、合法手の中からランダムに一手を選びます。
"""

import random
import sys


def main():
    player_id = int(input())
    board_size = int(input())

    while True:
        # 盤面の状態（board_size 行）を読み飛ばす
        for _ in range(board_size):
            input()

        # 合法手の数とリスト
        action_count = int(input())
        legal_moves = [input().strip() for _ in range(action_count)]

        if not legal_moves:
            print("pass")
        else:
            print(random.choice(legal_moves))

        sys.stdout.flush()


if __name__ == "__main__":
    main()
