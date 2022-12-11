#include "player_minimax.hpp"

player_minimax::player_minimax() {
  eval_ = game_board_eval();
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
    return eval_.evaluate(board);
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
