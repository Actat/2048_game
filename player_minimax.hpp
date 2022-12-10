#ifndef PLAYER_MINIMAX_HPP
#define PLAYER_MINIMAX_HPP

#include <array>
#include <chrono>
#include <iostream>
#include <limits>
#include <unordered_map>
#include "game_board.hpp"
#include "player.hpp"

class time_keeper {
private:
  std::chrono::high_resolution_clock::time_point start_time_;
  int time_limit_ms_;

public:
  time_keeper(int time_limit_ms) {
    start_time_    = std::chrono::high_resolution_clock::now();
    time_limit_ms_ = time_limit_ms;
  }

  bool is_time_over() {
    auto duration = std::chrono::high_resolution_clock::now() - start_time_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
               .count() >= time_limit_ms_;
  }
};

class hash_board_array {
public:
  size_t operator()(std::array<int, BOARD_SIZE * BOARD_SIZE> const arr) const {
    const int C = 97;
    size_t t    = 0;
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
      t = t * C + arr.at(i);
    }
    return t;
  }
};

class player_minimax : public player {
private:
  mutable std::unordered_map<std::array<int, BOARD_SIZE * BOARD_SIZE>,
                             int,
                             hash_board_array>
      evaluation_cache;

  int evaluate_(game_board board) const;
  int iterative_deeping_(game_board board, int time_limit_ms) const;
  int iterative_deeping_alphabeta_(game_board board,
                                   int depth,
                                   time_keeper *tk) const;
  int iterative_deeping_alphabeta_move_(game_board board,
                                        int depth,
                                        int alpha,
                                        int beta,
                                        time_keeper *tk) const;
  int iterative_deeping_alphabeta_add_(game_board board,
                                       int depth,
                                       int alpha,
                                       int beta,
                                       time_keeper *tk) const;

public:
  int play(game_board const &board) const override {
    return iterative_deeping_(board, 500);
  };
};

int player_minimax::evaluate_(game_board board) const {
  int evaluate_score = 0;

  try {
    evaluate_score += evaluation_cache.at(board.get_board_array());
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
      std::array<int, BOARD_SIZE> a1 = board.fetch_array(DIRECTION_D, i);
      std::array<int, BOARD_SIZE> a2 = board.fetch_array(DIRECTION_L, i);
      evaluate_array += eval_array(a1);
      evaluate_array += eval_array(a2);
    }

    evaluation_cache.insert(
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

int player_minimax::iterative_deeping_(game_board board,
                                       int time_limit_ms) const {
  auto tk            = time_keeper(time_limit_ms);
  int best_direction = -1;
  for (int depth = 1;; depth++) {
    int dir = iterative_deeping_alphabeta_(board, depth, &tk);

    if (tk.is_time_over()) {
      std::cout << "Time is up. depth: " << depth - 1  //
                << ", direction: " << best_direction << std::endl;
      break;
    }

    best_direction = dir;
    std::cout << "depth: " << depth
              << " completed. best_direction: " << best_direction << std::endl;
  }

  return best_direction;
}

int player_minimax::iterative_deeping_alphabeta_(game_board board,
                                                 int depth,
                                                 time_keeper *tk) const {
  int best_direction      = -1;
  int best_evaluate_score = std::numeric_limits<int>::min();
  for (int direction = 0; direction < 4; direction++) {
    if (board.can_move(direction)) {
      game_board b = board;
      b.move(direction);
      int evaluate_score = iterative_deeping_alphabeta_add_(
          b, depth, std::numeric_limits<int>::min(),
          std::numeric_limits<int>::max(), tk);

      if (tk->is_time_over()) {
        return best_direction;
      }

      std::cout << "depth: " << depth << " dir: " << direction
                << " score: " << evaluate_score << std::endl;

      if (evaluate_score > best_evaluate_score) {
        best_direction      = direction;
        best_evaluate_score = evaluate_score;
      }
    }
  }
  return best_direction;
}

int player_minimax::iterative_deeping_alphabeta_move_(game_board board,
                                                      int depth,
                                                      int alpha,
                                                      int beta,
                                                      time_keeper *tk) const {
  if (tk->is_time_over()) {
    return alpha;
  }

  int a;

  if (depth == 0 || board.is_terminated()) {
    return evaluate_(board);
  }

  for (int direction = 0; direction < 4; direction++) {
    if (board.can_move(direction)) {
      game_board b = board;
      b.move(direction);
      int evaluate_score =
          iterative_deeping_alphabeta_add_(b, depth, alpha, beta, tk);

      if (tk->is_time_over()) {
        return alpha;
      }

      a = std::max(alpha, evaluate_score);
      if (a >= beta) {
        return a;
      }
    }
  }
  return a;
}

int player_minimax::iterative_deeping_alphabeta_add_(game_board board,
                                                     int depth,
                                                     int alpha,
                                                     int beta,
                                                     time_keeper *tk) const {
  if (tk->is_time_over()) {
    return beta;
  }

  int b;

  for (int position = 0; position < BOARD_SIZE * BOARD_SIZE; position++) {
    if (board.is_blank(position)) {
      game_board b1 = board;
      b1.add_tile(position, 1);
      int evaluate_score1 =
          iterative_deeping_alphabeta_move_(b1, depth - 1, alpha, beta, tk);

      if (tk->is_time_over()) {
        return beta;
      }

      b = std::min(beta, evaluate_score1);
      if (alpha >= b) {
        return b;
      }

      game_board b2 = board;
      b2.add_tile(position, 2);
      int evaluate_score2 =
          iterative_deeping_alphabeta_move_(b2, depth - 1, alpha, beta, tk);

      if (tk->is_time_over()) {
        return beta;
      }

      b = std::min(beta, evaluate_score2);
      if (alpha >= b) {
        return b;
      }
    }
  }
  return b;
}

#endif  // PLAYER_MINIMAX_HPP
