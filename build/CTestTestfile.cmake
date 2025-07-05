# CMake generated Testfile for 
# Source directory: D:/MyProjects/OrderBookEngine
# Build directory: D:/MyProjects/OrderBookEngine/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(AllTests "D:/MyProjects/OrderBookEngine/build/Debug/OrderBookEngine_tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/MyProjects/OrderBookEngine/CMakeLists.txt;54;add_test;D:/MyProjects/OrderBookEngine/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(AllTests "D:/MyProjects/OrderBookEngine/build/Release/OrderBookEngine_tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/MyProjects/OrderBookEngine/CMakeLists.txt;54;add_test;D:/MyProjects/OrderBookEngine/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(AllTests "D:/MyProjects/OrderBookEngine/build/MinSizeRel/OrderBookEngine_tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/MyProjects/OrderBookEngine/CMakeLists.txt;54;add_test;D:/MyProjects/OrderBookEngine/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(AllTests "D:/MyProjects/OrderBookEngine/build/RelWithDebInfo/OrderBookEngine_tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "D:/MyProjects/OrderBookEngine/CMakeLists.txt;54;add_test;D:/MyProjects/OrderBookEngine/CMakeLists.txt;0;")
else()
  add_test(AllTests NOT_AVAILABLE)
endif()
subdirs("_deps/googletest-build")
