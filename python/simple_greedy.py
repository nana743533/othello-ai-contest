#!/usr/bin/env python3
"""
シンプルな位置重み付けグリーディーアルゴリズム

PythonコードをC++に移植したもの: src/evaluators/simple_evaluator.{h,cpp}

アルゴリズム:
1. score(a) で座標（例: "c3"）から重みを取得
2. max(a, key=score)で合法手の中から最も重みが高い手を選ぶ
"""

W = [
    [100, -20,  10,   5,   5,  10, -20, 100],
    [-20, -50,  -2,  -2,  -2,  -2, -50, -20],
    [ 10,  -2,   1,   1,   1,   1,  -2,  10],
    [  5,  -2,   1,   0,   0,   1,  -2,   5],
    [  5,  -2,   1,   0,   0,   1,  -2,   5],
    [ 10,  -2,   1,   1,   1,   1,  -2,  10],
    [-20, -50,  -2,  -2,  -2,  -2, -50, -20],
    [100, -20,  10,   5,   5,  10, -20, 100]
]

def score(a):
    """座標（例: "c3"）から重みを取得"""
    return W[int(a[1]) - 1][ord(a[0]) - 97]

def main():
    pid = input()
    n = int(input())

    while True:
        b = [input() for _ in range(n)]
        m = int(input())
        a = [input() for _ in range(m)]
        print(max(a, key=score))

if __name__ == "__main__":
    main()
