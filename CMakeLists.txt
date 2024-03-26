cmake_minimum_required(VERSION 3.21)
project(labwork_4_DaryaPanyukova)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(labwork_4_DaryaPanyukova
        Archive.cpp
        Archive.h
        File.cpp
        File.h
        main.cpp
        Hamming.h
        )
