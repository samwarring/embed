# Generate a large file to be embedded.
message(STATUS "Generating a large file")
set(CHUNKSIZE 256)
set(TEST_FILE "${CMAKE_CURRENT_BINARY_DIR}/test.txt")
file(WRITE "${TEST_FILE}" )
foreach(ROWNUM RANGE 1 ${CHUNKSIZE})
  file(APPEND "${TEST_FILE}" "abcdefghijklmnopqrstuvwxyz123456789\n")
endforeach()
message(STATUS "Generating a large file - done")

# Add the test program. Embed the large file within.
add_executable(embed_test embed_test.cpp)
add_embedded_file(embedded_large_file "${TEST_FILE}" CHUNKSIZE ${CHUNKSIZE})
target_link_libraries(embed_test PRIVATE embedded_large_file)

# Add the CTest case
add_test(NAME embed_test COMMAND embed_test "${TEST_FILE}")
