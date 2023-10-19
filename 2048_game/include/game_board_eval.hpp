#ifndef GAME_BOARD_EVAL_HPP
#define GAME_BOARD_EVAL_HPP

#include <array>
#include <unordered_map>
#include "game_board.hpp"
#include "game_constants.hpp"

class GameBoardEval {
private:
  int evaluate_line_(std::array<int, BOARD_SIZE> &line) const;

public:
  int evaluate(GameBoard const &board) const;
};

#endif  // GAME_BOARD_EVAL_HPP
