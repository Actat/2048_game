#include <iostream>
#include "game_board.hpp"
#include "game_master.hpp"
#include "player_console.hpp"

int main(int argc, char *argv[]) {
  auto m             = game_master();
  m.on_game_start    = [](game_board const &b) { std::cout << b << std::endl; };
  m.on_turn_finished = [](game_board const &b) { std::cout << b << std::endl; };
  m.on_game_terminated = [](game_board const &b) {
    std::cout << "Game terminated." << std::endl;
  };
  auto p = player_console();
  game_board board;
  std::tie(board) = m.game(p);
  std::cout << board << std::endl;

  return 0;
}