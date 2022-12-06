#include <Siv3D.hpp>
#include "../game_board.hpp"
#include "../player_minimax.hpp"
#include "visualizer.hpp"

void Main() {
  game_board b;
  visualizer v;
  auto p = player_minimax();

  int scene_w    = Scene::Width();
  int scene_h    = Scene::Height();
  int scene_size = scene_h < scene_w ? scene_h : scene_w;

  v.draw_board(b, scene_size, (scene_w - scene_size) / 2,
               (scene_h - scene_size) / 2);

  while (System::Update() && !b.is_terminated()) {
    int input = p.play(b);
    b.input(input);
    v.draw_board(b, scene_size, (scene_w - scene_size) / 2,
                 (scene_h - scene_size) / 2);
  }

  while (System::Update()) {
    v.draw_board(b, scene_size, (scene_w - scene_size) / 2,
                 (scene_h - scene_size) / 2);
  }
}
