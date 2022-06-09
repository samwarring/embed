add_executable(embed_example embed_example.cpp)
add_embedded_file(
  TARGET embed_example
  INPUT lorem.txt
  HPP lorem.hpp
  CPP lorem.cpp
  IDENT lorem
  NAMESPACE example)
