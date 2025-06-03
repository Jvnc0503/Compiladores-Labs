cmake_minimum_required(VERSION 3.30)
project(Exam2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Exam2 main.cpp
        scanner.h
        scanner.cpp
        token.h
        token.cpp
        parser.h
        parser.cpp)