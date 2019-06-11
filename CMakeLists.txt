cmake_minimum_required(VERSION 3.6)
project(lab13)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/graphics.cpp src/generator.cpp src/point.cpp)
add_executable(game ${SOURCE_FILES})