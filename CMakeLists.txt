cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/graphics.cpp src/generator.cpp src/point.cpp src/read_input.cpp src/buffer_error.cpp)
add_executable(game ${SOURCE_FILES})