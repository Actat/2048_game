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

  v.draw(b, 0, 0, scene_w, scene_h);

  while (System::Update() && !b.is_terminated()) {
    int input = p.play(b);
    b.input(input);
    v.draw(b, 0, 0, scene_w, scene_h);
  }

  while (System::Update()) {
    v.draw(b, 0, 0, scene_w, scene_h);
  }
}
