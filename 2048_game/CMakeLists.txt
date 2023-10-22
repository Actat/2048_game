cmake_minimum_required(VERSION 3.12)
project(2048_game)

add_library(${PROJECT_NAME}
  ./src/game_board.cpp
  ./src/game_board_eval.cpp
  ./src/game_master.cpp
  ./src/player_game_tree.cpp
  )

target_include_directories(${PROJECT_NAME}
  INTERFACE
  ./include
  )

