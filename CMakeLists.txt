cmake_minimum_required(VERSION 3.6)
project(Loops)

set(CMAKE_CXX_STANDARD 17)

file(GLOB loops_hdrs  "${CMAKE_CURRENT_SOURCE_DIR}/include/loops/*.hpp")
file(GLOB loops_ihdrs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB loops_srcs  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )

file(GLOB test_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp")
file(GLOB test_srcs "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

add_library(loops STATIC ${loops_hdrs} ${loops_srcs} ${loops_ihdrs})
include_directories(loops "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(first examples/first.cpp)
target_link_libraries(first loops)

add_executable(testall ${test_srcs} ${test_hdrs})
if (MSVC)
    set_target_properties(testall PROPERTIES COMPILE_FLAGS /DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
else()
    set_target_properties(testall PROPERTIES COMPILE_FLAGS -DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
endif()
target_link_libraries(testall loops)
