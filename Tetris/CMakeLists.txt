cmake_minimum_required(VERSION 3.13)
project(tetris)

set(CMAKE_CXX_STANDARD 14)


add_subdirectory(glew-2.1.0)
add_subdirectory(soil)

add_executable(tetris WIN32
        Game.cpp
        GLWindow.cpp
        Graphics.cpp
        Input.cpp
        loadTexture.cpp
        main.cpp
        pieces.cpp)

target_link_libraries(tetris glew soil opengl32)