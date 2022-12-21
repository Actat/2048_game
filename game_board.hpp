#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>
#include "game_constants.hpp"

class game_board {
private:
  std::array<int, BOARD_SIZE * BOARD_SIZE> board_;
  int score_;
  int turn_;
  int tile_sum_;

public:
  game_board();
  game_board(game_board const &board);

  // for game master ----------------------------------------------------------
  bool add_tile(int position, int tile);
  bool can_move(int direction) const;
  std::vector<int> find_blank_tiles() const;
  bool is_terminated() const;
  void move(int direction);

  // for game player ----------------------------------------------------------
  std::array<int, BOARD_SIZE * BOARD_SIZE> const &get_board_array() const;
  int get_largest_tile() const;
  int get_score() const;
  int get_tile_sum() const;
  int get_turn() const;
  bool is_blank(int position) const;
  bool is_blank_tile_exists() const;
  bool is_match_available() const;
  bool is_move_available() const;

  // other methods ------------------------------------------------------------
  void apply_line(int direction, int index, std::array<int, BOARD_SIZE> array);
  bool can_move(std::array<int, BOARD_SIZE> array) const;
  std::array<int, BOARD_SIZE> fetch_line(int direction, int index) const;
  int get_neighbor_index(int index, int direction) const;
  std::tuple<std::array<int, BOARD_SIZE>, int> move(
      std::array<int, BOARD_SIZE> array);

  friend std::ostream &operator<<(std::ostream &os, const game_board &board);
};

#endif  // GAME_BOARD_HPP
