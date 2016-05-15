cmake_minimum_required(VERSION 3.5)
project(Graphs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    graph.cpp
    graph.hpp
    main.cpp
    tools.cpp
    tools.hpp random.hpp random.cpp)

add_executable(Graphs ${SOURCE_FILES})