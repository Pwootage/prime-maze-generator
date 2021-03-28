cmake_minimum_required(VERSION 3.17)
project(prime_maze_checker)

set(CMAKE_CXX_STANDARD 17)

add_executable(prime_maze_checker
        src/main.cpp
        src/GCNTypes.hpp
        src/CRandom16.hpp
        src/CRandom16.cpp
        src/CScriptMazeNode.hpp
        src/CScriptMazeNode.cpp
        src/seeds.h
        )