
cmake_minimum_required(VERSION 3.16.1)
project(reactor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -ggdb")

include_directories(base/)
include_directories(poller/)

aux_source_directory(base/ reactor_base)
aux_source_directory(poller/ reactor_poller)

add_library(reactor ${reactor_base} ${reactor_poller})

add_executable(test_1 test_1.cc)

target_link_libraries(test_1 reactor)
