cmake_minimum_required(VERSION 3.16)
project(case1_ringbuffer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Header-only library
add_library(ring_buffer INTERFACE)
target_include_directories(ring_buffer INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Tests
find_package(GTest REQUIRED)
add_executable(test_ring_buffer tests/test_ring_buffer.cpp)
target_link_libraries(test_ring_buffer PRIVATE ring_buffer GTest::GTest GTest::Main)
enable_testing()
add_test(NAME RingBufferTests COMMAND test_ring_buffer)

# Benchmark
add_executable(bench_ring_buffer tests/bench_ring_buffer.cpp)
target_link_libraries(bench_ring_buffer PRIVATE ring_buffer GTest::GTest)
target_compile_definitions(bench_ring_buffer PRIVATE BENCHMARK_MODE)
