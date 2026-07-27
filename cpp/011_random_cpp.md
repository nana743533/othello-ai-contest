# `01_random.cpp` で使う C++ まとめ

対象実装: [`01_random.cpp`](./01_random.cpp)

## 1. ヘッダ（`#include`）

| ヘッダ | 役割 | このファイルでの用途 |
|--------|------|----------------------|
| `<iostream>` | 標準入出力 | `cin`, `cout`, `endl`, `ios`, `getline` |
| `<string>` | 文字列 | `string`, `stoi`, `to_string` |
| `<vector>` | 可変長配列 | `vector` |
| `<random>` | 乱数 | `mt19937`, `random_device`, `uniform_int_distribution` |

`using namespace std;` は、これらを `std::cin` ではなく `cin` と短く書くためのものです。

## 2. 型・クラス

| 名前 | 何か | このファイルでの意味 |
|------|------|----------------------|
| `string` | 文字列クラス | 1行入力や座標・MSG を入れる |
| `vector<string>` | 文字列の動的配列 | 合法手リスト |
| `mt19937` | 乱数エンジン | ランダムに手を選ぶための乱数源 |
| `random_device` | 種（シード）生成器 | `mt19937` の初期化に使う |
| `uniform_int_distribution<int>` | 一様分布 | `0`〜`合法手数-1` の整数をランダムに取る |
| `ios` | 入出力の共通設定まわり | `sync_with_stdio` を呼ぶ |
| `int` | 整数 | プレイヤーID、盤サイズ、手番など |

## 3. グローバルオブジェクト（入出力）

| 名前 | 何か | 役割 |
|------|------|------|
| `cin` | 標準入力ストリーム | ジャッジからの入力を読む |
| `cout` | 標準出力ストリーム | 指し手を出す |
| `endl` | 改行＋flush | 行末を出して出力を送る |
| `nullptr` | ヌルポインタ | `cin.tie(nullptr)` で紐づけ解除 |

## 4. 関数・メソッド

### 入出力まわり

| 名前 | 種類 | 意味 |
|------|------|------|
| `ios::sync_with_stdio(false)` | 静的関数 | C の入出力との同期を切る |
| `cin.tie(nullptr)` | メソッド | `cin` 前の `cout` 自動 flush を外す |
| `getline(cin, line)` | 関数 | 1行読んで `line` に入れる |
| `operator<<`（`cout << ...`） | 演算子 | 出力する |

### 文字列まわり

| 名前 | 種類 | 意味 |
|------|------|------|
| `stoi(line)` | 関数 | 文字列 → 整数（`"0"` → `0`） |
| `to_string(n)` | 関数 | 整数 → 文字列（`1` → `"1"`） |
| `s.empty()` | メソッド | 空文字か |
| `s.back()` | メソッド | 末尾の1文字 |
| `s.pop_back()` | メソッド | 末尾を削除 |

### `vector` まわり

| 名前 | 種類 | 意味 |
|------|------|------|
| `reserve(n)` | メソッド | あらかじめ容量を確保 |
| `push_back(x)` | メソッド | 末尾に追加 |
| `empty()` | メソッド | 要素が0個か |
| `size()` | メソッド | 要素数 |
| `legalMoves[i]` | 添字 | i番目の要素 |

### 乱数まわり

| 名前 | 種類 | 意味 |
|------|------|------|
| `random_device{}()` | 呼び出し | 乱数の種を取る |
| `mt19937 rng(...)` | コンストラクタ | 乱数エンジンを作る |
| `dist(rng)` | 呼び出し | 分布に従って乱数を1つ取る |

### このファイル独自

| 名前 | 種類 | 意味 |
|------|------|------|
| `trim(s)` | 自作関数 | 行末の空白・`\r` などを削る |
| `main()` | エントリポイント | プログラムの開始関数 |

### その他

| 名前 | 意味 |
|------|------|
| `static_cast<int>(...)` | 型変換（ここでは `size_t` → `int`） |
| `(void)playerId` | 「未使用変数」警告を抑える書き方（値は使っていない） |

## 5. Python とのざっくり対応

| C++ | Python っぽいイメージ |
|-----|------------------------|
| `string line` | `line = ""` |
| `getline(cin, line)` | `line = input()` |
| `stoi(line)` | `int(line)` |
| `to_string(n)` | `str(n)` |
| `vector<string>` | `list[str]` |
| `push_back` | `append` |
| `cout << x << endl` | `print(x)` |
| `mt19937` + `uniform_int_distribution` | `random.choice` / `random.randrange` |

## 6. 補足

このファイルに出てくる「クラスっぽいもの」は主に `string`・`vector`・乱数系（`mt19937` など）です。  
自作クラスはなく、自作は `trim` と `main` だけです。
