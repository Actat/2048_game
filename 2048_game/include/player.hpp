#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "game_board.hpp"
#include "move.hpp"

class player {
private:
public:
  virtual Move play(game_board const &board) {  // Override this function!
    if (board.can_move(DIRECTION_L)) {
      return Move(DIRECTION_L);
    } else if (board.can_move(DIRECTION_D)) {
      return Move(DIRECTION_D);
    } else if (board.can_move(DIRECTION_U)) {
      return Move(DIRECTION_U);
    } else {
      return Move(DIRECTION_R);
    }
  };
};

#endif  // PLAYER_HPP