cmake_minimum_required(VERSION 3.20)
project(embed VERSION 0.1.0)
option(EMBED_BUILD_EXAMPLE "Enable building of embed/example project" OFF)
option(EMBED_BUILD_TEST "Enable building of embed/test project" OFF)
if(EMBED_BUILD_TEST)
  enable_testing()
endif()

add_subdirectory(src)
include(cmake/add_embedded_file.cmake)

if(EMBED_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

if(EMBED_BUILD_TEST)
  add_subdirectory(test)
endif()
