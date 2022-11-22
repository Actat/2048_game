#include <iostream>
#include "game_board.hpp"
#include "game_master.hpp"
#include "player.hpp"

int main(int argc, char *argv[]) {
  auto m = game_master();
  auto p = player();
  game_board board;
  std::tie(board) = m.game(p);
  std::cout << board << std::endl;

  return 0;
}