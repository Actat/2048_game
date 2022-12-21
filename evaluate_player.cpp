#include <algorithm>
#include <iostream>
#include "game_board.hpp"
#include "game_master.hpp"
#include "player_game_tree.hpp"

#define NUM_TRIAL 100

int main(int argc, char *argv[]) {
  std::array<game_board, NUM_TRIAL> result;

  for (int i = 0; i < NUM_TRIAL; i++) {
    auto m = game_master();
    // auto p = player_random();
    auto p = player_game_tree();

    game_board board;
    std::tie(board) = m.game(p);

    std::cout << "trial: " << i << "/" << NUM_TRIAL << "("
              << 100.0 * (i + 1) / NUM_TRIAL << " %)" << std::endl;
    std::cout << board << std::endl;

    result.at(i) = board;
  }

  double average_score   = 0;
  double average_largest = 0;
  std::array<int, NUM_TRIAL> score;
  std::array<int, NUM_TRIAL> largest_tile;
  for (int i = 0; i < NUM_TRIAL; i++) {
    score.at(i)        = result.at(i).get_score();
    largest_tile.at(i) = result.at(i).get_largest_tile();
    average_score += 1.0 * result.at(i).get_score() / NUM_TRIAL;
    average_largest += 1.0 * result.at(i).get_largest_tile() / NUM_TRIAL;
  }

  std::cout << "Summary of " << NUM_TRIAL << " trials." << std::endl;
  std::cout << "largest tile" << std::endl
            << "\tbest:\t"
            << *std::max_element(largest_tile.begin(), largest_tile.end())
            << "\t("
            << std::pow(2, *std::max_element(largest_tile.begin(),
                                             largest_tile.end()))
            << ")" << std::endl
            << "\tworst:\t"
            << *std::min_element(largest_tile.begin(), largest_tile.end())
            << "\t("
            << std::pow(2, *std::min_element(largest_tile.begin(),
                                             largest_tile.end()))
            << ")" << std::endl
            << "\tave.:\t" << average_largest << "\t("
            << std::pow(2, average_largest) << ")" << std::endl;
  std::cout << "score" << std::endl
            << "\tbest:\t" << *std::max_element(score.begin(), score.end())
            << std::endl
            << "\tworst:\t" << *std::min_element(score.begin(), score.end())
            << std::endl
            << "\tave.:\t" << average_score << std::endl;

  return 0;
}