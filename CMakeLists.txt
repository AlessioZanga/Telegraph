cmake_minimum_required(VERSION 3.8)

project(Telegraph
    VERSION 0.0.1
    DESCRIPTION "Telegraph"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -march=native -shared -fPIC)
endif()

include_directories(include)
add_subdirectory(src)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# Find Boost library.
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Add Eigen library.
include_directories(extern/eigen)

option (TELEGRAPH_ENABLE_TESTS "Build tests cases.")
option (TELEGRAPH_ENABLE_BENCHMARKS "Build benchmarks.")

if (TELEGRAPH_ENABLE_TESTS OR TELEGRAPH_ENABLE_BENCHMARKS)
    # Add GoogleTest library for tests and benchmarks.
    include(GoogleTest)
    add_subdirectory(extern/googletest)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

    # Add GoogleTest to make clean support.
    mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
    )

    set_target_properties(gtest PROPERTIES FOLDER extern)
    set_target_properties(gtest_main PROPERTIES FOLDER extern)
    set_target_properties(gmock PROPERTIES FOLDER extern)
    set_target_properties(gmock_main PROPERTIES FOLDER extern)
endif()

if (TELEGRAPH_ENABLE_TESTS AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    enable_testing()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
    add_subdirectory(tests)
endif()

if (TELEGRAPH_ENABLE_BENCHMARKS AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    add_subdirectory(extern/googlebenchmark)
    add_subdirectory(benchmarks)
endif()
