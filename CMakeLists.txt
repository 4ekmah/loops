cmake_minimum_required(VERSION 3.6)
project(Loops)

set(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB loops_hdrs  "${CMAKE_CURRENT_SOURCE_DIR}/include/loops/*.hpp")
file(GLOB loops_ihdrs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB loops_srcs  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )

file(GLOB testregress_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/test/testregress/*.hpp")
file(GLOB testperf_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/test/testperf/*.hpp")
file(GLOB testperf_srcs "${CMAKE_CURRENT_SOURCE_DIR}/test/testperf/*.cpp")

add_library(loops STATIC ${loops_hdrs} ${loops_srcs} ${loops_ihdrs})
include_directories(loops "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB examples "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp")
foreach(example_fn ${examples})
    get_filename_component(example_target ${example_fn} NAME_WLE)
	add_executable(${example_target} "${example_fn}")
	target_link_libraries(${example_target} loops)
	set_target_properties(${example_target} PROPERTIES FOLDER Examples)
endforeach()

add_executable(testregress ${testregress_hdrs} 
              "${CMAKE_CURRENT_SOURCE_DIR}/test/tests.cpp" 
              "${CMAKE_CURRENT_SOURCE_DIR}/test/tests.hpp"
              "${CMAKE_CURRENT_SOURCE_DIR}/test/testregress.cpp")
add_executable(testperf ${testperf_hdrs} ${testperf_srcs} 
              "${CMAKE_CURRENT_SOURCE_DIR}/test/tests.cpp" 
              "${CMAKE_CURRENT_SOURCE_DIR}/test/tests.hpp"
              "${CMAKE_CURRENT_SOURCE_DIR}/test/testperf.cpp")              
if (MSVC)
    set_target_properties(testregress PROPERTIES COMPILE_FLAGS /DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
    set_target_properties(testperf PROPERTIES COMPILE_FLAGS /DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
else()
    set_target_properties(testregress PROPERTIES COMPILE_FLAGS -DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
    set_target_properties(testperf PROPERTIES COMPILE_FLAGS -DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
endif()
target_link_libraries(testregress loops)
target_link_libraries(testperf loops)
