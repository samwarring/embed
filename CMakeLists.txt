cmake_minimum_required(VERSION 3.20)
project(embed VERSION 0.1.0)
add_executable(embed embed.cpp usage.cpp)
target_include_directories(embed PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
