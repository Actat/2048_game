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

class GameTreeNode {
private:
  GameBoardEval eval_;
  GameBoard board_;
  std::vector<int> score_;
  bool is_player_turn_;
  int move_;
  std::vector<std::shared_ptr<GameTreeNode>> children_;

  bool add_children_();

public:
  GameTreeNode(GameBoardEval const &e,
                 GameBoard const &b,
                 bool is_player_turn,
                 int move);
  int best_move(int const depth, TimeKeeper const &tk);
  GameBoard const get_board() const;
  int get_eval_score(int const depth,
                     TimeKeeper const &tk,
                     int alpha,
                     int beta);
  int get_move() const;
  std::shared_ptr<GameTreeNode> find(GameBoard const &b) const;
};

class GameTree {
private:
  GameBoardEval eval_;
  std::shared_ptr<GameTreeNode> root_node_;

public:
  GameTree();
  int best_move(int const depth, TimeKeeper const &tk);
  void update_root(GameBoard const &b);
};

class PlayerGameTree : public Player {
private:
  GameTree tree_;

  int iterative_deeping_(GameBoard const &board, int time_limit_ms);

public:
  PlayerGameTree();
  Move play(GameBoard const &board) override {
    return iterative_deeping_(board, 500);
  };
};

#endif  // PLAYER_GAME_TREE_HPP
