cmake_minimum_required(VERSION 3.0)
project(tpDatos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -pedantic -Wall -std=c++11")

set(SOURCE_FILES test.cpp random_forest.cpp random_forest.h Coordinate.cpp Coordinate.h C45.cpp C45.h gain.cpp gain.h test_functions.cpp test_functions.h Reader.cpp Reader.h Crime.cpp Crime.h)
add_executable(tpDatos ${SOURCE_FILES})
