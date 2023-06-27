cmake_minimum_required(VERSION 3.24)
project(pong_game_OpenGL)

set(CMAKE_CXX_STANDARD 17)

add_executable(pong_game_OpenGL main.cpp)

find_package(GLEW REQUIRED)
target_link_libraries(pong_game_OpenGL GLEW::glew)
target_link_libraries(pong_game_OpenGL -lOpenGL32 -lfreeGLUT)