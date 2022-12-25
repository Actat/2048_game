#ifndef PLAYER_MINIMAX_HPP
#define PLAYER_MINIMAX_HPP

#include <array>
#include <iostream>
#include <limits>
#include <unordered_map>
#include "game_board.hpp"
#include "game_board_eval.hpp"
#include "player.hpp"

class player_minimax : public player {
private:
  game_board_eval eval_;

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
  player_minimax();
  int play(game_board const &board) override {
    return iterative_deeping_(board, 500);
  };
};

#endif  // PLAYER_MINIMAX_HPP
