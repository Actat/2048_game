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

class GameMaster {
private:
  GameBoard board_;
  std::mt19937 engine_;

  bool add_random_tile_();

public:
  std::function<void(GameBoard const &)> on_game_start;
  std::function<void(GameBoard const &)> on_turn_finished;
  std::function<void(GameBoard const &)> on_game_terminated;

  std::tuple<GameBoard> game(Player &player);
};

#endif  // GAME_MASTER_HPP
