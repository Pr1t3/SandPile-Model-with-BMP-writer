cmake_minimum_required(VERSION 3.16)
project(labwork3_Pr1t3)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(labwork3_Pr1t3
        ArgumentsParser.cpp
        ArgumentsParser.hpp
        main.cpp
        Structures.hpp
        SandPileLogic.cpp
        SandPileLogic.h)
