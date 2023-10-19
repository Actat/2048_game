#ifndef PLAYER_CONSOLE_HPP
#define PLAYER_CONSOLE_HPP

#include <iostream>
#include <string>
#include "game_board.hpp"
#include "player.hpp"

class PlayerConsole : public Player {
private:
  int get_input(GameBoard board) const;

public:
  Move play(GameBoard const &board) override { return get_input(board); };
};

int PlayerConsole::get_input(GameBoard board) const {
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