#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <array>
#include <cmath>
#include <iomanip>

#define BOARD_SIZE 4

class game_board {
private:
  std::array<int, BOARD_SIZE * BOARD_SIZE> board_;
  int score_;

public:
  game_board();
  void initialize();

  friend std::ostream &operator<<(std::ostream &os, const game_board &board);
};

game_board::game_board() {
  initialize();
}

void game_board::initialize() {
  score_ = 0;
  for (std::size_t i = 0; i < board_.size(); i++) {
    board_[i] = 0;
  }
}

std::ostream &operator<<(std::ostream &os, const game_board &board) {
  os << "score: " << board.score_ << std::endl;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      os << "+-----";
    }
    os << "+" << std::endl;
    for (int j = 0; j < BOARD_SIZE; j++) {
      os << "|";
      if (board.board_.at(i * BOARD_SIZE + j) == 0) {
        os << "     ";
      } else {
        os << std::setw(5) << std::pow(2, board.board_.at(i * BOARD_SIZE + j));
      }
    }
    os << "|" << std::endl;
  }
  for (int i = 0; i < BOARD_SIZE; i++) {
    os << "+-----";
  }
  os << "+" << std::endl;
  return os;
}

#endif  // GAME_BOARD_HPP
