cmake_minimum_required(VERSION 3.1.0)

project(correlate)

set (CMAKE_CXX_STANDARD 11)

set(SOURCES main.cpp correlate.cpp)
add_executable(wav-correlate ${SOURCES})
target_include_directories(wav-correlate PRIVATE ..)