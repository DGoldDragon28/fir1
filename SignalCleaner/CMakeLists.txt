cmake_minimum_required(VERSION 3.1.0)

project(signal-cleaner)

set (CMAKE_CXX_STANDARD 11)
find_package(Boost REQUIRED)
message("Boost found: ${Boost_FOUND}")

set(SOURCES main.cpp SignalCleaner.cpp ../Fir1.cpp ../deepNeuronalFilter/dnf.h)
add_executable(signal-cleaner ${SOURCES})
target_link_libraries(signal-cleaner iir fir)

target_include_directories(signal-cleaner PRIVATE ..)
