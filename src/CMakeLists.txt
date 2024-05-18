cmake_minimum_required(VERSION 3.5.0)
project(arkanoid VERSION 0.1.0 LANGUAGES C CXX)

# add_executable(arkanoid main.cpp draw_utilities.cpp)
add_executable(arkanoid main.cpp ball.cpp vars.hpp draw_utilities.cpp)
if (WIN32)
    target_link_libraries(arkanoid -lfreeglut)
    target_link_libraries(arkanoid -lopengl32)
endif (WIN32)

if(UNIX AND NOT APPLE)
    target_link_libraries(arkanoid -lglut)
    target_link_libraries(arkanoid -lGL)
endif (UNIX AND NOT APPLE)