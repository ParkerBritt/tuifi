#!/usr/bin/env bash
set -e

# rm -rf ./build
mkdir ./build  -p
cd build 
cmake .. 
make -j
cd .. 
./build/bin/tui_app_launcher
