#include "game_board.hpp"

game_board::game_board() {
  board_.fill(0);
  score_ = 0;
  turn_  = 0;
}

game_board::game_board(game_board const &board) {
  score_ = board.score_;
  turn_  = board.turn_;
  std::copy(board.board_.begin(), board.board_.end(), board_.begin());
}

// for game master ------------------------------------------------------------

bool game_board::add_tile(int position, int tile) {
  if (position < 0 || position > BOARD_SIZE * BOARD_SIZE) {
    return false;
  }
  if (tile != 1 && tile != 2) {
    return false;
  }
  board_.at(position) = tile;
  return true;
}

bool game_board::can_move(int direction) const {
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (can_move(fetch_array(direction, i))) {
      return true;
    }
  }
  return false;
}

std::vector<int> game_board::find_blank_tiles() const {
  std::vector<int> vec = {};
  for (std::size_t i = 0; i < board_.size(); i++) {
    if (board_.at(i) == 0) {
      vec.push_back((int)i);
    }
  }
  return vec;
}

bool game_board::is_terminated() const {
  return !is_move_available();
}

void game_board::move(int direction) {
  for (int index = 0; index < BOARD_SIZE; index++) {
    auto moved = move(fetch_array(direction, index));
    auto array = std::get<0>(moved);
    auto score = std::get<1>(moved);
    apply_array(direction, index, array);
    score_ += score;
    turn_++;
  }
}

// for game player ------------------------------------------------------------

int game_board::get_largest_tile() const {
  return *std::max_element(board_.begin(), board_.end());
}

int game_board::get_score() const {
  return score_;
}
int game_board::get_turn() const {
  return turn_;
}

bool game_board::is_blank(int position) const {
  return board_.at(position) == 0;
}

bool game_board::is_blank_tile_exists() const {
  return find_blank_tiles().size() > 0;
}

bool game_board::is_match_available() const {
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

bool game_board::is_move_available() const {
  return is_blank_tile_exists() || is_match_available();
}

// other methods --------------------------------------------------------------

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

bool game_board::can_move(std::array<int, BOARD_SIZE> array) const {
  bool is_tile_included = false;
  for (int i = BOARD_SIZE - 1; i >= 0; i--) {
    if (array.at(i) == 0 && is_tile_included) {
      return true;
    }
    if (array.at(i) > 0) {
      is_tile_included = true;
      if (i > 0 && array.at(i) == array.at(i - 1)) {
        return true;
      }
    }
  }
  return false;
}

std::array<int, BOARD_SIZE> game_board::fetch_array(int direction,
                                                    int index) const {
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

int game_board::get_neighbor_index(int index, int direction) const {
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
