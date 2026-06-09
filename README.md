# CodinGame Othello AI

[CodinGame Othello コンテスト](https://www.codingame.com/multiplayer/bot-programming/othello-1)向けのAIプロジェクトです。

## 言語別構成

```
.
├── cpp/                        # C++（本実装）
│   ├── src/                    #   ソースコード
│   │   ├── core/               #     ビットボード盤面表現
│   │   ├── evaluators/         #     評価関数
│   │   ├── search/             #     探索エンジン（NegaScout + 終盤完全読み）
│   │   └── utils/              #     置換表
│   ├── tests/                  #   Google Test テストコード
│   ├── submit.cpp              #   CodinGame提出用単一ファイル
│   └── CMakeLists.txt          #   ビルド設定
├── java/                       # Java（今後追加予定）
├── python/                     # Python（参考実装）
│   └── simple_greedy.py
├── docs/                       # ドキュメント
│   ├── 01_io_spec.md           #   入出力仕様書
│   └── 02_implementation_plan.md # 実装計画
├── README.md                   # このファイル
└── .gitignore
```

## ビルド方法（C++）

### 通常ビルド

```bash
cd cpp
mkdir build && cd build
cmake ..
make
```

実行ファイル: `build/othello_ai`

### テスト付きビルド

```bash
cd cpp
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make
ctest
```

## 提出方法

`cpp/submit.cpp` を CodinGame に提出してください。

## 実行方法

```bash
cd cpp/build
./othello_ai
```

## ライセンス

MIT License
