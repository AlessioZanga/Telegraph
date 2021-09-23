cmake_minimum_required(VERSION 3.1)

project(MyProject
    VERSION 0.0.1
    DESCRIPTION "Telegraph"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_compile_options(-Wall -Wextra -pedantic)
add_compile_options(-O3 -march=native -shared -std=c++17 -fPIC)

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

include_directories(extern/eigen)

enable_testing()
include(GoogleTest)
add_subdirectory(tests)
