#ifndef PLAYER_MINIMAX_HPP
#define PLAYER_MINIMAX_HPP

#include <chrono>
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

class hash_board {
public:
  size_t operator()(game_board b) const {
    const int C                                   = 97;
    size_t t                                      = 0;
    std::array<int, BOARD_SIZE *BOARD_SIZE> array = b.get_board();
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
      t = t * C + array.at(i);
    }
    return t;
  }
};

class player_minimax : public player {
private:
  std::unordered_map<game_board, int, hash_board> evaluation_cache;

  int evaluate_(game_board board);
  int alphabeta_(game_board board, int depth);
  int alphabeta_recursion_move_(game_board board,
                                int depth,
                                int alpha,
                                int beta);
  int alphabeta_recursion_add_(game_board board,
                               int depth,
                               int alpha,
                               int beta);
  int minimax_(game_board board, int depth);
  int minimax_recursion_move_(game_board board, int depth);
  int minimax_recursion_add_(game_board board, int depth);

public:
  // virtual int play(game_board board) { return minimax_(board, 2); };
  virtual int play(game_board board) { return alphabeta_(board, 2); };
};

int player_minimax::evaluate_(game_board board) {
  int evaluate_score = 0;

  if (board.is_terminated()) {
    evaluate_score -= 65536;
  }
  evaluate_score += board.get_score();
  evaluate_score += board.get_largest_tile();

  return evaluate_score;
}

int player_minimax::alphabeta_(game_board board, int depth) {
  int best_direction      = -1;
  int best_evaluate_score = std::numeric_limits<int>::min();
  for (int direction = 0; direction < 4; direction++) {
    if (board.can_move(direction)) {
      game_board b = board;
      b.move(direction);
      int evaluate_score =
          alphabeta_recursion_add_(b, depth, std::numeric_limits<int>::min(),
                                   std::numeric_limits<int>::max());
      if (evaluate_score > best_evaluate_score) {
        best_direction = direction;
      }
    }
  }
  return best_direction;
}

int player_minimax::alphabeta_recursion_move_(game_board board,
                                              int depth,
                                              int alpha,
                                              int beta) {
  if (depth == 0 || board.is_terminated()) {
    return evaluate_(board);
  }

  for (int direction = 0; direction < 4; direction++) {
    if (board.can_move(direction)) {
      game_board b = board;
      b.move(direction);
      int evaluate_score = alphabeta_recursion_add_(b, depth, alpha, beta);
      alpha              = std::max(alpha, evaluate_score);
      if (alpha >= beta) {
        return alpha;
      }
    }
  }
  return alpha;
}

int player_minimax::alphabeta_recursion_add_(game_board board,
                                             int depth,
                                             int alpha,
                                             int beta) {
  for (int position = 0; position < BOARD_SIZE * BOARD_SIZE; position++) {
    if (board.is_blank(position)) {
      game_board b1 = board;
      b1.add_tile(position, 1);
      int evaluate_score1 =
          alphabeta_recursion_move_(b1, depth - 1, alpha, beta);
      beta = std::min(beta, evaluate_score1);
      if (alpha >= beta) {
        return beta;
      }

      game_board b2 = board;
      b2.add_tile(position, 2);
      int evaluate_score2 =
          alphabeta_recursion_move_(b2, depth - 1, alpha, beta);
      beta = std::min(beta, evaluate_score2);
      if (alpha >= beta) {
        return beta;
      }
    }
  }
  return beta;
}

int player_minimax::minimax_(game_board board, int depth) {
  int best_direction      = -1;
  int best_evaluate_score = std::numeric_limits<int>::min();
  for (int direction = 0; direction < 4; direction++) {
    if (board.can_move(direction)) {
      game_board b = board;
      b.move(direction);
      int evaluate_score = minimax_recursion_add_(b, depth);
      if (evaluate_score > best_evaluate_score) {
        best_direction = direction;
      }
    }
  }
  return best_direction;
}

int player_minimax::minimax_recursion_move_(game_board board, int depth) {
  if (depth == 0 || board.is_terminated()) {
    return evaluate_(board);
  }

  int best_evaluate_score = std::numeric_limits<int>::min();
  for (int direction = 0; direction < 4; direction++) {
    if (board.can_move(direction)) {
      game_board b = board;
      b.move(direction);
      int evaluate_score = minimax_recursion_add_(b, depth);
      if (evaluate_score > best_evaluate_score) {
        best_evaluate_score = evaluate_score;
      }
    }
  }
  return best_evaluate_score;
}

int player_minimax::minimax_recursion_add_(game_board board, int depth) {
  int worst_evaluate_score = std::numeric_limits<int>::max();
  for (int position = 0; position < BOARD_SIZE * BOARD_SIZE; position++) {
    if (board.is_blank(position)) {
      game_board b1 = board;
      b1.add_tile(position, 1);
      int evaluate_score1 = minimax_recursion_move_(b1, depth - 1);
      if (evaluate_score1 < worst_evaluate_score) {
        worst_evaluate_score = evaluate_score1;
      }

      game_board b2 = board;
      b2.add_tile(position, 2);
      int evaluate_score2 = minimax_recursion_move_(b2, depth - 1);
      if (evaluate_score2 < worst_evaluate_score) {
        worst_evaluate_score = evaluate_score2;
      }
    }
  }
  return worst_evaluate_score;
}

#endif  // PLAYER_MINIMAX_HPP
