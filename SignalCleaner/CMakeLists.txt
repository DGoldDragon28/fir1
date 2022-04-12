cmake_minimum_required(VERSION 3.1.0)

project(signal-cleaner)

set (CMAKE_CXX_STANDARD 11)
find_package(Boost REQUIRED)
message("Boost found: ${Boost_FOUND}")

include_directories(../deepNeuronalFilter)
link_directories(../deepNeuronalFilter)

set(SOURCES main.cpp SignalCleaner.cpp ../Fir1.cpp )
add_executable(signal-cleaner ${SOURCES} )
target_link_libraries(signal-cleaner dnf)

target_include_directories(signal-cleaner PRIVATE ..)
