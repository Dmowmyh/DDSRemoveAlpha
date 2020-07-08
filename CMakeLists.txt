cmake_minimum_required(VERSION 3.5)

project(Sandbox)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage -std=c++14 -fsanitize=address")

file(GLOB SOURCE_FILES "*.cpp")
add_executable(exe ${SOURCE_FILES})
