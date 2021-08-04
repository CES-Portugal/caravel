cmake_minimum_required(VERSION 3.13)
project(caravel)

set(CMAKE_CXX_STANDARD 14)
set(liblist boost_program_options)

add_executable(caravel src/main.cpp)
target_link_libraries(caravel boost_program_options)
