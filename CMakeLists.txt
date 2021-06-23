cmake_minimum_required(VERSION 3.16)
project(2048)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(2048
        src/main.cpp
        src/Window.cpp
        src/Window.h
        src/App.cpp
        src/App.h
        src/Surface.cpp
        src/Surface.h
        src/Renderer.cpp
        src/Renderer.h
        src/Texture.cpp
        src/Texture.h
        src/Application.cpp
        src/Application.h
        src/Image.cpp
        src/Image.h
        src/Ttf.cpp
        src/Ttf.h
        src/Font.cpp
        src/Font.h
        src/Music.cpp
        src/Music.h)

TARGET_LINK_LIBRARIES(2048
        pthread
        SDL2
        SDL2_ttf
        SDL2_image
        SDL2_mixer)