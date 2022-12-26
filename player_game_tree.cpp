#include "player_game_tree.hpp"

game_tree_node::game_tree_node(game_board_eval const &e,
                               game_board const &b,
                               bool is_player_turn,
                               int move) {
  eval_           = e;
  board_          = b;
  is_player_turn_ = is_player_turn;
  move_           = move;
  score_          = eval_.evaluate(board_);

  children_ = std::vector<std::shared_ptr<game_tree_node>>();
}

int game_tree_node::best_move(int const depth, time_keeper const &tk) {
  if (children_.empty()) {
    add_children_();
    if (tk.is_time_over()) {
      return score_;
    }
  }

  int best_move  = -1;
  int best_score = std::numeric_limits<int>::min();
  for (auto child : children_) {
    int const s = child->get_eval_score(depth - 1, tk);

    if (tk.is_time_over()) {
      return -1;
    }

    if (s > best_score) {
      best_move  = child->get_move();
      best_score = s;
    }
  }
  return best_move;
}

game_board const game_tree_node::get_board() const {
  return board_;
}

int game_tree_node::get_eval_score(int const depth, time_keeper const &tk) {
  if (depth == 0) {
    return score_;
  }

  if (tk.is_time_over()) {
    return score_;
  }

  if (children_.empty()) {
    add_children_();
    if (tk.is_time_over()) {
      return score_;
    }
  }

  if (is_player_turn_) {
    int best = std::numeric_limits<int>::min();
    for (auto child : children_) {
      int const s = child->get_eval_score(depth - 1, tk);

      if (tk.is_time_over()) {
        return score_;
      }

      if (s > best) {
        best = s;
      }
    }
    return best;
  } else {
    int sum = 0;
    for (auto child : children_) {
      int const s = child->get_eval_score(depth - 1, tk);

      if (tk.is_time_over()) {
        return score_;
      }

      sum += s;
    }
    return sum / children_.size();
  }
}

int game_tree_node::get_move() const {
  return move_;
}

std::shared_ptr<game_tree_node> game_tree_node::find(
    game_board const &b) const {
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

bool game_tree_node::add_children_() {
  if (is_player_turn_) {
    for (int i = 0; i < 4; i++) {
      auto b = game_board(board_);
      if (b.can_move(i)) {
        b.move(i);
        std::shared_ptr<game_tree_node> ptr =
            std::make_shared<game_tree_node>(eval_, b, !is_player_turn_, i);
        children_.push_back(ptr);
      }
    }
  } else {
    auto blanks = board_.find_blank_tiles();
    for (auto blank : blanks) {
      auto b1 = game_board(board_);
      auto b2 = game_board(board_);
      b1.add_tile(blank, 1);
      b2.add_tile(blank, 2);
      std::shared_ptr<game_tree_node> ptr1 =
          std::make_shared<game_tree_node>(eval_, b1, !is_player_turn_, -1);
      std::shared_ptr<game_tree_node> ptr2 =
          std::make_shared<game_tree_node>(eval_, b2, !is_player_turn_, -1);
      children_.push_back(ptr1);
      children_.push_back(ptr2);
    }
  }
  return true;
}

game_tree::game_tree() {
  eval_      = game_board_eval();
  root_node_ = nullptr;
}

int game_tree::best_move(int const depth, time_keeper const &tk) {
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

void game_tree::update_root(game_board const &b) {
  if (!root_node_) {
    root_node_ = std::make_shared<game_tree_node>(eval_, b, true, -1);
    return;
  }

  if (root_node_->get_board() == b) {
    return;
  }

  root_node_ = std::move(root_node_.get()->find(b));
}

player_game_tree::player_game_tree() {
  tree_ = game_tree();
}

int player_game_tree::iterative_deeping_(game_board const &board,
                                         int time_limit_ms) {
  auto tk            = time_keeper(time_limit_ms);
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
