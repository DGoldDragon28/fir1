cmake_minimum_required(VERSION 3.1.0)

project(signal-cleaner)

set (CMAKE_CXX_STANDARD 11)


set(SOURCES main.cpp SignalCleaner.cpp ../Fir1.cpp)
add_executable(signal-cleaner ${SOURCES})
target_include_directories(signal-cleaner PRIVATE ..)
