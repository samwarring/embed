#include <fstream>
#include <iostream>
#include <octal.hpp>
#include <regex>
#include <string>
#include <usage.hpp>
#include <vector>

void error(const std::string& message) {
    std::cerr << "error: " << message << '\n';
    std::exit(1);
}

void open_namespace(std::ostream& out, const options& opts) {
    if (!opts.identifier_namespace.empty()) {
        out << "namespace " << opts.identifier_namespace << " {\n\n";
    }
}

void close_namespace(std::ostream& out, const options& opts) {
    if (!opts.identifier_namespace.empty()) {
        out << "\n} // namespace " << opts.identifier_namespace << '\n';
    }
}

std::size_t process_chunk(std::istream& in, std::ostream& out, const options& opts) {
    const int CHARS_PER_LINE = 20;
    int num_chars_on_line = 0;
    std::size_t chunk_size = 0;
    char ch = '\0';
    bool first = true;
    for (std::size_t i = 0; in && i < (opts.chunk_size - 1); ++i) {
        in.read(&ch, 1);
        if (first) {
            out << "    \"";
            first = false;
        } else if (num_chars_on_line == CHARS_PER_LINE) {
            out << "\"\n    \"";
            num_chars_on_line = 0;
        }
        out << octals[static_cast<unsigned int>(ch)];
        ++num_chars_on_line;
        ++chunk_size;
    }
    if (chunk_size) {
        out << "\",";
    }
    return chunk_size;
}

int main(int argc, char** argv) {
    auto opts = parse_options(argc, argv);

    // Validate identifier and namespace
    std::regex valid_identifier{"[a-zA-Z_][a-zA-Z0-9_]*"};
    if (!std::regex_match(opts.identifier, valid_identifier)) {
        error("identifier not suitable as variable name: " + opts.identifier);
    }
    if (!opts.identifier_namespace.empty() &&
        !std::regex_match(opts.identifier_namespace, valid_identifier)) {
        error("invalid namespace: " + opts.identifier_namespace);
    }

    // Open files for reading and writing.
    std::ifstream input_stream(opts.input_file);
    if (!input_stream) {
        error("failed to open input file: " + opts.input_file.string());
    }
    std::filesystem::path output_hpp = opts.include_dir / opts.output_hpp;
    std::ofstream output_hpp_stream(output_hpp);
    if (!output_hpp_stream) {
        error("failed to open output file: " + output_hpp.string());
    }
    std::ofstream output_cpp_stream(opts.output_cpp);
    if (!output_cpp_stream) {
        error("failed to open output file: " + opts.output_cpp.string());
    }

    // Write the output hpp file.
    output_hpp_stream << "#ifndef INCLUDED_EMBED_" << opts.identifier << '\n'
                      << "#define INCLUDED_EMBED_" << opts.identifier << '\n'
                      << "#include <cstdint>\n"
                      << '\n';
    open_namespace(output_hpp_stream, opts);
    output_hpp_stream << "extern std::size_t " << opts.identifier << "_num_chunks;\n";
    output_hpp_stream << "extern const char* " << opts.identifier << "_chunks[];\n";
    output_hpp_stream << "extern std::size_t " << opts.identifier << "_chunk_sizes[];\n";
    close_namespace(output_hpp_stream, opts);
    output_hpp_stream << "\n#endif\n";
    output_hpp_stream.close();

    // Scan the input file and write the output cpp file.
    output_cpp_stream << "#include <" << opts.output_hpp.string() << ">\n\n";
    open_namespace(output_cpp_stream, opts);
    output_cpp_stream << "const char* " << opts.identifier << "_chunks[] = {\n";

    std::vector<std::size_t> chunk_sizes;
    while (std::size_t chunk_size =
               process_chunk(input_stream, output_cpp_stream, opts)) {
        chunk_sizes.push_back(chunk_size);
        output_cpp_stream << "\n\n";
    }

    output_cpp_stream << "}; // " << opts.identifier << "_chunks\n\n"
                      << "std::size_t " << opts.identifier
                      << "_num_chunks = " << chunk_sizes.size() << ";\n\n"
                      << "std::size_t " << opts.identifier << "_chunk_sizes[] = {\n";
    for (auto chunk_size : chunk_sizes) {
        output_cpp_stream << "    " << chunk_size << ",\n";
    }
    output_cpp_stream << "}; // " << opts.identifier << "_chunk_sizes\n";
    close_namespace(output_cpp_stream, opts);
}