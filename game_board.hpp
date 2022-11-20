#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <array>
#include <cmath>
#include <iomanip>
#include <random>
#include <vector>

#define BOARD_SIZE 4
#define START_TILES 2

class game_board {
private:
  std::array<int, BOARD_SIZE * BOARD_SIZE> board_;
  int score_;

  std::mt19937 engine_;

  bool add_random_tile_();
  int count_blank_tile_();
  std::vector<int> find_blank_tiles_();
  bool is_blank_tile_exists_();

public:
  game_board();
  void initialize();

  friend std::ostream &operator<<(std::ostream &os, const game_board &board);
};

game_board::game_board() {
  std::random_device seed_gen;
  engine_ = std::mt19937(seed_gen());

  initialize();
}

void game_board::initialize() {
  for (std::size_t i = 0; i < board_.size(); i++) {
    board_[i] = 0;
  }

  score_ = 0;
  for (int i = 0; i < START_TILES; i++) {
    add_random_tile_();
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

// private --------------------------------------------------------------------

bool game_board::add_random_tile_() {
  auto blanks = find_blank_tiles_();
  if (blanks.size() == 0) {
    return false;
  }

  std::uniform_int_distribution<> tile_selector(0.0, (int)blanks.size() - 1);
  int blank_tile = blanks.at(tile_selector(engine_));

  std::uniform_real_distribution<> rand(0.0, 1.0);
  board_.at(blank_tile) = rand(engine_) < 0.9 ? 1 : 2;

  return true;
}

int game_board::count_blank_tile_() {
  return (int)find_blank_tiles_().size();
}

std::vector<int> game_board::find_blank_tiles_() {
  std::vector<int> vec = {};
  for (size_t i = 0; i < board_.size(); i++) {
    if (board_.at(i) == 0) {
      vec.push_back((int)i);
    }
  }
  return vec;
}

bool game_board::is_blank_tile_exists_() {
  return count_blank_tile_() > 0;
}

#endif  // GAME_BOARD_HPP
