#ifndef INCLUDED_EMBED_USAGE_H
#define INCLUDED_EMBED_USAGE_H
#include <filesystem>
#include <string>

struct options {
    std::filesystem::path input_file;
    std::filesystem::path output_hpp;
    std::filesystem::path output_cpp;
    std::filesystem::path include_dir;
    std::string identifier;
    std::string identifier_namespace;
    std::size_t chunk_size{4096};
};

options parse_options(int argc, char** argv);

void usage_error(const std::string& message);

#endif