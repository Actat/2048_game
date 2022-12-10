#include <Siv3D.hpp>
#include <random>
#include "../game_board.hpp"
#include "../player_console.hpp"
#include "../player_minimax.hpp"
#include "visualizer.hpp"

void Main() {
  visualizer v;
  int scene_w = s3d::Scene::Width();
  int scene_h = s3d::Scene::Height();

  std::random_device seed_gen;
  std::mt19937 engine_ = std::mt19937(seed_gen());

  game_board board_ = game_board();

  auto on_game_start = [&v, &scene_w, &scene_h](game_board const &b) {
    v.draw(b, 0, 0, scene_w, scene_h);
  };
  auto on_turn_finished = [&v, &scene_w, &scene_h](game_board const &b) {
    v.draw(b, 0, 0, scene_w, scene_h);
  };
  auto on_game_terminated = [](game_board const &b) {
    std::cout << "Game terminated." << std::endl;
  };
  auto add_random_tile = [&engine_](game_board &board_) {
    auto blanks = board_.find_blank_tiles();
    if (blanks.size() == 0) {
      return false;
    }

    std::uniform_int_distribution<> tile_selector(0, (int)blanks.size() - 1);
    int blank_tile = blanks.at(tile_selector(engine_));

    std::uniform_real_distribution<> rand(0.0, 1.0);
    board_.add_tile(blank_tile, rand(engine_) < 0.9 ? 1 : 2);

    return true;
  };

  // auto p = player_console();
  auto p = player_minimax();

  for (int i = 0; i < START_TILES; i++) {
    add_random_tile(board_);
  }
  on_game_start(board_);

  while (!board_.is_terminated() && s3d::System::Update()) {
    auto time_s   = std::chrono::system_clock::now();
    int input     = p.play(board_);
    auto time_e   = std::chrono::system_clock::now();
    auto duration = time_e - time_s;
    auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "Player takes " << msec << " ms." << std::endl;
    if (board_.can_move(input)) {
      board_.move(input);
      add_random_tile(board_);
    }
    on_turn_finished(board_);
  }
  if (on_game_terminated) {
    on_game_terminated(board_);
  }

  while (s3d::System::Update()) {
    v.draw(board_, 0, 0, scene_w, scene_h);
  }
}
