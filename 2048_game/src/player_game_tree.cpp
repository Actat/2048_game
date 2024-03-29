#include "../include/player_game_tree.hpp"

GameTreeNode::GameTreeNode(GameBoardEval const &e,
                           GameBoard const &b,
                           bool is_player_turn,
                           int move) {
  eval_           = e;
  board_          = b;
  is_player_turn_ = is_player_turn;
  move_           = move;
  score_          = std::vector<int>{eval_.evaluate(board_)};

  children_ = std::vector<std::shared_ptr<GameTreeNode>>();
}

int GameTreeNode::best_move(int const depth, TimeKeeper const &tk) {
  if (children_.empty()) {
    add_children_();
    if (tk.is_time_over()) {
      return score_.at(0);
    }
  }

  int best_move  = -1;
  int best_score = std::numeric_limits<int>::min();
  for (auto child : children_) {
    int const s =
        child->get_eval_score(depth - 1, tk, std::numeric_limits<int>::min(),
                              std::numeric_limits<int>::max());

    if (tk.is_time_over()) {
      return -1;
    }

    if (s >= best_score) {
      best_move  = child->get_move();
      best_score = s;
    }
  }
  return best_move;
}

GameBoard const GameTreeNode::get_board() const {
  return board_;
}

int GameTreeNode::get_eval_score(int const depth,
                                 TimeKeeper const &tk,
                                 int alpha,
                                 int beta) {
  if (depth < score_.size()) {
    return score_.at(depth);
  }

  if (tk.is_time_over()) {
    return score_.at(0);
  }

  if (children_.empty()) {
    add_children_();
    if (tk.is_time_over()) {
      return score_.at(0);
    }
  }

  if (is_player_turn_) {
    for (auto child : children_) {
      alpha =
          std::max(alpha, child->get_eval_score(depth - 1, tk, alpha, beta));

      if (tk.is_time_over()) {
        return score_.at(0);
      }

      if (alpha >= beta) {
        break;
      }
    }
    score_.push_back(alpha);
    return alpha;
  } else {
    for (auto child : children_) {
      beta = std::min(beta, child->get_eval_score(depth - 1, tk, alpha, beta));

      if (tk.is_time_over()) {
        return score_.at(0);
      }

      if (alpha >= beta) {
        break;
      }
    }
    score_.push_back(beta);
    return beta;
  }
}

int GameTreeNode::get_move() const {
  return move_;
}

std::shared_ptr<GameTreeNode> GameTreeNode::find(GameBoard const &b) const {
  for (auto &child : children_) {
    if (child.get()->get_board().get_tile_sum() < b.get_tile_sum()) {
      auto ptr = child.get()->find(b);
      if (ptr) {
        return ptr;
      }
    } else if (child.get()->get_board().get_tile_sum() == b.get_tile_sum()) {
      if (child.get()->get_board() == b) {
        return child;
      }
    }
  }
  return nullptr;
}

bool GameTreeNode::add_children_() {
  if (is_player_turn_) {
    for (int i = 0; i < 4; i++) {
      auto b = GameBoard(board_);
      if (b.can_move(i)) {
        b.move(i);
        std::shared_ptr<GameTreeNode> ptr =
            std::make_shared<GameTreeNode>(eval_, b, !is_player_turn_, i);
        children_.push_back(ptr);
      }
    }
  } else {
    auto blanks = board_.find_blank_tiles();
    for (auto blank : blanks) {
      auto b1 = GameBoard(board_);
      auto b2 = GameBoard(board_);
      b1.add_tile(blank, 1);
      b2.add_tile(blank, 2);
      std::shared_ptr<GameTreeNode> ptr1 =
          std::make_shared<GameTreeNode>(eval_, b1, !is_player_turn_, -1);
      std::shared_ptr<GameTreeNode> ptr2 =
          std::make_shared<GameTreeNode>(eval_, b2, !is_player_turn_, -1);
      children_.push_back(ptr1);
      children_.push_back(ptr2);
    }
  }
  return true;
}

GameTree::GameTree() {
  eval_      = GameBoardEval();
  root_node_ = nullptr;
}

int GameTree::best_move(int const depth, TimeKeeper const &tk) {
  if (!root_node_) {
    return -1;
  }

  int best = root_node_.get()->best_move(depth, tk);
  if (tk.is_time_over()) {
    return -1;
  } else {
    return best;
  }
}

void GameTree::update_root(GameBoard const &b) {
  if (!root_node_) {
    root_node_ = std::make_shared<GameTreeNode>(eval_, b, true, -1);
    return;
  }

  if (root_node_->get_board() == b) {
    return;
  }

  root_node_ = std::move(root_node_.get()->find(b));
}

PlayerGameTree::PlayerGameTree() {
  tree_ = GameTree();
}

int PlayerGameTree::iterative_deeping_(GameBoard const &board,
                                       int time_limit_ms) {
  auto tk            = TimeKeeper(time_limit_ms);
  int best_direction = -1;

  tree_.update_root(board);

  std::cout << "Root is updated in " << tk.get_duration_ms() << " ms."
            << std::endl;

  for (int depth = 1;; depth++) {
    int dir = tree_.best_move(depth, tk);

    if (tk.is_time_over()) {
      std::cout << std::endl
                << "Time is up. depth: " << depth - 1  //
                << ", direction: " << best_direction << std::endl
                << std::endl;
      break;
    }

    best_direction = dir;
    std::cout << "depth: " << depth
              << " completed. best_direction: " << best_direction
              << " time passed: " << tk.get_duration_ms() << std::endl;
  }

  return best_direction;
}
