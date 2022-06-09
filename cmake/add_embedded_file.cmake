cmake_minimum_required(VERSION 3.20)

#[=======================================================================[.rst:
add_embedded_file
-----------------

Creates an object-library target that embeds arbitrary file data.

The new target can be linked to other libraries and executables with the
`target_link_libraries` command. `add_embedded_file` uses the `embed` utility
to generate a C++ header/source file pair with variables that refer to
chunks of data from the original file.

The header file, source file, variable names, variable namespace, and maximum
chunksize can all be configured with optional keyword arguments `HPP`, `CPP`,
`IDENT`, `NAMESPACE`, and `CHUNKSIZE` respectively.

If present the `HPP` argument must be a relative path. By default, it is assumed
relative to CMAKE_CURRENT_BINARY_DIR, but this can be overridden with the
`INCLUDE_DIR` keyword argument. This INCLUDE_DIR is added as a PUBLIC include
directory for the generated target.

By default, the header file and identifier are derived from the name of the input
file, with all invalid characters replaced by '_', and the header file name is
appended with an *.hpp extension.

Example
^^^^^^^

  add_embedded_file(mydata data.csv)

  Targets that link to the new `mydata` target will be able to include the
  data via `#include <data_csv.hpp>`. The header file defines the following
  variables:

  1. data_csv_num_chunks: Number of embedded chunks of contiguous data.
  2. data_csv_chunks: Array of pointers to each embedded chunk.
  3. data_csv_chunk_sizes: Array of chunk sizes; one for each chunk.

#]=======================================================================]
macro(add_embedded_file embed_target embed_input)
  set(embed_args0)
  set(embed_args1 HPP CPP INCLUDE_DIR IDENT NAMESPACE CHUNKSIZE)
  set(embed_argsN)
  cmake_parse_arguments(embed "${embed_args0}" "${embed_args1}"
                        "${embed_argsN}" ${ARGN})
  set(embed_INPUT "${embed_input}")
  cmake_path(GET embed_INPUT FILENAME embed_input_filename)

  # Set default output *.hpp file.
  if(NOT embed_HPP)
    set(embed_HPP "${embed_input_filename}.hpp")
  endif()

  # Set default output *.cpp file.
  if(NOT embed_CPP)
    set(embed_CPP "${embed_input_filename}.cpp")
  endif()

  # Set default include directory.
  if(NOT embed_INCLUDE_DIR)
    set(embed_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  endif()

  # Set default identifier
  if(NOT embed_IDENT)
    string(REGEX REPLACE "[^a-zA-Z0-9_]" "_" embed_IDENT
                         "${embed_input_filename}")
  endif()

  # Enforce absolute/relative paths.
  if(IS_ABSOLUTE embed_HPP)
    message(FATAL_ERROR "add_embedded_file: HPP must be relative path")
  endif()
  if(NOT IS_ABSOLUTE embed_INPUT)
    set(embed_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/${embed_INPUT}")
  endif()
  if(NOT IS_ABSOLUTE embed_CPP)
    set(embed_CPP "${CMAKE_CURRENT_BINARY_DIR}/${embed_CPP}")
  endif()

  # Build the command line
  set(embed_CMD
      embed::embed
      -f
      "${embed_INPUT}"
      -I
      "${embed_INCLUDE_DIR}"
      -H
      "${embed_HPP}"
      -C
      "${embed_CPP}"
      -x
      "${embed_IDENT}")
  if(embed_NAMESPACE)
    list(APPEND embed_CMD -n "${embed_NAMESPACE}")
  endif()
  if(embed_CHUNKSIZE)
    list(APPEND embed_CMD -s "${embed_CHUNKSIZE}")
  endif()

  # Create the target
  add_custom_command(
    OUTPUT "${embed_CPP}" "${embed_HPP}"
    COMMAND ${embed_CMD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS embed::embed "${embed_input}")
  add_library(${embed_target} OBJECT "${embed_CPP}")
  target_include_directories(${embed_target} PUBLIC "${embed_INCLUDE_DIR}")
endmacro()
