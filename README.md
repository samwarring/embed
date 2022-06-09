# embed
Converts arbitrary files to global strings for easy embedding in C++ programs. The project
provides a command-line utility and a CMake macro for easy integration in CMake projects.
This aims to implement the [XD](https://stackoverflow.com/a/7288516/1123681) approach, but
provides the following enhancements:
1. XD is widely available on Unix platforms, but less so on Windows. This project aims
to be portable and consumable via submodule.
1. `embed` separates the embedded data into separate .hpp and .cpp files, so consumers
don't #include megabytes of data into every translation unit that uses it.
1. `embed` can add generated variables to a namespace of your choosing.
1. `embed` splits the data into configurably-sized chunks, which better handles files that
exceed the compiler limits for static string/array literals.
1. `embed` provides a CMake macro for easier integration into CMake projects.

## Requirements
`embed` was designed not to use any third-party libraries. However, it does require
the following developer environment:
1. C++17
1. CMake 3.20

## Using `embed` in your CMake Project
1. Add this project as a git submodule.
1. From your CMake Project, `add_subdirectory(embed)`
1. Use the `add_embedded_file()` macro to embed files in your targets.
```
# Assume data.csv is a very large file to be embedded in one or more targets.
add_embedded_file(embedded_data data.csv)

# Embed the bytes of data.csv directly into an executable or library.
add_executable(my_app my_app.cpp)
target_link_libraries(my_app PRIVATE embedded_data)
```
4. Within `my_app.cpp`, the data can be accessed as follows:
```
#include <data_csv.hpp> // Header file named after the input file.
#include <ostream>

void dump_data(std::ostream& out) {
    // Embedded data is divided into chunks to avoid compiler limits
    // on static string sizes.
    for (int i = 0; i < data_csv_num_chunks; ++i) {
        out.write(data_csv_chunks[i], data_csv_chunk_sizes[i]);
    }
}
```

## Customizing the generated files
The easiest form of `add_embedded_file()` takes two arguments: the name for the new target,
and the path to the input file. The macro handles additional keyword arguments for customizing
the output file locations, names of the generated variables, namespace of the generated
variables, and maximum chunk size. The full form of the macro is:
```
add_embedded_file(target input_file
                  [HPP header_file]
                  [CPP source_file]
                  [IDENT identifier]
                  [NAMESPACE namespace]
                  [CHUNKSIZE chunksize]
                  [INCLUDE_DIR include_dir])
```
- The `identifier` controls the base name of all generated variables. This base name is
prepended to `_num_chunks`, `_chunks`, and `_chunk_sizes`.
- The `header_file` controls the filename of the generated header. If specified, this
must be a relative path. The absolute path to the header is the concatenation of
`include_dir` and `header_file`. This lets you organize the generated headers most
conveniently for your project. The `include_dir` is automatically set as a PUBLIC
include directory of the generated target, so consumers can
`#include <relative/path/to/header.hpp>` if desired.
- If `namespace` is given, then the generated variables are declared and defined within the
specified namespace. `embed` currently does not allow for nested namespaces.

## Using `embed` in non-CMake projects
The `embed` project was specifically designed for use with CMake, but other systems can
make use of the code-generation by executing the `embed` utility built by this project.
It's usage follows closely to the CMake macro, but unlike the macro, most of the options
are mandatory. The command line usage is:
```
Usage: embed -f INPUT_FILE -I INCLUDE_DIR -H OUTPUT_HPP_FILE
             -C OUTPUT_CPP_FILE -x NAME [-n NAMESPACE]
             [-s CHUNKSIZE] [-h]

This utility reads an input file and transforms it into a C++ header
file/source file pair. This allows the contents of the original file
to be easily embedded in C++ programs.

Options:
  -h                    Show this help message and exit.
  -f INPUT_FILE         The original file for embedding.
  -I INCLUDE_DIR        Generates headers relative to this include
                         directory.
  -H OUTPUT_HPP_FILE    Output header file declaring the generated
                         variables. This must be a relative path. It
                         will be interpreted relative to INCLUDE_DIR
  -C OUTPUT_CPP_FILE    Output cpp file defining the generated
                         variables.
  -x NAME               Base name for all generated variables.
  -n NAMESPACE          Generates variables in this namespace.
  -s CHUNKSIZE          Splits embedded data in to chunks no greater
                         than this size in bytes. (Default 4096)
```