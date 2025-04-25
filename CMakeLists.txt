cmake_minimum_required(VERSION 3.15)
project(OrderBookEngineTests)

# 使用 vcpkg 提供的 toolchain
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  message(FATAL_ERROR "You must pass -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake")
endif()

# 找到 GTest
find_package(GTest CONFIG REQUIRED)

# 添加测试可执行文件
add_executable(test_smoke tests/test_smoke.cpp)

# 链接 GTest
target_link_libraries(test_smoke PRIVATE GTest::gtest_main)

# 注册到 CTest
enable_testing()
add_test(NAME SmokeTest COMMAND test_smoke)
