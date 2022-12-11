#include "game_board_eval.hpp"

std::unordered_map<std::array<int, BOARD_SIZE * BOARD_SIZE>,
                   int,
                   hash_board_array>
    game_board_eval::board_cache_{};
std::unordered_map<std::array<int, BOARD_SIZE>, int, hash_line_array>
    game_board_eval::line_cache_{};

int game_board_eval::evaluate(game_board const &board) const {
  if (board.is_terminated()) {
    return 0;
  }

  try {
    return board_cache_.at(board.get_board_array());
  } catch (std::out_of_range &) {
    int evaluate_value = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
      std::array<int, BOARD_SIZE> a1 = board.fetch_line(DIRECTION_D, i);
      std::array<int, BOARD_SIZE> a2 = board.fetch_line(DIRECTION_L, i);
      evaluate_value += evaluate_line_(a1);
      evaluate_value += evaluate_line_(a2);
    }

    board_cache_.insert(
        std::pair<std::array<int, BOARD_SIZE * BOARD_SIZE>, int>{
            board.get_board_array(), evaluate_value});
    return evaluate_value;
  }
}

int game_board_eval::evaluate_line_(std::array<int, BOARD_SIZE> &line) const {
  try {
    return line_cache_.at(line);
  } catch (std::out_of_range &) {
    int monotonic    = 0;
    int largest_tile = 0;
    int trend_p      = 0;
    int trend_n      = 0;
    for (int i = 0; i < BOARD_SIZE - 1; i++) {
      if (line[i] > largest_tile) {
        largest_tile = line[i];
      }
      if (line[i] <= line[i + 1]) {
        trend_p++;
      } else if (line[i] >= line[i + 1]) {
        trend_n++;
      }
    }
    if (trend_p == BOARD_SIZE - 1 || trend_n == BOARD_SIZE - 1) {
      monotonic = largest_tile;
    }

    int zeros = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
      if (line[i] == 0) {
        zeros++;
      }
    }

    return monotonic * 30 + zeros * 50;
  }
};
