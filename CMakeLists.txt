cmake_minimum_required(VERSION 3.16)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")

#add_compile_definitions(MyDebug)
add_executable(code ULL.cpp ULL.hpp storeage_test.cpp Char.hpp MemoryRiver.hpp)
