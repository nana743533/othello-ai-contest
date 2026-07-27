#!/usr/bin/env python3
"""
04_path_score + 終盤完全読み

空きマスが ENDGAME_EMPTY 以下なら、評価関数を使わず最後まで読み切る。
終盤の評価値 = 最終的な自分の石数 - 相手の石数。
それ以外は 04 と同じ打点合計の先読み。
"""

DEPTH = 4  # 中盤の読み深さ（半手 = ply）
ENDGAME_EMPTY = 12  # この空きマス数以下で終盤完全読み

W = [
    [100, -20,  10,   5,   5,  10, -20, 100],
    [-20, -50,  -2,  -2,  -2,  -2, -50, -20],
    [ 10,  -2,   1,   1,   1,   1,  -2,  10],
    [  5,  -2,   1,   0,   0,   1,  -2,   5],
    [  5,  -2,   1,   0,   0,   1,  -2,   5],
    [ 10,  -2,   1,   1,   1,   1,  -2,  10],
    [-20, -50,  -2,  -2,  -2,  -2, -50, -20],
    [100, -20,  10,   5,   5,  10, -20, 100],
]

DIRS = [(-1, -1), (0, -1), (1, -1), (-1, 0), (1, 0), (-1, 1), (0, 1), (1, 1)]


def cell_weight(x, y):
    """座標 (x, y) の位置重みを返す"""
    return W[y][x]


def move_delta(coord, current_player, me):
    """一手の寄与。自分の打点は +W、相手の打点は -W"""
    x, y = from_coord(coord)
    w = cell_weight(x, y)
    return w if current_player == me else -w


def count_empty(board):
    """空きマス数を数える"""
    return sum(row.count(".") for row in board)


def final_score(board, me):
    """終局時の石数差（自分 - 相手）"""
    my = opp = 0
    me_s, opp_s = str(me), str(1 - me)
    for row in board:
        my += row.count(me_s)
        opp += row.count(opp_s)
    return my - opp


def get_flips(board, x, y, player):
    """(x, y) に player が打ったとき反転するマス一覧"""
    opponent = str(1 - player)
    me = str(player)
    flips = []
    for dx, dy in DIRS:
        cx, cy = x + dx, y + dy
        line = []
        while 0 <= cx < 8 and 0 <= cy < 8 and board[cy][cx] == opponent:
            line.append((cx, cy))
            cx += dx
            cy += dy
        if line and 0 <= cx < 8 and 0 <= cy < 8 and board[cy][cx] == me:
            flips.extend(line)
    return flips


def is_legal(board, x, y, player):
    """(x, y) が player の合法手かどうか"""
    if board[y][x] != ".":
        return False
    return bool(get_flips(board, x, y, player))


def to_coord(x, y):
    """盤面座標を CodinGame 形式（例: d3）に変換"""
    return chr(ord("a") + x) + str(y + 1)


def from_coord(coord):
    """CodinGame 形式（例: d3）を盤面座標 (x, y) に変換"""
    return ord(coord[0]) - ord("a"), int(coord[1]) - 1


def get_legal_moves(board, player):
    """player の合法手を重み降順で返す"""
    moves = []
    for y in range(8):
        for x in range(8):
            if is_legal(board, x, y, player):
                moves.append(to_coord(x, y))
    moves.sort(key=lambda c: W[from_coord(c)[1]][from_coord(c)[0]], reverse=True)
    return moves


def apply_move(board, coord, player):
    """coord に player の石を置き、反転後の盤面を返す"""
    x, y = from_coord(coord)
    new_board = [list(row) for row in board]
    new_board[y][x] = str(player)
    for fx, fy in get_flips(board, x, y, player):
        new_board[fy][fx] = str(player)
    return ["".join(row) for row in new_board]


