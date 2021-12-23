cmake_minimum_required(VERSION 3.16)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")

#add_compile_definitions(MyDebug)
add_executable(code main.cpp ULL_new.hpp Char.hpp MemoryRiver.hpp Error.hpp UserManager.cpp UserManager.hpp BookManager.cpp BookManager.hpp Logger.cpp Logger.hpp Parser.cpp Parser.hpp Node.cpp Node.hpp main.cpp)
