#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <array>
#include <cmath>
#include <iomanip>
#include <queue>
#include <random>
#include <tuple>
#include <vector>

#define BOARD_SIZE 4
#define START_TILES 2
#define DIRECTION_L 0
#define DIRECTION_D 1
#define DIRECTION_U 2
#define DIRECTION_R 3

class game_board {
private:
  std::array<int, BOARD_SIZE * BOARD_SIZE> board_;
  int score_;

  std::mt19937 engine_;

  bool add_random_tile_();

public:
  game_board();
  void apply_array(int direction, int index, std::array<int, BOARD_SIZE> array);
  bool can_move(int direction);
  bool can_move(std::array<int, BOARD_SIZE> array);
  int count_blank_tile();
  std::array<int, BOARD_SIZE> fetch_array(int direction, int index);
  std::vector<int> find_blank_tiles();
  int get_neighbor_index(int index, int direction);
  void initialize();
  bool is_blank_tile_exists();
  bool is_match_available();
  bool is_move_available();
  void move(int direction);
  std::tuple<std::array<int, BOARD_SIZE>, int> move(
      std::array<int, BOARD_SIZE> array);

  friend std::ostream &operator<<(std::ostream &os, const game_board &board);
};

game_board::game_board() {
  std::random_device seed_gen;
  engine_ = std::mt19937(seed_gen());

  initialize();
}

void game_board::apply_array(int direction,
                             int index,
                             std::array<int, BOARD_SIZE> array) {
  switch (direction) {
    case DIRECTION_L:
      for (int i = 0; i < BOARD_SIZE; i++) {
        board_.at(BOARD_SIZE * index + i) = array.at(i);
      }
      return;
      break;

    case DIRECTION_R:
      for (int i = 0; i < BOARD_SIZE; i++) {
        board_.at(BOARD_SIZE * index + i) = array.at(BOARD_SIZE - 1 - i);
      }
      return;
      break;

    case DIRECTION_U:
      for (int i = 0; i < BOARD_SIZE; i++) {
        board_.at(BOARD_SIZE * i + index) = array.at(i);
      }
      return;
      break;

    case DIRECTION_D:
      for (int i = 0; i < BOARD_SIZE; i++) {
        board_.at(BOARD_SIZE * i + index) = array.at(BOARD_SIZE - 1 - i);
      }
      return;
      break;

    default:
      break;
  }
}

bool game_board::can_move(int direction) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (can_move(fetch_array(direction, i))) {
      return true;
    }
  }
  return false;
}

bool game_board::can_move(std::array<int, BOARD_SIZE> array) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (array.at(i) == 0) {
      return true;
    }
  }
  for (int i = 0; i < BOARD_SIZE - 1; i++) {
    if (array.at(i) == array.at(i + 1)) {
      return true;
    }
  }
  return false;
}

int game_board::count_blank_tile() {
  return (int)find_blank_tiles().size();
}

std::array<int, BOARD_SIZE> game_board::fetch_array(int direction, int index) {
  std::array<int, BOARD_SIZE> arr;
  switch (direction) {
    case DIRECTION_L:
      for (int i = 0; i < BOARD_SIZE; i++) {
        arr.at(i) = board_.at(BOARD_SIZE * index + i);
      }
      return arr;
      break;

    case DIRECTION_R:
      for (int i = 0; i < BOARD_SIZE; i++) {
        arr.at(BOARD_SIZE - 1 - i) = board_.at(BOARD_SIZE * index + i);
      }
      return arr;
      break;

    case DIRECTION_U:
      for (int i = 0; i < BOARD_SIZE; i++) {
        arr.at(i) = board_.at(BOARD_SIZE * i + index);
      }
      return arr;
      break;

    case DIRECTION_D:
      for (int i = 0; i < BOARD_SIZE; i++) {
        arr.at(BOARD_SIZE - 1 - i) = board_.at(BOARD_SIZE * i + index);
      }
      return arr;
      break;

    default:
      for (int i = 0; i < BOARD_SIZE; i++) {
        arr.at(i) = -1;
      }
      return arr;
      break;
  }
}

std::vector<int> game_board::find_blank_tiles() {
  std::vector<int> vec = {};
  for (size_t i = 0; i < board_.size(); i++) {
    if (board_.at(i) == 0) {
      vec.push_back((int)i);
    }
  }
  return vec;
}

int game_board::get_neighbor_index(int index, int direction) {
  switch (direction) {
    case DIRECTION_L:
      return index % BOARD_SIZE == 0 ? -1 : index - 1;
      break;

    case DIRECTION_R:
      return index % BOARD_SIZE == 3 ? -1 : index + 1;
      break;

    case DIRECTION_U:
      return index / BOARD_SIZE == 0 ? -1 : index + BOARD_SIZE;
      break;

    case DIRECTION_D:
      return index / BOARD_SIZE == BOARD_SIZE - 1 ? -1 : index + BOARD_SIZE;

    default:
      return -1;
      break;
  }
}

void game_board::initialize() {
  for (std::size_t i = 0; i < board_.size(); i++) {
    board_.at(i) = 0;
  }

  score_ = 0;
  for (int i = 0; i < START_TILES; i++) {
    add_random_tile_();
  }
}

bool game_board::is_blank_tile_exists() {
  return count_blank_tile() > 0;
}

bool game_board::is_match_available() {
  for (int i = 0; i < (int)board_.size(); i++) {
    for (int direction = 0; direction < 4; direction++) {
      int neighbor = get_neighbor_index(i, direction);
      if (neighbor >= 0 && neighbor < (int)board_.size() &&
          board_.at(neighbor) == board_.at(i)) {
        return true;
      }
    }
  }
  return false;
}

bool game_board::is_move_available() {
  return is_blank_tile_exists() || is_match_available();
}

void game_board::move(int direction) {
  for (int index = 0; index < BOARD_SIZE; index++) {
    auto moved = move(fetch_array(direction, index));
    auto array = std::get<0>(moved);
    auto score = std::get<1>(moved);
    apply_array(direction, index, array);
    score_ += score;
  }
}

std::tuple<std::array<int, BOARD_SIZE>, int> game_board::move(
    std::array<int, BOARD_SIZE> array) {
  int score = 0;
  std::array<int, BOARD_SIZE> result;
  for (int i = 0; i < BOARD_SIZE; i++) {
    result.at(i) = 0;
  }

  std::queue<int> queue;
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (array.at(i) > 0) {
      queue.push(array.at(i));
    }
  }

  int j = 0;
  while (!queue.empty()) {
    int f = queue.front();
    queue.pop();
    if (!queue.empty() && queue.front() == f) {
      queue.pop();
      result.at(j) = f + 1;
      score += (int)std::pow(2, result.at(j));
    } else {
      result.at(j) = f;
    }
    j++;
  }

  return std::tie(result, score);
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
  auto blanks = find_blank_tiles();
  if (blanks.size() == 0) {
    return false;
  }

  std::uniform_int_distribution<> tile_selector(0.0, (int)blanks.size() - 1);
  int blank_tile = blanks.at(tile_selector(engine_));

  std::uniform_real_distribution<> rand(0.0, 1.0);
  board_.at(blank_tile) = rand(engine_) < 0.9 ? 1 : 2;

  return true;
}

#endif  // GAME_BOARD_HPP
