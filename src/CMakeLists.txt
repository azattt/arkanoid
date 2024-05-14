cmake_minimum_required(VERSION 3.5.0)
project(arkanoid VERSION 0.1.0 LANGUAGES C CXX)

# add_executable(arkanoid main.cpp draw_utilities.cpp)
add_executable(arkanoid main.cpp ball.cpp vars.hpp)
target_link_libraries(arkanoid -lfreeglut)
target_link_libraries(arkanoid -lopengl32)