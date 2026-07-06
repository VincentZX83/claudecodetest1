cmake_minimum_required(VERSION 3.16)
project(claude_code_cpp_study VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build test suites" ON)
option(BUILD_BENCHMARKS "Build benchmark targets" OFF)

# Sub-projects
add_subdirectory(cases/case1_ringbuffer)
# add_subdirectory(cases/case2_httpclient)  # Requires Boost.Asio
# add_subdirectory(cases/case3_logger)       # Requires spdlog

message(STATUS "Claude Code C++ Study — Build configured")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Tests: ${BUILD_TESTS}")
