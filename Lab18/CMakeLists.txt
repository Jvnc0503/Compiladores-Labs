cmake_minimum_required(VERSION 3.30)
project(Lab18)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(Lab18
        exp.cpp
        exp.h
        main.cpp
        parser.cpp
        parser.h
        scanner.cpp
        scanner.h
        token.cpp
        token.h
        visitor.cpp
        visitor.h)
