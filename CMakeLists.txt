cmake_minimum_required(VERSION 3.6)
project(Loops)

set(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
    set(CMP_OPTIONS PRIVATE /W4 /WX)
else()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        set(CMP_OPTIONS PRIVATE -Wall -Wextra -Wpedantic -Wno-gnu-zero-variadic-macro-arguments -Wunused-value -Wshorten-64-to-32 -Wdeprecated-declarations -Winvalid-offsetof -Werror)
    else()
        set(CMP_OPTIONS PRIVATE -Wall -Wextra -Wpedantic -Wunused-value -Wdeprecated-declarations -Winvalid-offsetof -Werror)
    endif()
endif()
if (MSVC)
    set(TESTFOLDER_PROPERTIES PROPERTIES COMPILE_FLAGS /DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
else()
    set(TESTFOLDER_PROPERTIES PROPERTIES COMPILE_FLAGS -DLOOPS_TEST_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/test\\")
endif()

#LOOPS LIBRARY
file(GLOB loops_hdrs  "${CMAKE_CURRENT_SOURCE_DIR}/include/loops/*.hpp")
file(GLOB loops_himp  "${CMAKE_CURRENT_SOURCE_DIR}/include/loops/*.ipp")
file(GLOB loops_ihdrs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB loops_srcs  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
add_library(loops STATIC ${loops_hdrs} ${loops_himp} ${loops_srcs} ${loops_ihdrs})
include_directories(loops "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(loops "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/uthash/src/")
target_compile_options(loops ${CMP_OPTIONS})

#LOOPSLAYERS LIBRARY
file(GLOB loopslayers_hdrs  "${CMAKE_CURRENT_SOURCE_DIR}/include/loopslayers/*.h")
file(GLOB loopslayers_ihdrs "${CMAKE_CURRENT_SOURCE_DIR}/loopslayers/generators/*.hpp")
file(GLOB loopslayers_srcs  "${CMAKE_CURRENT_SOURCE_DIR}/loopslayers/*.cpp" )
add_library(loopslayers STATIC ${loops_hdrs} ${loops_himp} ${loopslayers_hdrs} ${loopslayers_srcs} ${loopslayers_ihdrs} ${loopslayers_hdrs})
include_directories(loopslayers "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(loopslayers loops)
target_compile_options(loopslayers ${CMP_OPTIONS})

#TESTREGRESS
file(GLOB testregress_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/test/testregress/*.hpp")
add_executable(testregress ${testregress_hdrs} 
              "${CMAKE_CURRENT_SOURCE_DIR}/test/tests.cpp" 
              "${CMAKE_CURRENT_SOURCE_DIR}/test/tests.hpp"
              "${CMAKE_CURRENT_SOURCE_DIR}/test/testregress.cpp")
include_directories(testregress "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(testregress "${CMAKE_CURRENT_SOURCE_DIR}/test")
include_directories(testregress "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest/googletest/include")
include_directories(testregress "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/miniz")
if (MSVC)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
add_subdirectory(thirdparty/googletest EXCLUDE_FROM_ALL)
target_link_libraries(testregress loops gtest)
target_compile_options(testregress ${CMP_OPTIONS})
set_target_properties(testregress ${TESTFOLDER_PROPERTIES})

#TESTPERF
file(GLOB testperf_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/test/testperf/*.hpp")
file(GLOB testperf_srcs "${CMAKE_CURRENT_SOURCE_DIR}/test/testperf/*.cpp")
add_executable(testperf ${testperf_hdrs} ${testperf_srcs} ${loopslayers_hdrs}
              "${CMAKE_CURRENT_SOURCE_DIR}/test/tests.cpp" 
              "${CMAKE_CURRENT_SOURCE_DIR}/test/tests.hpp"
              "${CMAKE_CURRENT_SOURCE_DIR}/test/testperf.cpp")
include_directories(testperf "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(testperf "${CMAKE_CURRENT_SOURCE_DIR}/test")
target_link_libraries(testperf loops loopslayers)
if ((CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64") AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(testperf PRIVATE -march=armv8.2-a+fp16+dotprod)
    target_link_libraries(testperf pthread)
endif()
target_compile_options(testperf ${CMP_OPTIONS})
set_target_properties(testperf ${TESTFOLDER_PROPERTIES})

#EXAMPLES 
file(GLOB examples "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp")
foreach(example_fn ${examples})
    get_filename_component(example_target ${example_fn} NAME_WLE)
	add_executable(${example_target} "${example_fn}")
	target_link_libraries(${example_target} loops)
	set_target_properties(${example_target} PROPERTIES FOLDER Examples)
    target_compile_options(testperf ${CMP_OPTIONS})
endforeach()