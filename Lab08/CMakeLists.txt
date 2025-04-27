cmake_minimum_required(VERSION 3.30)
project(Lab08)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab08 main.cpp
        exp.cpp
        parser.cpp
        scanner.cpp
        token.cpp
        visitor.cpp)
