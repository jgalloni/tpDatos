cmake_minimum_required(VERSION 3.0)
project(tpDatos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test.cpp Feature.h Feature.cpp Coordinate.cpp Coordinate.h C45.cpp C45.h Reader.cpp Reader.h Crime.cpp Crime.h)
add_executable(tpDatos ${SOURCE_FILES})
