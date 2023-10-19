#ifndef GAME_MASTER_HPP
#define GAME_MASTER_HPP

#include <chrono>
#include <functional>
#include <iostream>
#include <random>
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
  std::function<void(game_board const &)> on_game_start;
  std::function<void(game_board const &)> on_turn_finished;
  std::function<void(game_board const &)> on_game_terminated;

  std::tuple<game_board> game(player &player);
};

#endif  // GAME_MASTER_HPP
