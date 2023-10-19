cmake_minimum_required(VERSION 3.12)
project(2048_game_visualizer CXX)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_subdirectory(2048_game)
add_subdirectory(OpenSiv3D/Linux)

add_executable(2048_game.out 2048_game/src/Main.cpp)

target_include_directories(2048_game.out PRIVATE
./OpenSiv3D/Siv3D/include
./OpenSiv3D/Siv3D/include/Siv3D
./OpenSiv3D/Siv3D/include/ThirdParty
)
target_link_libraries(2048_game.out PUBLIC
  2048_game
  Siv3D::Siv3D
)
target_compile_features(2048_game.out PRIVATE cxx_std_20)

