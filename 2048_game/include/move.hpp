#ifndef MOVE_HPP
#define MOVE_HPP

#include "game_constants.hpp"

class Move {
private:
  int direction_;

public:
  Move() { direction_ = DIRECTION_L; };
  Move(int direction) {
    direction_ = DIRECTION_L;
    if (direction == DIRECTION_L || direction == DIRECTION_D ||
        direction == DIRECTION_U || direction == DIRECTION_R) {
      direction_ = direction;
    }
  };
  int direction() const { return direction_; };
  void set_dir(int direction) {
    if (direction == DIRECTION_L || direction == DIRECTION_D ||
        direction == DIRECTION_U || direction == DIRECTION_R) {
      direction_ = direction;
    }
  };
};

#endif  // MOVE_HPP