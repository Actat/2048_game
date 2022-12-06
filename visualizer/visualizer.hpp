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
  void draw_board(game_board b, int size, int pos_x, int pos_y);
};

visualizer::visualizer() {}

void visualizer::draw_board(game_board b, int size, int pos_x, int pos_y) {
  Rect{pos_x, pos_y, size, size}.draw();
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      Rect r{pos_x + size * j / BOARD_SIZE, pos_y + size * i / BOARD_SIZE,
             size / BOARD_SIZE, size / BOARD_SIZE};
      Vec2 pos{pos_x + size * j / BOARD_SIZE + size / 2 / BOARD_SIZE,
               pos_y + size * i / BOARD_SIZE + size / 2 / BOARD_SIZE};
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
      r.drawFrame(3, 3, COLOR_BG);
    }
  }
};

#endif  // VISUALIZER_HPP
