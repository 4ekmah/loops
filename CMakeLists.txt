cmake_minimum_required(VERSION 3.6)
project(Loops)

set(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB loops_hdrs  "${CMAKE_CURRENT_SOURCE_DIR}/include/loops/*.hpp")
file(GLOB loops_ihdrs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB loops_srcs  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )

file(GLOB test_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp")
file(GLOB test_srcs "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

add_library(loops STATIC ${loops_hdrs} ${loops_srcs} ${loops_ihdrs})
include_directories(loops "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB examples "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp")
foreach(example_fn ${examples})
    get_filename_component(example_target ${example_fn} NAME_WLE)
	add_executable(${example_target} "${example_fn}")
	target_link_libraries(${example_target} loops)
	set_target_properties(${example_target} PROPERTIES FOLDER Examples)
endforeach()

add_executable(testall ${test_srcs} ${test_hdrs})
if (MSVC)
    set_target_properties(testall PROPERTIES COMPILE_FLAGS /DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
else()
    set_target_properties(testall PROPERTIES COMPILE_FLAGS -DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
endif()
target_link_libraries(testall loops)
