cmake_minimum_required(VERSION 3.30)
project(MiniProyecto)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(MiniProyecto
        environment.hh
        exp.cpp
        exp.h
        gencode.cpp
        gencode.hh
        imp_interpreter.cpp
        imp_interpreter.hh
        imp_type.cpp
        imp_type.hh
        imp_value.cpp
        imp_value.hh
        imp_value_visitor.hh
        main.cpp
        parser.cpp
        parser.h
        scanner.cpp
        scanner.h
        token.cpp
        token.h
        type_visitor.hh
        visitor.cpp
        visitor.h)
