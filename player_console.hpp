#ifndef PLAYER_CONSOLE_HPP
#define PLAYER_CONSOLE_HPP

#include <functional>
#include <string>
#include "game_board.hpp"
#include "player.hpp"

class player_console : public player {
private:
  int get_input(game_board board);

public:
  int play(game_board board) override { return get_input(board); };
};

int player_console::get_input(game_board board) {
  int direction;
  std::string str;
  while (true) {
    std::cout << "input hjkl: ";
    std::cin >> str;
    switch (str.at(0)) {
      case 'h':
        direction = DIRECTION_L;
        break;

      case 'j':
        direction = DIRECTION_D;
        break;

      case 'k':
        direction = DIRECTION_U;
        break;

      case 'l':
        direction = DIRECTION_R;
        break;

      default:
        direction = -1;
        break;
    }
    if (board.can_move(direction)) {
      return direction;
    }
  }
}

#endif  // PLAYER_CONSOLE_HPP