def search_path(board, depth, current_player, me, path_score, alpha, beta):
    """中盤: 読み筋の打点合計を返す"""
    if depth == 0:
        return path_score, None

    moves = get_legal_moves(board, current_player)
    if not moves:
        if not get_legal_moves(board, 1 - current_player):
            return path_score, None
        score, _ = search_path(
            board, depth - 1, 1 - current_player, me, path_score, alpha, beta
        )
        return score, None

    best_move = moves[0]
    if current_player == me:
        best_score = float("-inf")
        for move in moves:
            next_score = path_score + move_delta(move, current_player, me)
            next_board = apply_move(board, move, current_player)
            score, _ = search_path(
                next_board, depth - 1, 1 - current_player, me, next_score, alpha, beta
            )
            if score > best_score:
                best_score = score
                best_move = move
            alpha = max(alpha, best_score)
            if beta <= alpha:
                break
        return best_score, best_move

    best_score = float("inf")
    for move in moves:
        next_score = path_score + move_delta(move, current_player, me)
        next_board = apply_move(board, move, current_player)
        score, _ = search_path(
            next_board, depth - 1, 1 - current_player, me, next_score, alpha, beta
        )
        if score < best_score:
            best_score = score
            best_move = move
        beta = min(beta, best_score)
        if beta <= alpha:
            break
    return best_score, best_move


def search_endgame(board, current_player, me, alpha, beta):
    """終盤完全読み: 最終石数差を最大化 / 最小化"""
    moves = get_legal_moves(board, current_player)
    if not moves:
        # 双方パスなら終局、そうでなければパスして手番交代
        if not get_legal_moves(board, 1 - current_player):
            return final_score(board, me), None
        return search_endgame(board, 1 - current_player, me, alpha, beta)

    best_move = moves[0]
    if current_player == me:
        best_score = float("-inf")
        for move in moves:
            next_board = apply_move(board, move, current_player)
            score, _ = search_endgame(next_board, 1 - current_player, me, alpha, beta)
            if score > best_score:
                best_score = score
                best_move = move
            alpha = max(alpha, best_score)
            if beta <= alpha:
                break
        return best_score, best_move

    best_score = float("inf")
    for move in moves:
        next_board = apply_move(board, move, current_player)
        score, _ = search_endgame(next_board, 1 - current_player, me, alpha, beta)
        if score < best_score:
            best_score = score
            best_move = move
        beta = min(beta, best_score)
        if beta <= alpha:
            break
    return best_score, best_move


def choose_path_move(board, legal_moves, me):
    """中盤: DEPTH 手分の打点合計が最大になる初手を選ぶ"""
    best_move = legal_moves[0]
    best_score = float("-inf")
    alpha, beta = float("-inf"), float("inf")
    ordered = sorted(
        legal_moves,
        key=lambda c: W[from_coord(c)[1]][from_coord(c)[0]],
        reverse=True,
    )
    for move in ordered:
        path_score = move_delta(move, me, me)
        next_board = apply_move(board, move, me)
        score, _ = search_path(
            next_board, DEPTH - 1, 1 - me, me, path_score, alpha, beta
        )
        if score > best_score:
            best_score = score
            best_move = move
        alpha = max(alpha, best_score)
    return best_move, int(best_score)


def choose_endgame_move(board, legal_moves, me):
    """終盤: 最終石数差が最大になる初手を選ぶ"""
    best_move = legal_moves[0]
    best_score = float("-inf")
    alpha, beta = float("-inf"), float("inf")
    for move in legal_moves:
        next_board = apply_move(board, move, me)
        score, _ = search_endgame(next_board, 1 - me, me, alpha, beta)
        if score > best_score:
            best_score = score
            best_move = move
        alpha = max(alpha, best_score)
    return best_move, int(best_score)


def choose_move(board, legal_moves, me):
    """空きマス数に応じて中盤探索か終盤完全読みかを選ぶ。MSG 文字列も返す"""
    if count_empty(board) <= ENDGAME_EMPTY:
        move, score = choose_endgame_move(board, legal_moves, me)
        # 終盤読み切り開始以降は EG 付き（例: EG+4, EG-12）
        return move, f"EG{score:+d}"
    move, score = choose_path_move(board, legal_moves, me)
    return move, str(score)


def main():
    """CodinGame 入出力ループ。最善手と評価値を出力"""
    me = int(input())
    board_size = int(input())

    while True:
        board = [input() for _ in range(board_size)]
        action_count = int(input())
        legal_moves = [input().strip() for _ in range(action_count)]

        if not legal_moves:
            print("pass")
            continue

        move, msg = choose_move(board, legal_moves, me)
        print(f"{move} MSG {msg}")


if __name__ == "__main__":
    main()
