cmake_minimum_required(VERSION 3.30)
project(Lab11)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(Lab11
        environment.h
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
