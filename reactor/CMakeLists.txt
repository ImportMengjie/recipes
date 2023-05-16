
cmake_minimum_required(VERSION 3.22.1)
project(reactor)

set(CMAKE_CXX_STANDARD 17)

include_directories(base/)
include_directories(poller/)

add_executable(main main.cc)
