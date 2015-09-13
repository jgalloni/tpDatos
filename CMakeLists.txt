cmake_minimum_required(VERSION 3.3)
project(tpDatos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Reader.cpp Reader.h Crimen.cpp Crimen.h)
add_executable(tpDatos ${SOURCE_FILES})