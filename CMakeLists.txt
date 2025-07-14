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
  URL https://github.com/google/googletest/archive/refs/heads/main.zip)
FetchContent_MakeAvailable(googletest)

find_package(GTest CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core library
add_library(orderbook_core
    Sources/OrderBook.cpp
    Sources/MatchingEngine.cpp
)
target_include_directories(orderbook_core PUBLIC Headers)

# Main executable
add_executable(OrderBookEngine
    Sources/main.cpp
)
target_link_libraries(OrderBookEngine PRIVATE orderbook_core)

# Benchmarks
foreach(bm
    benchmark_orders.cpp
    benchmark_orders_v2.cpp
    benchmark_orders_v3.cpp
    benchmark_orders_mt.cpp
    benchmark_orders_mt_v2.cpp
    benchmark_latency_mt.cpp
)
    get_filename_component(name ${bm} NAME_WE)
    add_executable(${name} benchmarks/${bm})
    target_link_libraries(${name} PRIVATE orderbook_core)
    target_include_directories(${name} PRIVATE Headers)
endforeach()

# Tests
enable_testing()
add_executable(OrderBookEngine_tests
    Tests/orderbook_market_test.cpp
    Tests/orderbook_limit_test.cpp
)
target_link_libraries(OrderBookEngine_tests
    PRIVATE
      orderbook_core
      GTest::gtest_main
)
add_test(NAME AllTests COMMAND OrderBookEngine_tests)
