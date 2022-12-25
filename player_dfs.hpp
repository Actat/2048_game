#ifndef PLAYER_DFS_HPP
#define PLAYER_DFS_HPP

#include <array>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>
#include "game_board.hpp"
#include "game_board_eval.hpp"
#include "player.hpp"

class player_dfs : public player {
private:
  game_board_eval eval_;

  int iterative_deeping_(game_board const &board, int time_limit_ms) const;
  int iterative_deeping_dfs_(game_board const &board,
                             int depth,
                             time_keeper const &tk) const;
  int iterative_deeping_dfs_move_(game_board const &board,
                                  int depth,
                                  time_keeper const &tk) const;
  int iterative_deeping_dfs_add_(game_board const &board,
                                 int depth,
                                 time_keeper const &tk) const;

public:
  player_dfs();
  int play(game_board const &board) override {
    return iterative_deeping_(board, 500);
  };
};

#endif  // PLAYER_DFS_HPP
