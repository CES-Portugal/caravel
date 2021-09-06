cmake_minimum_required(VERSION 3.13)
project(caravel)

add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")

conan_basic_setup()

add_executable(caravel src/main.cpp src/application.cpp src/utils.cpp src/signal.cpp 
                       src/actors/actors.cpp src/actors/receive_msg_act.cpp src/actors/output_msg_act.cpp
                       src/actors/send_msg_act.cpp src/actors/log_msg_act.cpp src/actors/parse_input_act.cpp 
                       src/actors/parse_simulation_act.cpp src/actors/exit_act.cpp)
target_link_libraries(caravel ${CONAN_LIBS})

