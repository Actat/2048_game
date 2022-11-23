#include <iostream>
#include "game_board.hpp"
#include "game_master.hpp"
#include "player_random.hpp"

#define NUM_TRIAL 100

int main(int argc, char *argv[]) {
  std::array<game_board, NUM_TRIAL> result;

  for (int i = 0; i < NUM_TRIAL; i++) {
    auto m = game_master();
    auto p = player_random();

    game_board board;
    std::tie(board) = m.game(&p);
    std::cout << board << std::endl;

    result.at(i) = board;
  }

  return 0;
}