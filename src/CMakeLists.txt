cmake_minimum_required(VERSION 3.5.0)
project(arkanoid VERSION 0.1.0 LANGUAGES C CXX)

# add_executable(arkanoid main.cpp draw_utilities.cpp)
add_executable(arkanoid main.cpp ball.cpp vars.hpp draw_utilities.cpp gui.hpp)


if (WIN32)
    target_link_libraries(arkanoid -lfreeglut -lopengl32 -lfreetype)
    target_include_directories(arkanoid PRIVATE "../deps" PRIVATE "D:/programs/msys/mingw64/include/freetype2")
endif (WIN32)

if(UNIX AND NOT APPLE)
    target_link_libraries(arkanoid -lglut -lGL -lfreetype)
    target_include_directories(arkanoid PRIVATE "../deps" PRIVATE "/usr/include/freetype2")
endif (UNIX AND NOT APPLE)