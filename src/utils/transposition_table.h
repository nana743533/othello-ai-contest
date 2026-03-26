#ifndef OTHello_AI_TRANSPOSITION_TABLE_H
#define OTHello_AI_TRANSPOSITION_TABLE_H

#include "../core/bitboard.h"
#include "../evaluators/evaluator.h"
#include <cstdint>
#include <vector>

namespace othello {

// エントリーの種類
enum class EntryType : uint8_t {
    EXACT,    // 正確な値
    LOWER,    // 下界（Alphaカット）
    UPPER     // 上界（Betaカット）
};

// 置換表エントリー
struct TTEntry {
    uint64_t key;      // Zobristハッシュの上位ビット
    Score score;       // 評価値
    int8_t depth;      // 探索深さ
    EntryType type;    // エントリーの種類
    uint8_t moveBits;  // 最善手（ビット位置 0-63、255=パス）

    TTEntry() : key(0), score(0), depth(-1), type(EntryType::EXACT), moveBits(255) {}

    TTEntry(uint64_t k, Score s, int8_t d, EntryType t, uint8_t m)
        : key(k), score(s), depth(d), type(t), moveBits(m) {}
};

// 置換表（Transposition Table）
class TranspositionTable {
public:
    explicit TranspositionTable(size_t sizeMB = 16);

    // エントリーを取得
    bool lookup(uint64_t hash, TTEntry& entry) const;

    // エントリーを保存
    void store(uint64_t hash, Score score, int8_t depth, EntryType type, uint8_t moveBits);

    // テーブルをクリア
    void clear();

    // 使用率を取得（デバッグ用）
    double getUsage() const;

private:
    std::vector<TTEntry> table_;
    size_t mask_;  // サイズ-1（ビットマスク用）

    // ハッシュの衝突チェック用
    static constexpr uint64_t HASH_MASK = 0xFFFFFFFFFFFFFFFULL; // 下位56ビット
};

// Zobrist Hashing
class Zobrist {
public:
    Zobrist();

    // 初期化（乱数でテーブル生成）
    void init();

    // 盤面のハッシュを計算
    uint64_t hash(const Bitboard& board) const;

    // 手を適用した後のハッシュを計算（増分更新）
    uint64_t applyMove(uint64_t hash, int moveBit, uint64_t flips, bool isPlayer) const;

private:
    // 各位置のプレイヤー石、相手石に対する乱数
    uint64_t playerTable_[64];
    uint64_t opponentTable_[64];
};

// グローバルインスタンス
extern Zobrist g_zobrist;

} // namespace othello

#endif // OTHello_AI_TRANSPOSITION_TABLE_H
