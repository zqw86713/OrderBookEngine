cmake_minimum_required(VERSION 3.15)
project(OrderBookEngine)

# vcpkg toolchain check
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  message(FATAL_ERROR "You must pass -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake")
endif()

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

find_package(GTest CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Main Application Executable
add_executable(OrderBookEngine
    Sources/main.cpp
    Sources/MatchingEngine.cpp
    Sources/OrderBook.cpp
)

# Test Executable (GoogleTest)
add_executable(OrderBookEngine_tests
    Tests/my_tests.cpp
    Tests/test_order_book.cpp
    Tests/test_smoke.cpp
)

target_link_libraries(OrderBookEngine_tests PRIVATE GTest::gtest_main)

enable_testing()
add_test(NAME AllTests COMMAND OrderBookEngine_tests)
