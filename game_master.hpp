#ifndef GAME_MASTER_HPP
#define GAME_MASTER_HPP

#include <functional>
#include <iostream>
#include <tuple>
#include "game_board.hpp"
#include "player_console.hpp"

class game_master {
private:
  game_board board_;

public:
  game_master();
  std::tuple<game_board> game(std::function<int(game_board)> player);
};

game_master::game_master() {
  board_ = game_board();
}

std::tuple<game_board> game_master::game(
    std::function<int(game_board)> player) {
  while (!board_.is_terminated()) {
    std::cout << board_ << std::endl;
    game_board b = board_;
    board_.input(player(b));
  }
  std::cout << "game terminated" << std::endl;
  return std::make_tuple(board_);
}

#endif  // GAME_MASTER_HPP
