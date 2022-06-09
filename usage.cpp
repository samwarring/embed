#include <iostream>
#include <usage.hpp>

static void usage() {
    std::cout << "Usage: embed -f INPUT_FILE -I INCLUDE_DIR -H OUTPUT_HPP_FILE\n"
                 "             -C OUTPUT_CPP_FILE -x NAME [-n NAMESPACE]\n"
                 "             [-s CHUNKSIZE] [-h]\n"
                 "\n"
                 "This utility reads an input file and transforms it into a C++ header\n"
                 "file/source file pair. This allows the contents of the original file\n"
                 "to be easily embedded in C++ programs.\n"
                 "\n"
                 "Options:\n"
                 "  -h                     Show this help message and exit.\n"
                 "  -f  INPUT_FILE         The original file for embedding.\n"
                 "  -I  INCLUDE_DIR        Generates headers relative to this include\n"
                 "                          directory.\n"
                 "  -H OUTPUT_HPP_FILE     Output header file declaring the generated\n"
                 "                          variables. This must be a relative path. It\n"
                 "                          will be interpreted relative to INCLUDE_DIR\n"
                 "  -c OUTPUT_CPP_FILE     Output cpp file defining the generated\n"
                 "                          variables.\n"
                 "  -x  NAME               Base name for all generated variables.\n"
                 "  -n  NAMESPACE          Generates variables in this namespace.\n"
                 "  -s  CHUNKSIZE          Splits embedded data in to chunks no greater\n"
                 "                         than this size in bytes. (Default 4096)\n"
                 "\n";
    std::exit(0);
}

static std::string next_arg(int argc, char** argv, int& i, const std::string& optname) {
    if (++i >= argc) {
        usage_error("missing required value for '" + optname + "'");
    }
    return argv[i];
}

static int next_arg_int(int argc, char** argv, int& i, const std::string& optname) {
    auto raw = next_arg(argc, argv, i, optname);
    try {
        return std::stoul(raw);
    } catch (const std::exception& e) {
        usage_error("invalid integer for '" + optname + "'");
        return -1;
    }
}

options parse_options(int argc, char** argv) {
    options opts;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-f") {
            opts.input_file = next_arg(argc, argv, i, "-f");
        } else if (arg == "-I") {
            opts.include_dir = next_arg(argc, argv, i, "-I");
        } else if (arg == "-H") {
            opts.output_hpp = next_arg(argc, argv, i, "-H");
        } else if (arg == "-C") {
            opts.output_cpp = next_arg(argc, argv, i, "-C");
        } else if (arg == "-x") {
            opts.identifier = next_arg(argc, argv, i, "-x");
        } else if (arg == "-n") {
            opts.identifier_namespace = next_arg(argc, argv, i, "-n");
        } else if (arg == "-s") {
            opts.chunk_size = next_arg_int(argc, argv, i, "-s");
        } else if (arg == "-h") {
            usage();
        } else {
            usage_error("invalid option '" + arg + "'");
        }
    }
    if (opts.input_file.empty()) {
        usage_error("'-f' is required");
    }
    if (opts.output_hpp.empty()) {
        usage_error("'-H' is required");
    }
    if (opts.output_cpp.empty()) {
        usage_error("'-C' is required");
    }
    if (opts.identifier.empty()) {
        usage_error("'-x' is required");
    }
    return opts;
}

void usage_error(const std::string& message) {
    std::cerr << "error: " << message << '\n' << "note: see usage with '-h'\n";
    std::exit(1);
}