#ifndef GAME_MASTER_HPP
#define GAME_MASTER_HPP

#include <chrono>
#include <iostream>
#include <tuple>
#include "game_board.hpp"
#include "player.hpp"

class game_master {
private:
  game_board board_;

public:
  std::tuple<game_board> game(player *player);
  std::tuple<game_board> game(player *player, bool show_progress);
};

std::tuple<game_board> game_master::game(player *player) {
  return game(player, true);
}

std::tuple<game_board> game_master::game(player *player, bool show_progress) {
  board_ = game_board();
  while (!board_.is_terminated()) {
    if (show_progress) {
      std::cout << board_ << std::endl;
    }
    game_board b  = board_;
    auto time_s   = std::chrono::system_clock::now();
    int input     = player->play(b);
    auto time_e   = std::chrono::system_clock::now();
    auto duration = time_e - time_s;
    auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    if (show_progress) {
      std::cout << "Player takes " << msec << " ms." << std::endl;
    }
    board_.input(input);
  }
  if (show_progress) {
    std::cout << "game terminated" << std::endl;
  }
  return std::make_tuple(board_);
}

#endif  // GAME_MASTER_HPP
