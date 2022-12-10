#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <queue>
#include <random>
#include <tuple>
#include <vector>
#include "game_constants.hpp"

class game_board {
private:
  std::array<int, BOARD_SIZE * BOARD_SIZE> board_;
  int score_;
  int turn_;

public:
  game_board();
  game_board(game_board const &board);

  bool add_tile(int position, int tile);
  bool can_move(int direction) const;
  std::vector<int> find_blank_tiles() const;
  bool is_terminated() const;
  void move(int direction);

  int count_blank_tile() const;
  int get_largest_tile() const;
  int get_score() const;
  int get_turn() const;
  bool is_blank(int position) const;
  bool is_blank_tile_exists() const;
  bool is_match_available() const;
  bool is_move_available() const;

  void apply_array(int direction, int index, std::array<int, BOARD_SIZE> array);
  bool can_move(std::array<int, BOARD_SIZE> array);
  std::array<int, BOARD_SIZE> fetch_array(int direction, int index);
  int get_neighbor_index(int index, int direction);
  std::tuple<std::array<int, BOARD_SIZE>, int> move(
      std::array<int, BOARD_SIZE> array);

  friend std::ostream &operator<<(std::ostream &os, const game_board &board);
};

#endif  // GAME_BOARD_HPP
