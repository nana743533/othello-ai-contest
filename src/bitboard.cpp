#include "bitboard.h"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace othello {

Bitboard Bitboard::initial() {
    // 初期配置: d4(白), e5(白), e4(黒), d5(黒)
    // 座標系: a=0, b=1, ..., h=7; 1=0, 2=1, ..., 8=7
    // d4 = col=3, row=3 -> bit 3*8+3 = 27
    // e5 = col=4, row=4 -> bit 4*8+4 = 36
    // e4 = col=4, row=3 -> bit 3*8+4 = 28
    // d5 = col=3, row=4 -> bit 4*8+3 = 35

    // 黒（先手）: e4, d5
    uint64_t black = (1ULL << 28) | (1ULL << 35);
    // 白（後手）: d4, e5
    uint64_t white = (1ULL << 27) | (1ULL << 36);

    return Bitboard(black, white);
}

uint64_t Bitboard::getMask(int x, int y) {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return 0;
    return 1ULL << (y * 8 + x);
}

uint64_t Bitboard::getMask(const std::string& coord) {
    if (coord.length() < 2) return 0;
    char col = coord[0];
    int row = coord[1] - '1';
    return getMask(col - 'a', row);
}

bool Bitboard::hasPlayerAt(int x, int y) const {
    uint64_t mask = getMask(x, y);
    return (player & mask) != 0;
}

bool Bitboard::hasPlayerAt(const std::string& coord) const {
    uint64_t mask = getMask(coord);
    return (player & mask) != 0;
}

bool Bitboard::hasOpponentAt(int x, int y) const {
    uint64_t mask = getMask(x, y);
    return (opponent & mask) != 0;
}

bool Bitboard::hasOpponentAt(const std::string& coord) const {
    uint64_t mask = getMask(coord);
    return (opponent & mask) != 0;
}

bool Bitboard::isEmptyAt(int x, int y) const {
    uint64_t mask = getMask(x, y);
    return ((player | opponent) & mask) == 0;
}

bool Bitboard::isEmptyAt(const std::string& coord) const {
    uint64_t mask = getMask(coord);
    return ((player | opponent) & mask) == 0;
}

void Bitboard::placePlayer(int x, int y) {
    uint64_t mask = getMask(x, y);
    player |= mask;
    opponent &= ~mask;
}

void Bitboard::placePlayer(const std::string& coord) {
    uint64_t mask = getMask(coord);
    player |= mask;
    opponent &= ~mask;
}

void Bitboard::placeOpponent(int x, int y) {
    uint64_t mask = getMask(x, y);
    opponent |= mask;
    player &= ~mask;
}

void Bitboard::placeOpponent(const std::string& coord) {
    uint64_t mask = getMask(coord);
    opponent |= mask;
    player &= ~mask;
}

void Bitboard::removeAt(int x, int y) {
    uint64_t mask = getMask(x, y);
    player &= ~mask;
    opponent &= ~mask;
}

void Bitboard::removeAt(const std::string& coord) {
    uint64_t mask = getMask(coord);
    player &= ~mask;
    opponent &= ~mask;
}

uint64_t Bitboard::getFlipsInDirection(int x, int y, int dx, int dy) const {
    uint64_t flips = 0;
    int cx = x + dx;
    int cy = y + dy;

    // 相手の石が続く限り進む
    while (cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && hasOpponentAt(cx, cy)) {
        flips |= getMask(cx, cy);
        cx += dx;
        cy += dy;
    }

    // 最後が自分の石であれば反転対象
    if (cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && hasPlayerAt(cx, cy)) {
        return flips;
    }
    return 0;
}

uint64_t Bitboard::getFlips(int x, int y) const {
    if (!isEmptyAt(x, y)) return 0;

    uint64_t totalFlips = 0;
    for (int i = 0; i < 8; i++) {
        totalFlips |= getFlipsInDirection(x, y, DIRECTION_DELTAS[i][0], DIRECTION_DELTAS[i][1]);
    }
    return totalFlips;
}

uint64_t Bitboard::getLegalMoves() const {
    uint64_t legalMoves = 0;
    uint64_t empty = ~(player | opponent);

    // 全ての空きマスをチェック
    while (empty) {
        int lsb = __builtin_ctzll(empty);
        int x = lsb % 8;
        int y = lsb / 8;

        if (getFlips(x, y) != 0) {
            legalMoves |= (1ULL << lsb);
        }

        empty &= (empty - 1);
    }

    return legalMoves;
}

bool Bitboard::isLegalMove(int x, int y) const {
    if (!isEmptyAt(x, y)) return false;
    return getFlips(x, y) != 0;
}

bool Bitboard::isLegalMove(const std::string& coord) const {
    if (coord.length() < 2) return false;
    int x = coord[0] - 'a';
    int y = coord[1] - '1';
    return isLegalMove(x, y);
}

Bitboard Bitboard::placeAndFlip(int x, int y) const {
    Bitboard result = *this;
    uint64_t flips = getFlips(x, y);

    if (flips == 0) return result; // 非法手

    // 自分の石を置く
    result.placePlayer(x, y);

    // 相手の石を反転
    result.player |= flips;
    result.opponent &= ~flips;

    return result;
}

Bitboard Bitboard::placeAndFlip(const std::string& coord) const {
    if (coord.length() < 2) return *this;
    int x = coord[0] - 'a';
    int y = coord[1] - '1';
    return placeAndFlip(x, y);
}

int Bitboard::getEmptyCount() const {
    return __builtin_popcountll(~(player | opponent) & 0xFFFFFFFFFFFFFFFFULL);
}

int Bitboard::getPlayerCount() const {
    return __builtin_popcountll(player);
}

int Bitboard::getOpponentCount() const {
    return __builtin_popcountll(opponent);
}

std::string Bitboard::toString() const {
    std::stringstream ss;
    ss << "  a b c d e f g h\n";
    for (int y = 0; y < 8; y++) {
        ss << (y + 1) << " ";
        for (int x = 0; x < 8; x++) {
            uint64_t mask = 1ULL << (y * 8 + x);
            if (player & mask) {
                ss << "● ";
            } else if (opponent & mask) {
                ss << "○ ";
            } else {
                ss << ". ";
            }
        }
        ss << (y + 1) << "\n";
    }
    ss << "  a b c d e f g h\n";
    return ss.str();
}

Bitboard Bitboard::fromString(const std::string& boardStr, bool isBlack) {
    Bitboard board;
    int row = 0;

    std::istringstream iss(boardStr);
    std::string line;

    while (std::getline(iss, line) && row < 8) {
        // 空行をスキップ
        if (line.empty()) continue;

        for (int col = 0; col < static_cast<int>(line.length()) && col < 8; col++) {
            char c = line[col];
            if (c == '0') { // 黒
                if (isBlack) {
                    board.placePlayer(col, row);
                } else {
                    board.placeOpponent(col, row);
                }
            } else if (c == '1') { // 白
                if (isBlack) {
                    board.placeOpponent(col, row);
                } else {
                    board.placePlayer(col, row);
                }
            }
        }
        row++;
    }

    return board;
}

} // namespace othello
