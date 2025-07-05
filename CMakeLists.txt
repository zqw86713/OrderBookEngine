cmake_minimum_required(VERSION 3.15)
project(OrderBookEngine)

# vcpkg toolchain check
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  message(FATAL_ERROR "You must pass -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake")
endif()

# Dependencies
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

find_package(GTest CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core library
add_library(orderbook_core
    Sources/MatchingEngine.cpp
    Sources/OrderBook.cpp
)
target_include_directories(orderbook_core PUBLIC Headers)

# Main executable
add_executable(OrderBookEngine Sources/main.cpp)
target_link_libraries(OrderBookEngine PRIVATE orderbook_core)

# Benchmark v1
add_executable(benchmark_orders_v1 benchmarks/benchmark_orders.cpp)
target_link_libraries(benchmark_orders_v1 PRIVATE orderbook_core)

# Benchmark v2
add_executable(benchmark_orders_v2 benchmarks/benchmark_orders_v2.cpp)
target_link_libraries(benchmark_orders_v2 PRIVATE orderbook_core)

# Benchmark v3
add_executable(benchmark_orders_v3 benchmarks/benchmark_orders_v3.cpp)
target_link_libraries(benchmark_orders_v3 PRIVATE orderbook_core)

# Tests
add_executable(OrderBookEngine_tests
    Tests/my_tests.cpp
    Tests/test_order_book.cpp
    Tests/test_smoke.cpp
)
target_link_libraries(OrderBookEngine_tests PRIVATE GTest::gtest_main orderbook_core)

enable_testing()
add_test(NAME AllTests COMMAND OrderBookEngine_tests)
