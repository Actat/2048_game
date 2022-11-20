#include <iostream>
#include "game_board.hpp"

int main(int argc, char *argv[]) {
  game_board board = game_board();
  std::cout << board << std::endl;

  return 0;
}