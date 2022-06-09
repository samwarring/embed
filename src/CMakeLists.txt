add_executable(embed embed.cpp usage.cpp octal.cpp)
add_executable(embed::embed ALIAS embed)
target_include_directories(embed PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
