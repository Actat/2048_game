#ifndef PLAYER_GAME_TREE_HPP
#define PLAYER_GAME_TREE_HPP

#include <array>
#include <iostream>
#include <limits>
#include <memory>
#include <unordered_map>
#include <vector>
#include "game_board.hpp"
#include "game_board_eval.hpp"
#include "player.hpp"

class game_tree_node {
private:
  game_board_eval eval_;
  game_board board_;
  std::vector<int> score_;
  bool is_player_turn_;
  int move_;
  std::vector<std::shared_ptr<game_tree_node>> children_;

  bool add_children_();

public:
  game_tree_node(game_board_eval const &e,
                 game_board const &b,
                 bool is_player_turn,
                 int move);
  int best_move(int const depth, time_keeper const &tk);
  game_board const get_board() const;
  int get_eval_score(int const depth, time_keeper const &tk);
  int get_move() const;
  std::shared_ptr<game_tree_node> find(game_board const &b) const;
};

class game_tree {
private:
  game_board_eval eval_;
  std::shared_ptr<game_tree_node> root_node_;

public:
  game_tree();
  int best_move(int const depth, time_keeper const &tk);
  void update_root(game_board const &b);
};

class player_game_tree : public player {
private:
  game_tree tree_;

  int iterative_deeping_(game_board const &board, int time_limit_ms);

public:
  player_game_tree();
  int play(game_board const &board) override {
    return iterative_deeping_(board, 500);
  };
};

#endif  // PLAYER_GAME_TREE_HPP
