#include "../include/game_board_eval.hpp"

int GameBoardEval::evaluate(GameBoard const &board) const {
  if (board.is_terminated()) {
    return 0;
  }

  int evaluate_value = 0;

  for (int i = 0; i < BOARD_SIZE; i++) {
    std::array<int, BOARD_SIZE> a1 = board.fetch_line(DIRECTION_D, i);
    std::array<int, BOARD_SIZE> a2 = board.fetch_line(DIRECTION_L, i);
    evaluate_value += evaluate_line_(a1);
    evaluate_value += evaluate_line_(a2);
  }

  return evaluate_value;
}

int GameBoardEval::evaluate_line_(std::array<int, BOARD_SIZE> &line) const {
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
};
