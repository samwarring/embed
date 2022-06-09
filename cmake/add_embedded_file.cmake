macro(add_embedded_file)
  set(embed_args0)
  set(embed_args1
      TARGET
      INPUT
      HPP
      CPP
      INCLUDE_DIR
      IDENT
      NAMESPACE
      CHUNKSIZE)
  set(embed_argsN)
  cmake_parse_arguments(embed "${embed_args0}" "${embed_args1}"
                        "${embed_argsN}" ${ARGN})
  if(NOT embed_TARGET)
    message(FATAL_ERROR "add_embedded_file: Missing TARGET argument")
  endif()
  if(NOT TARGET "${embed_TARGET}")
    message(
      FATAL_ERROR "add_embedded_file: '${embed_TARGET}' is not a valid target")
  endif()
  if(NOT embed_INPUT)
    message(FATAL_ERROR "add_embedded_file: Missing INPUT argument")
  endif()
  if(NOT embed_HPP)
    message(FATAL_ERROR "add_embedded_file: Missing HPP argument")
  endif()
  if(NOT embed_CPP)
    message(FATAL_ERROR "add_embedded_file: Missing CPP argument")
  endif()
  if(NOT embed_IDENT)
    message(FATAL_ERROR "add_embedded_file: Missing IDENT argument")
  endif()
  if(IS_ABSOLUTE embed_HPP)
    message(FATAL_ERROR "add_embedded_file: HPP must be relative path")
  endif()
  if(NOT embed_INCLUDE_DIR)
    set(embed_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  endif()
  if(NOT IS_ABSOLUTE embed_INPUT)
    set(embed_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/${embed_INPUT}")
  endif()
  if(NOT IS_ABSOLUTE embed_CPP)
    set(embed_CPP "${CMAKE_CURRENT_BINARY_DIR}/${embed_CPP}")
  endif()
  set(embed_CMD
      embed
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
  add_custom_command(
    OUTPUT "${embed_CPP}" "${embed_HPP}"
    COMMAND ${embed_CMD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS embed "${embed_INPUT}")
  target_include_directories(${embed_TARGET} PRIVATE "${embed_INCLUDE_DIR}")
  target_sources(${embed_TARGET} PRIVATE "${embed_CPP}")
endmacro()
