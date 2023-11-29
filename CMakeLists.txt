cmake_minimum_required(VERSION 3.26)
project(http)

set(CMAKE_CXX_STANDARD 17)
link_libraries(ws2_32)
add_executable(http main.cpp
        Request.cpp
        tool.cpp
        ThreadPool.cpp
        ThreadPool.h
)
