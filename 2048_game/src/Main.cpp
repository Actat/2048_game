#include <Siv3D.hpp>
#include <chrono>
#include <random>
#include "game_board.hpp"
#include "game_board_eval.hpp"
#include "player_console.hpp"
#include "player_game_tree.hpp"
#include "visualizer.hpp"

void Main() {
  Visualizer v;
  int scene_w = s3d::Scene::Width();
  int scene_h = s3d::Scene::Height();

  auto eval = GameBoardEval();

  std::random_device seed_gen;
  std::mt19937 engine_ = std::mt19937(seed_gen());

  GameBoard board_ = GameBoard();

  auto on_game_start = [&v, &scene_w, &scene_h](VisualizerData const &data) {
    v.draw(data, 0, 0, scene_w, scene_h);
  };
  auto on_turn_finished = [&v, &scene_w, &scene_h](VisualizerData const &data) {
    v.draw(data, 0, 0, scene_w, scene_h);
  };
  auto on_game_terminated = [](VisualizerData const &data) {
    std::cout << "Game terminated." << std::endl;
  };
  auto add_random_tile = [&engine_](GameBoard &board_) {
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
  auto p = PlayerGameTree();

  for (int i = 0; i < START_TILES; i++) {
    add_random_tile(board_);
  }
  auto zero_sec     = std::chrono::nanoseconds(0);
  VisualizerData d0 = {board_, zero_sec, 0};
  on_game_start(d0);

  while (!board_.is_terminated() && s3d::System::Update()) {
    auto time_s   = std::chrono::system_clock::now();
    auto input    = p.play(board_);
    auto time_e   = std::chrono::system_clock::now();
    auto duration = time_e - time_s;
    auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "Player takes " << msec << " ms." << std::endl;
    if (board_.can_move(input.direction())) {
      board_.move(input);
      add_random_tile(board_);
    }
    VisualizerData d1 = {board_, duration, eval.evaluate(board_)};
    on_turn_finished(d1);
  }

  VisualizerData d2 = {board_, zero_sec, eval.evaluate(board_)};
  if (on_game_terminated) {
    on_game_terminated(d2);
  }

  while (s3d::System::Update()) {
    v.draw(d2, 0, 0, scene_w, scene_h);
  }
}
