cmake_minimum_required(VERSION 3.5.0)
project(arkanoid VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory("src")

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)
add_dependencies(arkanoid copy_assets)

