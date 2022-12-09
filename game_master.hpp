#ifndef GAME_MASTER_HPP
#define GAME_MASTER_HPP

#include <chrono>
#include <iostream>
#include <tuple>
#include "game_board.hpp"
#include "game_constants.hpp"
#include "player.hpp"

class game_master {
private:
  game_board board_;
  std::mt19937 engine_;

  bool add_random_tile_();

public:
  std::tuple<game_board> game(player *player);
  std::tuple<game_board> game(player *player, bool show_progress);
};

std::tuple<game_board> game_master::game(player *player) {
  return game(player, true);
}

std::tuple<game_board> game_master::game(player *player, bool show_progress) {
  std::random_device seed_gen;
  engine_ = std::mt19937(seed_gen());

  board_ = game_board();
  for (int i = 0; i < START_TILES; i++) {
    add_random_tile_();
  }

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
    if (board_.can_move(input)) {
      board_.move(input);
      add_random_tile_();
    }
  }
  if (show_progress) {
    std::cout << "game terminated" << std::endl;
  }
  return std::make_tuple(board_);
}

// private --------------------------------------------------------------------

bool game_master::add_random_tile_() {
  auto blanks = board_.find_blank_tiles();
  if (blanks.size() == 0) {
    return false;
  }

  std::uniform_int_distribution<> tile_selector(0, (int)blanks.size() - 1);
  int blank_tile = blanks.at(tile_selector(engine_));

  std::uniform_real_distribution<> rand(0.0, 1.0);
  board_.add_tile(blank_tile, rand(engine_) < 0.9 ? 1 : 2);

  return true;
}

#endif  // GAME_MASTER_HPP
