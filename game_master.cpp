#include "game_master.hpp"

std::tuple<game_board> game_master::game(player &player) {
  std::random_device seed_gen;
  engine_ = std::mt19937(seed_gen());

  board_ = game_board();
  for (int i = 0; i < START_TILES; i++) {
    add_random_tile_();
  }
  if (on_game_start) {
    on_game_start(board_);
  }

  while (!board_.is_terminated()) {
    game_board b  = board_;
    auto time_s   = std::chrono::system_clock::now();
    int input     = player.play(b);
    auto time_e   = std::chrono::system_clock::now();
    auto duration = time_e - time_s;
    auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "Player takes " << msec << " ms." << std::endl;
    if (board_.can_move(input)) {
      board_.move(input);
      add_random_tile_();
    }
    if (on_turn_finished) {
      on_turn_finished(board_);
    }
  }
  if (on_game_terminated) {
    on_game_terminated(board_);
  }
  return std::make_tuple(board_);
}

// private --------------------------------------------------------------------

bool game_master::add_random_tile_() {
  auto blanks = board_.find_blank_tiles();
  if (blanks.size() == 0) {
    return false;
  }

  std::uniform_int_distribution<> tile_selector(0, (int)blanks.size() - 1);
  int blank_tile = blanks.at(tile_selector(engine_));

  std::uniform_real_distribution<> rand(0.0, 1.0);
  board_.add_tile(blank_tile, rand(engine_) < 0.9 ? 1 : 2);

  return true;
}
