set(BENCHMARKS_SOURCES telegraph/containers.cpp)
add_executable(benchmarks ${BENCHMARKS_SOURCES})
target_link_libraries(benchmarks benchmark)
