cmake_minimum_required(VERSION 3.16)
project(labwork3_Pr1t3)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(labwork3_Pr1t3
        ArgumentsParser.cpp
        main.cpp
        SandPileLogic.cpp
        BMPFilerMaker.cpp)