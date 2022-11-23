#ifndef GAME_MASTER_HPP
#define GAME_MASTER_HPP

#include <iostream>
#include <tuple>
#include "game_board.hpp"
#include "player.hpp"

class game_master {
private:
  game_board board_;

public:
  std::tuple<game_board> game(player *player);
};

std::tuple<game_board> game_master::game(player *player) {
  board_ = game_board();
  while (!board_.is_terminated()) {
    std::cout << board_ << std::endl;
    game_board b = board_;
    board_.input(player->play(b));
  }
  std::cout << "game terminated" << std::endl;
  return std::make_tuple(board_);
}

#endif  // GAME_MASTER_HPP
