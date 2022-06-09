cmake_minimum_required(VERSION 3.20)
project(embed VERSION 0.1.0)
option(EMBED_BUILD_EXAMPLE "Enable building of embed/example project" OFF)

add_subdirectory(src)
include(cmake/add_embedded_file.cmake)

if(EMBED_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()
