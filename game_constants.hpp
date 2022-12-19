#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

#include <chrono>

static int const BOARD_SIZE  = 4;
static int const START_TILES = 2;
static int const DIRECTION_L = 0;
static int const DIRECTION_D = 1;
static int const DIRECTION_U = 2;
static int const DIRECTION_R = 3;

class time_keeper {
private:
  std::chrono::high_resolution_clock::time_point start_time_;
  int time_limit_ms_;

public:
  time_keeper(int const time_limit_ms) {
    start_time_    = std::chrono::high_resolution_clock::now();
    time_limit_ms_ = time_limit_ms;
  }

  int get_duration_ms() const {
    auto duration = std::chrono::high_resolution_clock::now() - start_time_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
        .count();
  }

  bool is_time_over() const {
    auto duration = std::chrono::high_resolution_clock::now() - start_time_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
               .count() >= time_limit_ms_;
  }
};

class hash_board_array {
public:
  size_t operator()(std::array<int, BOARD_SIZE * BOARD_SIZE> const &arr) const {
    const int C = 97;
    size_t t    = 0;
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
      t = t * C + arr.at(i);
    }
    return t;
  }
};

class hash_line_array {
public:
  size_t operator()(std::array<int, BOARD_SIZE> const &arr) const {
    const int C = 97;
    size_t t    = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
      t = t * C + arr.at(i);
    }
    return t;
  }
};

#endif  // GAME_CONSTANTS
