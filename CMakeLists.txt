cmake_minimum_required(VERSION 3.13)
project(caravel)

add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(caravel src/main.cpp src/application.cpp src/utils.cpp)
target_link_libraries(caravel ${CONAN_LIBS})

