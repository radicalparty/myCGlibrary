cmake_minimum_required(VERSION 3.22)
project(CGimplement)

set(CMAKE_CXX_STANDARD 20)

add_executable(CGimplement main.cpp
        polygon.cpp
        CircularList.cpp
)
