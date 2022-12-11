#ifndef GAME_BOARD_EVAL_HPP
#define GAME_BOARD_EVAL_HPP

#include <array>
#include <unordered_map>
#include "game_board.hpp"
#include "game_constants.hpp"

class game_board_eval {
private:
  static std::unordered_map<std::array<int, BOARD_SIZE * BOARD_SIZE>,
                            int,
                            hash_board_array>
      board_cache_;
  static std::unordered_map<std::array<int, BOARD_SIZE>, int, hash_line_array>
      line_cache_;

public:
  int evaluate(game_board const &board) const;
};

#endif  // GAME_BOARD_EVAL_HPP
