#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace std;

// 8方向のデルタ
const int DX[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int DY[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

// 座標文字列をインデックスに変換
bool coordToIndex(const string& coord, int& x, int& y) {
    if (coord.length() < 2) return false;
    x = coord[0] - 'a';
    y = coord[1] - '1';
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// 指定位置に石を置いたときに返る石数を計算
int countFlips(const vector<string>& board, int x, int y, int playerId) {
    if (board[y][x] != '.') return 0;

    int myChar = (playerId == 0) ? '0' : '1';
    int oppChar = (playerId == 0) ? '1' : '0';
    int totalFlips = 0;

    for (int d = 0; d < 8; d++) {
        int dx = DX[d], dy = DY[d];
        int cx = x + dx, cy = y + dy;
        int flips = 0;

        while (cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && board[cy][cx] == oppChar) {
            flips++;
            cx += dx;
            cy += dy;
        }

        if (flips > 0 && cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && board[cy][cx] == myChar) {
            totalFlips += flips;
        }
    }

    return totalFlips;
}

// 座標変換テスト
TEST(TestIO2, CoordinateConversion) {
    int x, y;

    EXPECT_TRUE(coordToIndex("a1", x, y));
    EXPECT_EQ(x, 0);
    EXPECT_EQ(y, 0);

    EXPECT_TRUE(coordToIndex("h8", x, y));
    EXPECT_EQ(x, 7);
    EXPECT_EQ(y, 7);

    EXPECT_TRUE(coordToIndex("e4", x, y));
    EXPECT_EQ(x, 4);
    EXPECT_EQ(y, 3);

    EXPECT_FALSE(coordToIndex("", x, y));
    EXPECT_FALSE(coordToIndex("a", x, y));
}

// 初期配置でのフリップ数テスト
TEST(TestIO2, InitialPositionFlips) {
    // 初期配置: 黒(0)がe4,d5、白(1)がd4,e5
    vector<string> board = {
        "........",
        "........",
        "........",
        "...10...",  // d4=1, e4=0
        "...01...",  // d5=0, e5=1
        "........",
        "........",
        "........"
    };

    // 黒(playerId=0)の視点
    EXPECT_EQ(countFlips(board, 2, 3, 0), 1); // c4: d4を反転
    EXPECT_EQ(countFlips(board, 3, 2, 0), 1); // d3: d4を反転
    EXPECT_EQ(countFlips(board, 4, 5, 0), 1); // e6: e5を反転
    EXPECT_EQ(countFlips(board, 5, 4, 0), 1); // f5: e5を反転
}

// 複数石反転テスト
TEST(TestIO2, MultipleFlips) {
    vector<string> board = {
        "........",
        "........",
        "........",
        "........",
        "...0....",  // d5=黒(自分の石)
        "...1....",  // d6=白(相手の石)
        "...1....",  // d7=白(相手の石)
        "........"   // d8=空き
    };

    // d8に置くと、d7,d6の2石を反転
    EXPECT_EQ(countFlips(board, 3, 7, 0), 2);

    // 3石反転のテスト
    board = {
        "........",
        "........",
        "........",
        "...0....",  // d4=黒(自分の石)
        "...1....",  // d5=白(相手の石)
        "...1....",  // d6=白(相手の石)
        "...1....",  // d7=白(相手の石)
        "........"   // d8=空き
    };

    // d8に置くと、d7,d6,d5の3石を反転
    EXPECT_EQ(countFlips(board, 3, 7, 0), 3);
}

// 空きマス以外のテスト
TEST(TestIO2, NonEmptyCell) {
    vector<string> board = {
        "........",
        "........",
        "........",
        "...10...",
        "...01...",
        "........",
        "........",
        "........"
    };

    // 既に石がある場所には置けない
    EXPECT_EQ(countFlips(board, 3, 3, 0), 0); // d4 (白)
    EXPECT_EQ(countFlips(board, 4, 3, 0), 0); // e4 (黒)
}

// 合法手でない手のテスト
TEST(TestIO2, IllegalMove) {
    vector<string> board = {
        "........",
        "........",
        "........",
        "...10...",
        "...01...",
        "........",
        "........",
        "........"
    };

    // a1は反転できない
    EXPECT_EQ(countFlips(board, 0, 0, 0), 0);
}

// コーナー周辺のテスト
TEST(TestIO2, CornerAdjacent) {
    vector<string> board = {
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........"
    };

    // 盤面が空なのでどこにも置けない
    EXPECT_EQ(countFlips(board, 0, 0, 0), 0);
    EXPECT_EQ(countFlips(board, 7, 7, 0), 0);
}
