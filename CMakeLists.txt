cmake_minimum_required(VERSION 3.16)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)

# add_compile_definitions(MyDebug)
add_executable(code ULL.cpp ULL.hpp storeage_test.cpp Char.hpp MemoryRiver.hpp)
