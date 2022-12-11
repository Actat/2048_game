#ifndef PLAYER_MINIMAX_HPP
#define PLAYER_MINIMAX_HPP

#include <array>
#include <iostream>
#include <limits>
#include <unordered_map>
#include "game_board.hpp"
#include "player.hpp"

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

#endif  // PLAYER_MINIMAX_HPP
