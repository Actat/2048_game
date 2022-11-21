#include <iostream>
#include "game_board.hpp"
#include "game_master.hpp"
#include "player_console.hpp"

int main(int argc, char *argv[]) {
  game_master master    = game_master();
  player_console player = player_console();
  game_board board;
  std::tie(board) = master.game(player.play);
  std::cout << board << std::endl;

  return 0;
}