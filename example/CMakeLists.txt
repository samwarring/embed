add_custom_command(
  OUTPUT lorem.h lorem.cpp
  COMMAND embed -f "${CMAKE_CURRENT_SOURCE_DIR}/lorem.txt" -H lorem.hpp -C
          lorem.cpp -I "${CMAKE_CURRENT_BINARY_DIR}" -x lorem -s 1024
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/lorem.txt" embed)

add_executable(embed_example embed_example.cpp
                             "${CMAKE_CURRENT_BINARY_DIR}/lorem.cpp")
target_include_directories(embed_example PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
