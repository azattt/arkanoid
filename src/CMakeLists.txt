cmake_minimum_required(VERSION 3.5.0)
project(arkanoid VERSION 0.1.0 LANGUAGES C CXX)

add_executable(arkanoid main.cpp draw_utilities.hpp draw_utilities.cpp game_structs.hpp)
target_link_libraries(arkanoid -lfreeglut)
target_link_libraries(arkanoid -lopengl32)