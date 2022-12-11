#include "game_board_eval.hpp"

std::unordered_map<std::array<int, BOARD_SIZE * BOARD_SIZE>,
                   int,
                   hash_board_array>
    game_board_eval::board_cache_{};
std::unordered_map<std::array<int, BOARD_SIZE>, int, hash_line_array>
    game_board_eval::line_cache_{};

int game_board_eval::evaluate(game_board const &board) const {
  int evaluate_score = 0;

  try {
    evaluate_score += board_cache_.at(board.get_board_array());
  } catch (std::out_of_range &) {
    int evaluate_array = 0;

    auto eval_array = [](std::array<int, BOARD_SIZE> arr) {
      int eval = 0;
      for (int i = 0; i < BOARD_SIZE - 1; i++) {
        if (true || (arr[i] != 0 && arr[i + 1] != 0)) {
          eval -= std::abs(arr[i] - arr[i + 1]);
        }
      }

      for (int i = 0; i < BOARD_SIZE; i++) {
        if (arr[i] == 0) {
          eval += 50;
        }
      }

      int trend_p = 0;
      int trend_n = 0;
      for (int i = 0; i < BOARD_SIZE - 1; i++) {
        if (arr[i] <= arr[i + 1]) {
          trend_p++;
        } else if (arr[i] >= arr[i + 1]) {
          trend_n++;
        }
      }
      if (trend_p == BOARD_SIZE - 1 || trend_n == BOARD_SIZE - 1) {
        eval += 30000;
      }

      return eval;
    };
    for (int i = 0; i < BOARD_SIZE; i++) {
      std::array<int, BOARD_SIZE> a1 = board.fetch_line(DIRECTION_D, i);
      std::array<int, BOARD_SIZE> a2 = board.fetch_line(DIRECTION_L, i);
      evaluate_array += eval_array(a1);
      evaluate_array += eval_array(a2);
    }

    board_cache_.insert(
        std::pair<std::array<int, BOARD_SIZE * BOARD_SIZE>, int>{
            board.get_board_array(), evaluate_array});
    evaluate_score += evaluate_array;
  }

  if (board.is_terminated()) {
    evaluate_score -= 65536;
  }
  evaluate_score += board.get_score();
  evaluate_score += board.get_largest_tile();

  return evaluate_score;
}
