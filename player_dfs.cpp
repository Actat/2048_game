#include "player_dfs.hpp"

player_dfs::player_dfs() {
  eval_ = game_board_eval();
}

int player_dfs::iterative_deeping_(game_board const &board,
                                   int time_limit_ms) const {
  auto tk            = time_keeper(time_limit_ms);
  int best_direction = -1;
  for (int depth = 1;; depth++) {
    int dir = iterative_deeping_dfs_(board, depth, tk);

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

int player_dfs::iterative_deeping_dfs_(game_board const &board,
                                       int depth,
                                       time_keeper const &tk) const {
  int best_direction      = -1;
  int best_evaluate_score = std::numeric_limits<int>::min();
  for (int direction = 0; direction < 4; direction++) {
    if (board.can_move(direction)) {
      auto b = game_board(board);
      b.move(direction);
      int evaluate_score = iterative_deeping_dfs_add_(b, depth, tk);

      if (tk.is_time_over()) {
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

int player_dfs::iterative_deeping_dfs_move_(game_board const &board,
                                            int depth,
                                            time_keeper const &tk) const {
  if (tk.is_time_over()) {
    return std::numeric_limits<int>::min();
  }

  if (depth == 0 || board.is_terminated()) {
    return eval_.evaluate(board);
  }

  auto legal_move = std::vector<int>{};
  for (int dir = 0; dir < 4; dir++) {
    if (board.can_move(dir)) {
      legal_move.push_back(dir);
    }
  }

  int best_eval_score = std::numeric_limits<int>::min();
  std::for_each(legal_move.begin(), legal_move.end(),
                [this, &board, &depth, &tk, &best_eval_score](int direction) {
                  auto b = game_board(board);
                  b.move(direction);
                  int eval_score = iterative_deeping_dfs_add_(b, depth - 1, tk);

                  if (tk.is_time_over()) {
                    return;
                  }

                  if (eval_score >= best_eval_score) {
                    best_eval_score = eval_score;
                  }
                });

  return best_eval_score;
}

int player_dfs::iterative_deeping_dfs_add_(game_board const &board,
                                           int depth,
                                           time_keeper const &tk) const {
  if (tk.is_time_over()) {
    return std::numeric_limits<int>::min();
  }

  if (depth == 0 || board.is_terminated()) {
    return eval_.evaluate(board);
  }

  double eval_score = 0;
  auto blanks       = board.find_blank_tiles();
  std::for_each(
      blanks.begin(), blanks.end(),
      [this, &board, &depth, &tk, &eval_score](int position) {
        auto b1 = game_board(board);
        b1.add_tile(position, 1);
        eval_score += 9 * iterative_deeping_dfs_move_(b1, depth - 1, tk);
        if (tk.is_time_over()) {
          return;
        }

        auto b2 = game_board(board);
        b2.add_tile(position, 2);
        eval_score += 1 * iterative_deeping_dfs_move_(b2, depth - 1, tk);
        if (tk.is_time_over()) {
          return;
        }
      });
  if (tk.is_time_over()) {
    return std::numeric_limits<int>::min();
  }

  return (int)eval_score / (blanks.size() * 10.0);
}
