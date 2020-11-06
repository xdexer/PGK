cmake_minimum_required(VERSION 3.16)
project(lista3deb)
set(CMAKE_CXX_STANDARD 14)
set(OpenGlLinkers -lepoxy -lGL -lglfw)
include_directories(.)

add_executable(lista3deb
        AGL3Drawable.hpp
        AGL3Window.cpp
        AGL3Window.hpp
        zadanie3.cpp MyLine.cpp MyLine.h Linetab.cpp Linetab.h)
target_link_libraries(lista3deb ${OpenGlLinkers})
