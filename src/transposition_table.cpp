#include "transposition_table.h"
#include <random>
#include <cstring>

namespace othello {

// グローバルZobristインスタンス
Zobrist g_zobrist;

// ================ TranspositionTable ================

TranspositionTable::TranspositionTable(size_t sizeMB) {
    // サイズを2の累乗に調整
    size_t entrySize = sizeof(TTEntry);
    size_t numEntries = (sizeMB * 1024 * 1024) / entrySize;

    // 2の累乗に丸める
    size_t powerOfTwo = 1;
    while (powerOfTwo * 2 <= numEntries) {
        powerOfTwo *= 2;
    }

    table_.resize(powerOfTwo);
    mask_ = powerOfTwo - 1;

    clear();
}

bool TranspositionTable::lookup(uint64_t hash, TTEntry& entry) const {
    size_t index = hash & mask_;
    const TTEntry& e = table_[index];

    // ハッシュの一致チェック
    if ((e.key ^ (hash >> 8)) == 0 && e.depth >= 0) {
        entry = e;
        return true;
    }
    return false;
}

void TranspositionTable::store(uint64_t hash, Score score, int8_t depth, EntryType type, uint8_t moveBits) {
    size_t index = hash & mask_;
    TTEntry& e = table_[index];

    // 上位56ビットをキーとして保存
    uint64_t key = hash >> 8;

    // 深い探索または空きエントリーなら上書き
    if (e.depth < 0 || depth >= e.depth || (e.key ^ key) != 0) {
        e.key = key;
        e.score = score;
        e.depth = depth;
        e.type = type;
        e.moveBits = moveBits;
    }
}

void TranspositionTable::clear() {
    std::memset(table_.data(), 0, table_.size() * sizeof(TTEntry));
}

double TranspositionTable::getUsage() const {
    size_t used = 0;
    for (const auto& e : table_) {
        if (e.depth >= 0) used++;
    }
    return static_cast<double>(used) / table_.size();
}

// ================ Zobrist ================

Zobrist::Zobrist() {
    init();
}

void Zobrist::init() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(1, UINT64_MAX);

    for (int i = 0; i < 64; i++) {
        playerTable_[i] = dist(gen);
        opponentTable_[i] = dist(gen);
    }
}

uint64_t Zobrist::hash(const Bitboard& board) const {
    uint64_t h = 0;

    // プレイヤーの石
    uint64_t p = board.player;
    while (p) {
        int lsb = __builtin_ctzll(p);
        h ^= playerTable_[lsb];
        p &= (p - 1);
    }

    // 相手の石
    uint64_t o = board.opponent;
    while (o) {
        int lsb = __builtin_ctzll(o);
        h ^= opponentTable_[lsb];
        o &= (o - 1);
    }

    return h;
}

uint64_t Zobrist::applyMove(uint64_t hash, int moveBit, uint64_t flips, bool isPlayer) const {
    // 新しく置く石
    if (isPlayer) {
        hash ^= playerTable_[moveBit];
    } else {
        hash ^= opponentTable_[moveBit];
    }

    // 反転する石
    while (flips) {
        int lsb = __builtin_ctzll(flips);
        if (isPlayer) {
            // 相手→自分: 相手テーブルをXOR、プレイヤーテーブルをXOR
            hash ^= opponentTable_[lsb];
            hash ^= playerTable_[lsb];
        } else {
            // 自分→相手: プレイヤーテーブルをXOR、相手テーブルをXOR
            hash ^= playerTable_[lsb];
            hash ^= opponentTable_[lsb];
        }
        flips &= (flips - 1);
    }

    return hash;
}

} // namespace othello
