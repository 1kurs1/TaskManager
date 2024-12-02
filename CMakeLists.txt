cmake_minimum_required(VERSION 3.5.0)
project(TaskManager VERSION 0.1.1 LANGUAGES C CXX)

add_executable(TaskManager main.cpp)

include(CTest)
enable_testing()

