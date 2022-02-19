cmake_minimum_required(VERSION 3.6)
project(Loops)

set(CMAKE_CXX_STANDARD 17)

file(GLOB loops_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/include/loops/*.hpp")
#file(GLOB loops_ihdrs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB loops_srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(loops STATIC ${loops_hdrs} ${loops_srcs})
include_directories(loops "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(first examples/first.cpp)
target_link_libraries(first loops)
