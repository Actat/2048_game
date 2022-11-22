#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "game_board.hpp"

class player {
private:
public:
  virtual int play(game_board board) {  // Override this function!
    if (board.can_move(DIRECTION_L)) {
      return DIRECTION_L;
    } else if (board.can_move(DIRECTION_D)) {
      return DIRECTION_D;
    } else if (board.can_move(DIRECTION_U)) {
      return DIRECTION_U;
    } else {
      return DIRECTION_R;
    }
  };
};

#endif  // PLAYER_HPP