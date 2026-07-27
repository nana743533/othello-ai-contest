#!/usr/bin/env python3
"""
位置重み評価 + n手先読み（Minimax）

評価値 = 自分の石の重み合計 - 相手の石の重み合計（02_greedy.py と同じ W）
相手は自分目線の評価値を最大化する手を選ぶ（= 自分の評価値を最小化）。
"""

DEPTH = 4  # 読みの深さ（半手 = ply）

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
    return W[y][x]


def evaluate(board, me):
    """自分目線: 自分の重み合計 - 相手の重み合計"""
    my_score = opp_score = 0
    for y, row in enumerate(board):
        for x, cell in enumerate(row):
            if cell == str(me):
                my_score += cell_weight(x, y)
            elif cell != ".":
                opp_score += cell_weight(x, y)
    return my_score - opp_score


def get_flips(board, x, y, player):
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
    if board[y][x] != ".":
        return False
    return bool(get_flips(board, x, y, player))


def to_coord(x, y):
    return chr(ord("a") + x) + str(y + 1)


def from_coord(coord):
    return ord(coord[0]) - ord("a"), int(coord[1]) - 1


def get_legal_moves(board, player):
    moves = []
    for y in range(8):
        for x in range(8):
            if is_legal(board, x, y, player):
                moves.append(to_coord(x, y))
    moves.sort(key=lambda c: W[from_coord(c)[1]][from_coord(c)[0]], reverse=True)
    return moves


def apply_move(board, coord, player):
    x, y = from_coord(coord)
    new_board = [list(row) for row in board]
    new_board[y][x] = str(player)
    for fx, fy in get_flips(board, x, y, player):
        new_board[fy][fx] = str(player)
    return ["".join(row) for row in new_board]


def minimax(board, depth, current_player, me, alpha, beta):
    if depth == 0:
        return evaluate(board, me), None

    moves = get_legal_moves(board, current_player)
    if not moves:
        if not get_legal_moves(board, 1 - current_player):
            return evaluate(board, me), None
        score, _ = minimax(board, depth - 1, 1 - current_player, me, alpha, beta)
        return score, None

    best_move = moves[0]
    if current_player == me:
        best_score = float("-inf")
        for move in moves:
            next_board = apply_move(board, move, current_player)
            score, _ = minimax(next_board, depth - 1, 1 - current_player, me, alpha, beta)
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
        score, _ = minimax(next_board, depth - 1, 1 - current_player, me, alpha, beta)
        if score < best_score:
            best_score = score
            best_move = move
        beta = min(beta, best_score)
        if beta <= alpha:
            break
    return best_score, best_move


def choose_move(board, legal_moves, me):
    """自分の手番: 合法手ごとに n 手先まで読み、評価値が最大の手を選ぶ"""
    best_move = legal_moves[0]
    best_score = float("-inf")
    alpha, beta = float("-inf"), float("inf")
    ordered = sorted(
        legal_moves,
        key=lambda c: W[from_coord(c)[1]][from_coord(c)[0]],
        reverse=True,
    )
    for move in ordered:
        next_board = apply_move(board, move, me)
        score, _ = minimax(next_board, DEPTH - 1, 1 - me, me, alpha, beta)
        if score > best_score:
            best_score = score
            best_move = move
        alpha = max(alpha, best_score)
    return best_move, int(best_score)


def main():
    me = int(input())
    board_size = int(input())

    while True:
        board = [input() for _ in range(board_size)]
        action_count = int(input())
        legal_moves = [input().strip() for _ in range(action_count)]

        if not legal_moves:
            print("pass")
            continue

        move, score = choose_move(board, legal_moves, me)
        print(f"{move} MSG {score}")


if __name__ == "__main__":
    main()
