#include <iostream>
#include "game_board.hpp"
#include "game_master.hpp"
#include "player_console.hpp"

int main(int argc, char *argv[]) {
  auto m = game_master();
  auto p = player_console();
  game_board board;
  std::tie(board) = m.game(p);
  std::cout << board << std::endl;

  return 0;
}