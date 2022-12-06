#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <Siv3D.hpp>
#include "../game_board.hpp"

class visualizer {
private:
  Color COLOR_BG{187, 173, 160};
  Color COLOR_0{204, 192, 179};
  Color COLOR_1{238, 228, 218};
  Color COLOR_2{237, 224, 200};
  Color COLOR_3{242, 177, 121};
  Color COLOR_4{245, 149, 99};
  Color COLOR_5{246, 124, 95};
  Color COLOR_6{246, 94, 59};
  Color COLOR_7{237, 207, 114};
  Color COLOR_8{237, 204, 97};
  Color COLOR_9{237, 200, 80};
  Color COLOR_10{237, 197, 63};
  Color COLOR_11{237, 194, 46};
  Color COLOR_OTHER{0, 0, 0};
  Color COLOR_VALUE_S{119, 110, 101};  // <= 4 = 2^2
  Color COLOR_VALUE_L{249, 246, 242};  // >= 8 = 2^3
  Font FONT_S{45, Typeface::Bold};     // <= 64 = 2^6
  Font FONT_M{35, Typeface::Bold};     // 128, 256, 512
  Font FONT_L{25, Typeface::Bold};     // >= 1024 = 2^10

public:
  visualizer();
  void draw(game_board b, int pos_x, int pos_y, int size_x, int size_y);
  void draw_board(game_board b, int pos_x, int pos_y, int size);
  void draw_info(game_board b, int pos_x, int pos_y, int size_x, int size_y);
};

visualizer::visualizer() {}

void visualizer::draw(game_board b,
                      int pos_x,
                      int pos_y,
                      int size_x,
                      int size_y) {
  Rect{pos_x, pos_y, size_x, size_y}.draw(COLOR_2);
  if (size_x > size_y) {
    int pading_size = size_x * 0.05;
    int board_size  = size_y - 2 * pading_size;
    draw_board(b, pos_x + pading_size, pos_y + (size_y - board_size) / 2,
               board_size);
    draw_info(b, pos_x + 2 * pading_size + board_size, pos_y + pading_size,
              size_x - 3 * pading_size - board_size, size_y - 2 * pading_size);
  }
}

void visualizer::draw_board(game_board b, int pos_x, int pos_y, int size) {
  Rect{pos_x, pos_y, size, size}.draw(COLOR_BG);
  int border_width = size * 5 / 100 / (BOARD_SIZE);
  int tile_size    = (size - border_width * (BOARD_SIZE + 1)) / BOARD_SIZE;

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      int rect_pos_x = pos_x + border_width + (tile_size + border_width) * j;
      int rect_pos_y = pos_y + border_width + (tile_size + border_width) * i;
      Rect r{rect_pos_x, rect_pos_y, tile_size, tile_size};
      Vec2 pos{rect_pos_x + tile_size / 2, rect_pos_y + tile_size / 2};
      int value = b.get_board().at(BOARD_SIZE * i + j);
      String text{Unicode::Widen(std::to_string((int)std::pow(2, value)))};
      switch (value) {
        case 0:
          r.draw(COLOR_0);
          break;

        case 1:
          r.draw(COLOR_1);
          FONT_S(text).drawAt(pos, COLOR_VALUE_S);
          break;

        case 2:
          r.draw(COLOR_2);
          FONT_S(text).drawAt(pos, COLOR_VALUE_S);
          break;

        case 3:
          r.draw(COLOR_3);
          FONT_S(text).drawAt(pos, COLOR_VALUE_L);
          break;

        case 4:
          r.draw(COLOR_4);
          FONT_S(text).drawAt(pos, COLOR_VALUE_L);
          break;

        case 5:
          r.draw(COLOR_5);
          FONT_S(text).drawAt(pos, COLOR_VALUE_L);
          break;

        case 6:
          r.draw(COLOR_6);
          FONT_S(text).drawAt(pos, COLOR_VALUE_L);
          break;

        case 7:
          r.draw(COLOR_7);
          FONT_M(text).drawAt(pos, COLOR_VALUE_L);
          break;

        case 8:
          r.draw(COLOR_8);
          FONT_M(text).drawAt(pos, COLOR_VALUE_L);
          break;

        case 9:
          r.draw(COLOR_9);
          FONT_M(text).drawAt(pos, COLOR_VALUE_L);
          break;

        case 10:
          r.draw(COLOR_10);
          FONT_L(text).drawAt(pos, COLOR_VALUE_L);
          break;

        case 11:
          r.draw(COLOR_11);
          FONT_L(text).drawAt(pos, COLOR_VALUE_L);
          break;

        default:
          r.draw(COLOR_OTHER);
          FONT_L(text).drawAt(pos, COLOR_VALUE_L);
          break;
      }
    }
  }
};

void visualizer::draw_info(game_board b,
                           int pos_x,
                           int pos_y,
                           int size_x,
                           int size_y) {
  int turn_pos_x, turn_pos_y, turn_size_x, turn_size_y;
  int score_pos_x, score_pos_y, score_size_x, score_size_y;
  Rect turn;
  Rect score;
  if (size_x < size_y) {
    turn_pos_x   = pos_x;
    turn_pos_y   = pos_y;
    turn_size_x  = size_x;
    turn_size_y  = 60;
    score_pos_x  = pos_x;
    score_pos_y  = turn_pos_y + turn_size_y + size_y * 5 / 100;
    score_size_x = size_x;
    score_size_y = 60;
    turn         = Rect{turn_pos_x, turn_pos_y, turn_size_x, turn_size_y};
    score        = Rect{score_pos_x, score_pos_y, score_size_x, score_size_y};
  }
  turn.draw(COLOR_BG);
  score.draw(COLOR_BG);
  FONT_L(U"Turn").drawAt(turn_pos_x + turn_size_x / 2,
                         turn_pos_y + turn_size_y / 4, COLOR_VALUE_L);
  FONT_L(String{Unicode::Widen(std::to_string(b.get_turn()))})
      .drawAt(turn_pos_x + turn_size_x / 2, turn_pos_y + turn_size_y * 3 / 4,
              COLOR_VALUE_L);
  FONT_L(U"Score").drawAt(score_pos_x + score_size_x / 2,
                          score_pos_y + score_size_y / 4, COLOR_VALUE_L);
  FONT_L(String{Unicode::Widen(std::to_string(b.get_score()))})
      .drawAt(score_pos_x + score_size_x / 2,
              score_pos_y + score_size_y * 3 / 4, COLOR_VALUE_L);
}
#endif  // VISUALIZER_HPP
