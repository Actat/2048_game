#ifndef PLAYER_CONSOLE_HPP
#define PLAYER_CONSOLE_HPP

#include "game_board.hpp"
#include "player.hpp"

class player_random : public player {
private:
  std::mt19937 engine_;

public:
  player_random();
  int play(game_board board) override;
};

player_random::player_random() {
  std::random_device seed_gen;
  engine_ = std::mt19937(seed_gen());
}

int player_random::play(game_board board) {
  while (true) {
    std::uniform_int_distribution<> direction_selector(0, 3);
    int direction = direction_selector(engine_);
    if (board.can_move(direction)) {
      return direction;
    }
  }
}

#endif  // PLAYER_CONSOLE_HPP