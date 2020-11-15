cmake_minimum_required(VERSION 3.17)
project(Lista4)
set(CMAKE_CXX_STANDARD 17)
set(OpenGlLinkers -lepoxy -lGL -lglfw)
include_directories(.)

add_executable(Lista4
        AGL3Drawable.hpp
        AGL3Window.cpp
        AGL3Window.hpp
        MyLine.cpp
        MyLine.h
        Linetab.cpp
        Linetab.h
        main.cpp Camera.h)
target_link_libraries(Lista4 ${OpenGlLinkers})