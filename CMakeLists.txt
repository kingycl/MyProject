cmake_minimum_required(VERSION 3.14)

PROJECT(test)

add_executable(Hello Test.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_HOME_DIRECTORY}/lib)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall")
endif()

add_subdirectory(TCPServer)
add_subdirectory(TCPClient)