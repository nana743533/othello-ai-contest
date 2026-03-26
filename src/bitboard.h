#ifndef OTHello_AI_BITBOARD_H
#define OTHello_AI_BITBOARD_H

#include <cstdint>
#include <string>
#include <array>

namespace othello {

// 座標のビット位置を計算するマクロ
inline constexpr int coordinateToBit(char col, int row) {
    return (row - 1) * 8 + (col - 'a');
}

// 8方向のデルタ（方向ベクトル）
inline constexpr int DIRECTION_DELTAS[8][2] = {
    {-1, -1}, {0, -1}, {1, -1},   // 上方向: 左上、上、右上
    {-1,  0},          {1,  0},    // 左右
    {-1,  1}, {0,  1}, {1,  1}     // 下方向: 左下、下、右下
};

// 盤面のビットボード表現
class Bitboard {
public:
    uint64_t player;   // 自分の石の配置（1=石あり、0=空または相手）
    uint64_t opponent; // 相手の石の配置（1=石あり、0=空または自分）

    // コンストラクタ
    Bitboard() : player(0), opponent(0) {}
    Bitboard(uint64_t p, uint64_t o) : player(p), opponent(o) {}

    // 初期盤面を設定（オセロの初期配置）
    static Bitboard initial();

    // 盤面が空かどうか
    bool isEmpty() const { return (player | opponent) == 0; }

    // 指定位置のビットマスクを取得
    static uint64_t getMask(int x, int y);
    static uint64_t getMask(const std::string& coord);

    // 指定位置に自分の石があるか
    bool hasPlayerAt(int x, int y) const;
    bool hasPlayerAt(const std::string& coord) const;

    // 指定位置に相手の石があるか
    bool hasOpponentAt(int x, int y) const;
    bool hasOpponentAt(const std::string& coord) const;

    // 指定位置が空か
    bool isEmptyAt(int x, int y) const;
    bool isEmptyAt(const std::string& coord) const;

    // 指定位置に石を置く
    void placePlayer(int x, int y);
    void placePlayer(const std::string& coord);

    void placeOpponent(int x, int y);
    void placeOpponent(const std::string& coord);

    // 石を削除
    void removeAt(int x, int y);
    void removeAt(const std::string& coord);

    // 指定位置に石を置いて反転処理を行う（合法手チェックなし）
    Bitboard placeAndFlip(int x, int y) const;
    Bitboard placeAndFlip(const std::string& coord) const;

    // 合法手を生成する
    uint64_t getLegalMoves() const;

    // 指定位置が合法手か
    bool isLegalMove(int x, int y) const;
    bool isLegalMove(const std::string& coord) const;

    // 1方向の反転ビットを計算
    uint64_t getFlipsInDirection(int x, int y, int dx, int dy) const;

    // 全方向の反転ビットを計算
    uint64_t getFlips(int x, int y) const;

    // 空きマスの数
    int getEmptyCount() const;

    // 自分の石の数
    int getPlayerCount() const;

    // 相手の石の数
    int getOpponentCount() const;

    // 盤面を文字列で取得（デバッグ用）
    std::string toString() const;

    // 文字列から盤面をパース
    static Bitboard fromString(const std::string& boardStr, bool isBlack);

    // プレイヤーと相手を入れ替える
    Bitboard swap() const { return Bitboard(opponent, player); }

    // 比較演算子
    bool operator==(const Bitboard& other) const {
        return player == other.player && opponent == other.opponent;
    }

    bool operator!=(const Bitboard& other) const {
        return !(*this == other);
    }
};

// 定数：角のマスク
inline constexpr uint64_t CORNER_MASK =
    (1ULL << 0) | (1ULL << 7) | (1ULL << 56) | (1ULL << 63);

// 定数：星（C打ち）のマスク（角に隣接する危険な位置）
inline constexpr uint64_t X_SQUARES_MASK =
    (1ULL << 9) | (1ULL << 14) | (1ULL << 49) | (1ULL << 54);

// 定数：辺のマスク
inline constexpr uint64_t EDGE_MASK =
    0xFF000000000000FFULL |  // 上辺と下辺
    0x8181818181818181ULL;   // 左辺と右辺

} // namespace othello

#endif // OTHello_AI_BITBOARD_H
