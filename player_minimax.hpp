#ifndef PLAYER_MINIMAX_HPP
#define PLAYER_MINIMAX_HPP

#include <limits>
#include "game_board.hpp"
#include "player.hpp"

class player_minimax : public player {
private:
  int evaluate_(game_board board);
  int minimax_(game_board board, int depth);
  int minimax_recursion_move_(game_board board, int depth);
  int minimax_recursion_add_(game_board board, int depth);

public:
  virtual int play(game_board board) { return minimax_(board, 2); };
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
