# 2048_game

2048 Game automatically played

# Usage

This repository is developed on Ubuntu.

```
sudo apt update
sudo apt install -y git cmake g++
sudo apt install -y ninja-build libasound2-dev libavcodec-dev libavformat-dev libavutil-dev libboost-dev libcurl4-openssl-dev libgtk-3-dev libgif-dev libglu1-mesa-dev libharfbuzz-dev libmpg123-dev libopencv-dev libopus-dev libopusfile-dev libsoundtouch-dev libswresample-dev libtiff-dev libturbojpeg0-dev libvorbis-dev libwebp-dev libxft-dev uuid-dev xorg-dev # dependency packages of OpenSiv3D
git clone --recursive https://github.com/Actat/2048_game.git
mkdir 2048_game/build
cd  2048_game/build
cmake ..
cmake --build .
../2048_game.out
```
