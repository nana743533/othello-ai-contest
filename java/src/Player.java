import java.util.*;

/**
 * CodinGame Othello AI - ランダムプレイヤー
 *
 * 入出力仕様（docs/01_io_spec.md）に従い、合法手の中からランダムに一手を選びます。
 */
class Player {
    private static int playerId;
    private static int boardSize;

    public static void main(String[] args) {
        try (Scanner scanner = new Scanner(System.in)) {
            // 初回入力
            playerId = Integer.parseInt(scanner.nextLine());
            boardSize = Integer.parseInt(scanner.nextLine());

            // ゲームループ
            while (scanner.hasNextLine()) {
                // 盤面の状態（boardSize行）を読み飛ばす
                for (int i = 0; i < boardSize; i++) {
                    if (!scanner.hasNextLine()) return;
                    scanner.nextLine();
                }

                // Expert Modeの追加入力（今回は不使用なので読み飛ばす）
                // ※ 実際のCodinGameでは独自判定が必要な場合あり

                // 合法手の数
                if (!scanner.hasNextLine()) return;
                int actionCount = Integer.parseInt(scanner.nextLine());

                // 合法手のリスト
                List<String> legalMoves = new ArrayList<>();
                for (int i = 0; i < actionCount; i++) {
                    if (!scanner.hasNextLine()) return;
                    legalMoves.add(scanner.nextLine().trim());
                }

                // ランダムに一手を選択して出力
                String move = chooseRandomMove(legalMoves);
                System.out.println(move);
            }
        }
    }

    private static String chooseRandomMove(List<String> legalMoves) {
        if (legalMoves.isEmpty()) {
            return "pass";
        }
        int index = new Random().nextInt(legalMoves.size());
        return legalMoves.get(index);
    }
}
